// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/IDoConsumedEvent.hpp"

#include "silkit/services/pubsub/IDataSubscriber.hpp"

#include "DoEvent.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a consumed event member of a distributed object. */
class DoConsumedEvent : public DoEvent, public IDoConsumedEvent
{
public:
  // Public methods
  // -------------------

  /*! \brief Instantiates a new consumed event member.
   *  \param fullPath The full path of the event member, i.e. "namespace::do.event".
   *  \param logger The logger instance used by the event member and all its child elements for logging.
   *  \param workerThreadService service to add custom tasks to be executed by a worker thread. */
  DoConsumedEvent(std::string fullPath,
    std::shared_ptr<spdlog::logger> logger,
    std::shared_ptr<WorkerThreadService> workerThreadService);

  /*! \brief Instantiates a new consumed event member which uses provided value entity.
   *  \param fullPath The full path of the event member, ie. "namespace::do.event".
   *  \param valueEntity The value entity which shall be used by the event.
   *  \param logger The logger instance used by the event member and all its child elements for logging.
   *  \param workerThreadService service to add custom tasks to be executed by a worker thread. */
  DoConsumedEvent(std::string fullPath,
    std::shared_ptr<ValueEntity> valueEntity,
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
  // IDoEvent
  // -------------------

  std::shared_ptr<IValueEntity> GetValue() const override;

  // Private members
  // -------------------

  SilKit::Services::PubSub::IDataSubscriber* mSubscriber = nullptr;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
