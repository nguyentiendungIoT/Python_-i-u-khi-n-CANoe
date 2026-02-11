// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/ICallContext.hpp"

#include "silkit/services/rpc/IRpcCallHandle.hpp"

#include "ValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

using SilKit::Services::Rpc::IRpcCallHandle;

/*! \brief Class representing a call context for a method call. */
class CallContext : public ICallContext
{
protected:
  // Protected Members
  // -------------------

  ValueEntity mInputParameters;  ///< Buffer for the input parameters.
  ValueEntity mOutputParameters; ///< Buffer for the output parameters.

  // Protected Methods
  // -------------------

  /*! \brief Instantiates a new call context.
   *  \param logger The logger instance used by the call context and all child elements. */
  CallContext(const std::shared_ptr<spdlog::logger>& logger);

};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
