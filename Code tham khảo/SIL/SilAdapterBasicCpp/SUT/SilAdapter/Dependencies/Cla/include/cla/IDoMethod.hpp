// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "IDoMember.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoMethod : public IDoMember {
public:
#ifdef _CANOECLA
  virtual std::shared_ptr<IValueEntity> GetLatestCall() const = 0;
  virtual std::shared_ptr<IValueEntity> GetLatestReturn() const = 0;
#endif
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
