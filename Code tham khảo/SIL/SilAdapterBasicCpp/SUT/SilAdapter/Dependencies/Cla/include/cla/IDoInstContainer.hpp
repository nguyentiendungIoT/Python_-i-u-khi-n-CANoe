// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>
#include <string>

#include "ClaMacros.hpp"
#include "DataTypes.hpp"
#include "IDoInstContainerElement.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoInstContainer
{
public:
  virtual ~IDoInstContainer() = default;
  virtual ContainerSize GetSize() const = 0;
  virtual IDoInstContainerElement* GetElement(ContainerSize index) = 0;

#ifdef _CANOECLA
  virtual void Resize(ContainerSize newSize) = 0;
#endif

};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
