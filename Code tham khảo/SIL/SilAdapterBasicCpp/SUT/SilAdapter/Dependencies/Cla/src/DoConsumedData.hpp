// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/IDoConsumedData.hpp"

#include "silkit/services/pubsub/IDataSubscriber.hpp"

#include "DoData.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a consumed data member of a distributed object. */
class DoConsumedData : public DoData, public IDoConsumedData
{
public:
  // Public methods
  // -------------------

  /*! \brief Instantiates a new consumed data member.
   *  \param fullPath The full path of the data member, i.e. "namespace::do.dataMember".
   *  \param logger The logger instance used by the data member and all its child elements for logging.
   *  \param workerThreadService service to add custom tasks to be executed by a worker thread. */
  DoConsumedData(std::string fullPath,
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
  // IDoData
  // -------------------

  std::shared_ptr<IValueEntity> GetValue() const override;

private:
  // Private members
  // -------------------

  SilKit::Services::PubSub::IDataSubscriber* mSubscriber = nullptr;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
