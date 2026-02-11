// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>

#include "IDoInterfaceConsumedMethod.hpp"
#include "IDoInterfaceProvidedMethod.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoInterfaceInternalMethod : public IDoInterfaceMember
{
public:
  virtual ~IDoInterfaceInternalMethod() = default;

  virtual CallbackHandle RegisterReturnedCallback(std::function<void(IDo&, IConsumerCallContext&)> callback) = 0;

  virtual CallbackHandle RegisterCallingCallback(std::function<void(IDo&, IProviderCallContext&)> callback) = 0;
  virtual CallbackHandle RegisterCalledCallback(std::function<void(IDo&, IProviderCallContext&)> callback) = 0;
  virtual CallbackHandle RegisterReturningCallback(std::function<void(IDo&, IProviderCallContext&)> callback) = 0;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
