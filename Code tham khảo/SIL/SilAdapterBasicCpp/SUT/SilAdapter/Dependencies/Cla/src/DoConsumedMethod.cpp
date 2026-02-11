// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "cla/ClaError.hpp"

#include "DoConsumedMethod.hpp"
#include "SilKitCommunicationInfo.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

using SilKit::Services::Rpc::RpcCallStatus;

// -------------------
// DoConsumedMethod
// -------------------

DoConsumedMethod::DoConsumedMethod(std::string fullPath,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoMethod(std::move(fullPath), MemberDirection::Consumed, std::move(logger), std::move(workerThreadService))
{
}

void DoConsumedMethod::OnCallSuccess(std::shared_ptr<ConsumerCallContext> cc, CallHandle callHandle, bool isOneWayCall)
{
  const auto thisShared = shared_from_this();
  if (!thisShared)
  {
    return;
  }

  if (!isOneWayCall)
  {
    std::weak_ptr<DoConsumedMethod> thisWeak = thisShared;
    cc->SetOnDestroyHandler([thisWeak, callHandle]() {
      auto thisShared = thisWeak.lock();
      if (!thisShared)
      {
        return;
      }

      thisShared->PopCallContext(callHandle);
    });
  }
  else
  {
    mPendingCallContexts.erase(callHandle);
  }
}

void DoConsumedMethod::SendRequest(ConsumerCallContext* cc, bool isOneWayCall)
{
  std::weak_ptr<ConsumerCallContext> ccWeak = cc->shared_from_this();
  auto call = [&, ccWeak, isOneWayCall]() {
    const auto ccShared = ccWeak.lock();
    if (ccShared == nullptr)
    {
      return CallResult::CallContextDestroyed;
    }

    if (!IsConnected())
    {
      return CallResult::NotConnected;
    }

    const auto callHandle = reinterpret_cast<CallHandle>(mNextCallHandle++);
    std::lock_guard<std::mutex> lock(mPendingCallContextsMutex);
    mPendingCallContexts[callHandle] = ccWeak;
    mRpcClient->Call(ccShared->GetInputParametersData(), callHandle);

    // If the call context was removed from mPendingCallContext in 'Call',
    // the server was not reachable.
    const auto it = mPendingCallContexts.find(callHandle);
    if (it != mPendingCallContexts.end())
    {
      OnCallSuccess(ccShared, callHandle, isOneWayCall);
      return CallResult::Success;
    }

    return CallResult::ServerNotReachable;
  };

  DispatchCall(call);
}

void DoConsumedMethod::DispatchCall(const std::function<CallResult()>& call)
{
  std::lock_guard<std::mutex> lock(mRetryCallTasksMutex);
  if (mIsServerReachable)
  {
    const auto callResult = call();
    if (callResult == CallResult::ServerNotReachable || callResult == CallResult::NotConnected)
    {
      mIsServerReachable = false;
      if (mRetryCallTasks.empty())
      {
        // Start retrying on worker thread
        mWorkerThreadService->PushTask([&]() { return RetryCalls(); });
      }
      mRetryCallTasks.emplace_back(call);
    }
  }
  else
  {
    mRetryCallTasks.emplace_back(call);
  }
}

bool DoConsumedMethod::RetryCalls()
{
  std::lock_guard<std::mutex> lock(mRetryCallTasksMutex);

  auto retryCallIterator = mRetryCallTasks.begin();
  while (retryCallIterator < mRetryCallTasks.end())
  {
    auto call = *retryCallIterator;
    const auto callResult = call();
    if (callResult == CallResult::ServerNotReachable || callResult == CallResult::NotConnected)
    {
      // Explicit fmt::format because of VS2017
      mLogger->debug(fmt::format("No connection between server and client was established for {}.", GetFullPath()));
      mIsServerReachable = false;
      break;
    }

    if (callResult == CallResult::CallContextDestroyed)
    {
      // force retry deletion
    }
    else if (callResult == CallResult::Success)
    {
      // Explicit fmt::format because of VS2017
      mLogger->debug(fmt::format("Server is now reachable and call was successful for {}.", GetFullPath()));
      mIsServerReachable = true;
    }
    ++retryCallIterator;
  }

  // Remove calls from retry list that were successful or their CallContext was destroyed
  if (retryCallIterator != mRetryCallTasks.begin())
  {
    mRetryCallTasks.erase(mRetryCallTasks.begin(), retryCallIterator);
  }

  const auto stopRetry = mIsServerReachable == true;
  return stopRetry;
}

// -------------------
// DoMember
// -------------------

void DoConsumedMethod::Connect(SilKit::IParticipant* participant)
{
  std::lock_guard<std::mutex> lock(mRetryCallTasksMutex);
  mParticipant = participant;

  const SilKitCommunicationInfo communicationInfo(mFullPath);

  auto callReturnHandler = [this](SilKit::Services::Rpc::IRpcClient*,
                             const SilKit::Services::Rpc::RpcCallResultEvent& callResultEvent) {
    switch (callResultEvent.callStatus)
    {
    case RpcCallStatus::Success:
      OnReceiveResponse(ToStdVector(callResultEvent.resultData), callResultEvent.userContext);
      break;
    case RpcCallStatus::ServerNotReachable:
      mPendingCallContexts.erase(callResultEvent.userContext);
      break;
    case RpcCallStatus::InternalServerError:
      throw ClaError("Method call failed because of internal server error.");
    case RpcCallStatus::UndefinedError:
      throw ClaError("Method call failed because of undefined error.");
    default:
      break;
    }
  };

  const auto spec = communicationInfo.CreateRpcSpec();
  mRpcClient = mParticipant->CreateRpcClient(communicationInfo.GetCanonicalName(), spec, callReturnHandler);
  mLogger->debug(fmt::format("Member connected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

void DoConsumedMethod::Disconnect()
{
  std::lock_guard<std::mutex> lock(mRetryCallTasksMutex);
  mRetryCallTasks.clear();
  mParticipant = nullptr;
  mRpcClient = nullptr;
  mLogger->debug(fmt::format("Member disconnected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

// -------------------
// IDoMember
// -------------------

MemberDirection DoConsumedMethod::GetDirection() const
{
  return DoMember::GetDirection();
}
MemberType DoConsumedMethod::GetType() const
{
  return DoMember::GetType();
}
bool DoConsumedMethod::IsConnected() const
{
  return DoMember::IsConnected();
}

// -------------------
// IDoConsumedMethod
// -------------------

std::shared_ptr<IConsumerCallContext> DoConsumedMethod::CreateCallContext()
{
  // Pass a weak ptr of this method to the call context lambdas, since
  // the call context could be kept alive longer than the method itself.
  std::weak_ptr<DoConsumedMethod> thisWeak = shared_from_this();

  const auto onCall = [thisWeak](ConsumerCallContext* cc, bool isOneWayCall) {
    const auto thisShared = thisWeak.lock();
    if (!thisShared)
    {
      return;
    }

    thisShared->SendRequest(cc, isOneWayCall);
  };

  return std::make_shared<ConsumerCallContext>(mLogger, onCall);
}

// Protected methods
// -------------------

bool DoConsumedMethod::OnReceiveResponse(const std::vector<uint8_t>& payload, CallHandle callHandle)
{
  const auto callContext = PopCallContext(callHandle).lock();
  if (callContext)
  {
    return callContext->OnReturn(payload);
  }

  mLogger->warn("Received method response for non-existing call context.");
  return false;
}

// Private methods
// -------------------

std::weak_ptr<ConsumerCallContext> DoConsumedMethod::PopCallContext(CallHandle callHandle)
{
  std::lock_guard<std::mutex> lock(mPendingCallContextsMutex);
  const auto it = mPendingCallContexts.find(callHandle);
  if (it != mPendingCallContexts.end())
  {
    const auto ptr = it->second.lock();
    mPendingCallContexts.erase(it);
    return ptr;
  }

  return {};
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
