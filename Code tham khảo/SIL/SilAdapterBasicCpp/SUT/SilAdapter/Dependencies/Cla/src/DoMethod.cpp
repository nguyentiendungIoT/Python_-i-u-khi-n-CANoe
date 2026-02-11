// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoMethod.hpp"
#include "SilKitCommunicationInfo.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// -------------------
// DoMethod
// -------------------

DoMethod::DoMethod(std::string fullPath, MemberDirection direction, std::shared_ptr<spdlog::logger> logger, std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoMember(std::move(fullPath), MemberType::Method, direction, std::move(logger), std::move(workerThreadService))
{
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
