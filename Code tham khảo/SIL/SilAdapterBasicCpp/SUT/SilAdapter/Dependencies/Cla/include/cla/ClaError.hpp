// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <stdexcept>

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Base class for all exceptions thrown in the CLA implementation. */
class ClaError : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
