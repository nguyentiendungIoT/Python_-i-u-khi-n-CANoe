// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <unordered_map>

#include "cla/IDoConsumedMethod.hpp"

#include "silkit/services/rpc/IRpcClient.hpp"

#include "ConsumerCallContext.hpp"
#include "DoMethod.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a consumed method which can be called by
 *         creating a consumer call context. */
class DoConsumedMethod : public DoMethod,
                         public IDoConsumedMethod,
                         public std::enable_shared_from_this<DoConsumedMethod>
{
public:
  enum class CallResult
  {
    Success,
    ServerNotReachable,
    NotConnected,
    CallContextDestroyed
  };

  using CallHandle = void*;
  using RetryTask = std::function<CallResult()>;

  // Public methods
  // -------------------

  /*! \brief Instantiates a new consumed method.
   *  \param fullPath The full path of the consumed method, i.e. 'Namespace::Do.MethodName'.
   *  \param logger The logger which is used for logging of this method
   *                and all its child elements, e.g. created call contexts.
   *  \param workerThreadService service to add custom tasks to be executed by a worker thread. */
  DoConsumedMethod(std::string fullPath,
    std::shared_ptr<spdlog::logger> logger,
    std::shared_ptr<WorkerThreadService> workerThreadService);

  // -------------------
  // DoMember
  // -------------------

  void Connect(SilKit::IParticipant* participant) override;

  void Disconnect() override;

  // -------------------
  // IDoMember
  // -------------------

  MemberType GetType() const override;
  MemberDirection GetDirection() const override;
  bool IsConnected() const override;

  // -------------------
  // IDoConsumedMethod
  // -------------------

  std::shared_ptr<IConsumerCallContext> CreateCallContext() override;

protected:
  // Protected methods
  // -------------------

  /*! \brief Is called when a response is received for the method.
   *  \param payload The payload of the response.
   *  \param callHandle The rpc call handle of the response.
   *  \returns `true` if the response was assigned to a call context and processed, otherwise `false`. */
  virtual bool OnReceiveResponse(const std::vector<uint8_t>& payload, CallHandle callHandle);

  /*! \brief Sends method request via the outgoing connection.
   *  \param cc The context containing the necessary data to perform the call.
   *  \param isOneWayCall Specify whether this request expects a response.
   */
  void SendRequest(ConsumerCallContext* cc, bool isOneWayCall);

private:
  // Private members
  // -------------------

  SilKit::Services::Rpc::IRpcClient* mRpcClient = nullptr;

  std::mutex mPendingCallContextsMutex;
  std::unordered_map<CallHandle, std::weak_ptr<ConsumerCallContext>> mPendingCallContexts;
  std::mutex mRetryCallTasksMutex;
  std::vector<RetryTask> mRetryCallTasks{};
  std::atomic<bool> mIsServerReachable{true};
  std::atomic<std::size_t> mNextCallHandle{1};

  // Private method
  // -------------------

  /*! \brief Removes a pending call context for a specific rpc call handle.
   *  \param callHandle The RPC call handle of the call context which shall be removed.
   *  \returns the removed call context. `empty` if no call context was removed. */
  std::weak_ptr<ConsumerCallContext> PopCallContext(CallHandle callHandle);

  void DispatchCall(const std::function<CallResult()>& call);
  bool RetryCalls();
  void OnCallSuccess(std::shared_ptr<ConsumerCallContext> cc, CallHandle callHandle, bool isOneWayCall);
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
