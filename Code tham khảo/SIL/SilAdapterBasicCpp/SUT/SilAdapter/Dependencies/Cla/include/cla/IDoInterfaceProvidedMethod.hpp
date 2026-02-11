// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>

#include "DataTypes.hpp"
#include "IDo.hpp"
#include "IDoInterfaceMember.hpp"
#include "IProviderCallContext.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoInterfaceProvidedMethod : public IDoInterfaceMember
{
public:
  virtual ~IDoInterfaceProvidedMethod() = default;

  virtual CallbackHandle RegisterCallingCallback(std::function<void(IDo&, IProviderCallContext&)> callback) = 0;
  virtual CallbackHandle RegisterCalledCallback(std::function<void(IDo&, IProviderCallContext&)> callback) = 0;
  virtual CallbackHandle RegisterReturningCallback(std::function<void(IDo&, IProviderCallContext&)> callback) = 0;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
