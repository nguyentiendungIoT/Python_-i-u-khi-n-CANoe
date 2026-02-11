// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "DataTypes.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoInterfaceMember
{
public:
  virtual ~IDoInterfaceMember() = default;

  virtual void UnregisterCallback(CallbackHandle handle) = 0;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
