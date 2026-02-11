// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/IDoMethod.hpp"

#include "DoMember.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a method member of a distributed object. */
class DoMethod : public DoMember, public IDoMethod
{
public:
  // -------------------
  // DoMethod
  // -------------------

  /*! \brief Instantiates a new method member.
   *  \param fullPath The full path of the method member, i.e. 'namespace::do.member'.
   *  \param direction The direction of the method member.
   *  \param logger The logger instance used by the method member for logging.
   *  \param workerThreadService service to add custom tasks to be executed by a worker thread. */
  DoMethod(std::string fullPath, MemberDirection direction, std::shared_ptr<spdlog::logger> logger, std::shared_ptr<WorkerThreadService> workerThreadService);
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
