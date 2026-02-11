// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoMember.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

DoMember::DoMember(std::string fullPath,
  MemberType type,
  MemberDirection direction,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  mLogger(std::move(logger)),
  mFullPath(std::move(fullPath)),
  mWorkerThreadService(std::move(workerThreadService)),
  mType(type),
  mDirection(direction)
{
}

const std::string& DoMember::GetFullPath() const
{
  return mFullPath;
}
std::string DoMember::GetVirtualNetworkName() const
{
  return "Default";
}

// -------------------
// IDoMember
// -------------------

MemberType DoMember::GetType() const
{
  return mType;
}
MemberDirection DoMember::GetDirection() const
{
  return mDirection;
}
bool DoMember::IsConnected() const
{
  return mParticipant != nullptr;
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
