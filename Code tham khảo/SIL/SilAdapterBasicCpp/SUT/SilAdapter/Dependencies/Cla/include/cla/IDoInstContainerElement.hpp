// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>

namespace Vector {
namespace CANoe {
namespace Cla {

class IDo;
class IDoInstContainer;

class IDoInstContainerElement
{
public:
  virtual std::shared_ptr<IDo> GetDo() = 0;
  virtual std::shared_ptr<IDoInstContainer> GetContainer() = 0;

#ifdef _CANOECLA
  virtual void CreateDo() = 0;
  virtual void DestroyDo() = 0;
#endif
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
