// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoProvidedMethod.hpp"
#include "ProviderCallContext.hpp"
#include "SilKitCommunicationInfo.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// -------------------
// DoProvidedMethod
// -------------------

DoProvidedMethod::DoProvidedMethod(std::string fullPath,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoMethod(std::move(fullPath), MemberDirection::Provided, std::move(logger), std::move(workerThreadService))
{
}

// -------------------
// DoMethod
// -------------------

void DoProvidedMethod::Connect(SilKit::IParticipant* participant)
{
  mParticipant = participant;

  const SilKitCommunicationInfo communicationInfo(mFullPath);
  const auto spec = communicationInfo.CreateRpcSpec();
  auto callHandler = [this](SilKit::Services::Rpc::IRpcServer* /*server*/,
                       const SilKit::Services::Rpc::RpcCallEvent& callEvent) {
    OnReceiveRequest(ToStdVector(callEvent.argumentData), callEvent.callHandle);
  };

  mRpcServer = mParticipant->CreateRpcServer(communicationInfo.GetCanonicalName(), spec, callHandler);
  mLogger->debug(fmt::format("Member connected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

void DoProvidedMethod::Disconnect()
{
  mParticipant = nullptr;
  mRpcServer = nullptr;
  mLogger->debug(fmt::format("Member disconnected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

void DoProvidedMethod::SendResponse(IRpcCallHandle* callHandle, const std::vector<uint8_t>& data) const
{
  if (!DoMember::IsConnected())
  {
    return;
  }

  mRpcServer->SubmitResult(callHandle, data);
}

// -------------------
// IDoMember
// -------------------

MemberDirection DoProvidedMethod::GetDirection() const
{
  return DoMember::GetDirection();
}
MemberType DoProvidedMethod::GetType() const
{
  return DoMember::GetType();
}
bool DoProvidedMethod::IsConnected() const
{
  return DoMember::IsConnected();
}

// -------------------
// IDoProvidedMethod
// -------------------

void DoProvidedMethod::SetCallback(std::function<void(IProviderCallContext&)> callback)
{
  std::lock_guard<std::mutex> lock(mCallbackMutex);
  mCallback = std::move(callback);
}

// Private methods
// -------------------
// TODO: pass flag if response will be send. -> SilKit needs one way flag
void DoProvidedMethod::OnReceiveRequest(const std::vector<uint8_t>& requestData, IRpcCallHandle* callHandle)
{
  std::lock_guard<std::mutex> lock(mCallbackMutex);
  if (mCallback)
  {
    ProviderCallContext callContext(mLogger, requestData);
    mCallback(callContext);

    if (callContext.WereOutputParametersSerialized())
    {
      SendResponse(callHandle, callContext.GetOutputParametersData());
    }
    else
    {
      SendResponse(callHandle,
        std::vector<uint8_t>()); // TODO: Empty response is avoidable if SilKit one way calls are available
    }
  }
  else
  {
    mLogger->warn(fmt::format("No callback registered for method {}", GetFullPath()));
  }
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
