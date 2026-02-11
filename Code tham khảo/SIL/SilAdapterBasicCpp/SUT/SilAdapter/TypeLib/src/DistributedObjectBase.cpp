// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "TypeLib/DistributedObjectBase.hpp"

namespace Vector {
namespace CANoe {
namespace TypeLib {

// Protected Methods
// -------------------

void DistributedObjectBase::_Init(Cla::IClaService* claService, const std::string& doPath, const std::string&)
{
  mDo = claService->GetDo(doPath);
}

void DistributedObjectBase::_WaitForFirstTransmission() {}

} // namespace TypeLib
} // namespace CANoe
} // namespace Vector
