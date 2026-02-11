// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoEvent.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

DoEvent::DoEvent(std::string fullPath,
  MemberDirection direction,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoMember(std::move(fullPath), MemberType::Event, direction, std::move(logger), std::move(workerThreadService)),
  mValue(std::make_shared<ValueEntity>(mLogger))
{
}

DoEvent::DoEvent(std::string fullPath,
  MemberDirection direction,
  std::shared_ptr<ValueEntity> valueEntity,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoMember(std::move(fullPath), MemberType::Event, direction, std::move(logger), std::move(workerThreadService)),
  mValue(std::move(valueEntity))
{
}

// -------------------
// IDoEvent
// -------------------

std::shared_ptr<IValueEntity> DoEvent::GetValue() const
{
  return mValue;
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
