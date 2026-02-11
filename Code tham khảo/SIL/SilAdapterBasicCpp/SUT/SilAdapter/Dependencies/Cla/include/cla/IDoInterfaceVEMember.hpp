// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <functional>
#include <memory>

#include "DataTypes.hpp"
#include "IDo.hpp"
#include "IDoInterfaceMember.hpp"
#include "IValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoInterfaceVEMember : public IDoInterfaceMember
{
public:
  virtual ~IDoInterfaceVEMember() = default;

  virtual CallbackHandle RegisterCallback(std::function<void(IDo&, IValueEntity&)> callback, ValueUpdateMode mode) = 0;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
