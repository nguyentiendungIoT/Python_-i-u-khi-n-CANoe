// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/IDoProvidedMethod.hpp"

#include "DoMethod.hpp"
#include "ProviderCallContext.hpp"

#include "silkit/services/rpc/IRpcServer.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a provided method member of a distributed object
 *         on which a callback can be registered. */
class DoProvidedMethod : public DoMethod, public IDoProvidedMethod
{
public:
  // -------------------
  // DoProvidedMethod
  // -------------------

  /*! \brief Instantiates a new provided method.
   *  \param fullPath The full path of the provided method, i.e. 'namespace::do.member'.
   *  \param logger The logger which shall be used for logging by the provided method.
   *  \param workerThreadService service to add custom tasks to be executed by a worker thread. */
  DoProvidedMethod(std::string fullPath,
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
  // IDoProvidedMethod
  // -------------------

  void SetCallback(std::function<void(IProviderCallContext&)> callback) override;

protected:
  // Protected methods
  // -------------------

  void SendResponse(IRpcCallHandle* callHandle, const std::vector<uint8_t>& data) const;

  // Protected members
  // -------------------
  std::function<void(ProviderCallContext&)> mCallback{nullptr};
  std::mutex mCallbackMutex;

  // Protected methods
  // -------------------

  virtual void OnReceiveRequest(const std::vector<uint8_t>& data, IRpcCallHandle* callHandle);

private:
  // Private members
  // -------------------

  SilKit::Services::Rpc::IRpcServer* mRpcServer = nullptr;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
