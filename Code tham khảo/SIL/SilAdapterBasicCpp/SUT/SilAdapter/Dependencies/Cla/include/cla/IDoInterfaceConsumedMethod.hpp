// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>

#include "DataTypes.hpp"
#include "IConsumerCallContext.hpp"
#include "IDo.hpp"
#include "IDoInterfaceMember.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoInterfaceConsumedMethod : public IDoInterfaceMember
{
public:
  virtual ~IDoInterfaceConsumedMethod() = default;

  virtual CallbackHandle RegisterReturnedCallback(std::function<void(IDo&, IConsumerCallContext&)> callback) = 0;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
