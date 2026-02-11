// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "CallContext.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {


// Protected Methods
// -------------------

CallContext::CallContext(const std::shared_ptr<spdlog::logger>& logger) :
  mInputParameters(logger),
  mOutputParameters(logger)
{
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
