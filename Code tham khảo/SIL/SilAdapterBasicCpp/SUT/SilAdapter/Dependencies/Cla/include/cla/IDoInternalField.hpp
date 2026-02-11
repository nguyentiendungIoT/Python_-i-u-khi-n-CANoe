// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "IDoField.hpp"
#include "IDoInternalEvent.hpp"
#include "IDoInternalMethod.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoInternalField : public IDoField
{
public:
  virtual std::shared_ptr<IDoInternalEvent> GetInternalEvent() = 0;
  virtual std::shared_ptr<IDoInternalMethod> GetInternalGetter() = 0;
  virtual std::shared_ptr<IDoInternalMethod> GetInternalSetter() = 0;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
