// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoData.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

DoData::DoData(std::string fullPath,
  MemberDirection direction,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoMember(std::move(fullPath), MemberType::DataMember, direction, std::move(logger), std::move(workerThreadService)),
  mValue(std::make_shared<ValueEntity>(mLogger))
{
}

// -------------------
// IDoData
// -------------------

std::shared_ptr<IValueEntity> DoData::GetValue() const
{
  return mValue;
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
