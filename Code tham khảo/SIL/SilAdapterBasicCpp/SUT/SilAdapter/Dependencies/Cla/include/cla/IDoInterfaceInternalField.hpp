// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "IDoInterfaceField.hpp"
#include "IDoInterfaceInternalMethod.hpp"

namespace Vector {
  namespace CANoe {
    namespace Cla {

      class IDoInterfaceInternalField : public IDoInterfaceField
      {
      public:
        virtual std::shared_ptr<IDoInterfaceInternalMethod> GetInternalGetter() = 0;
        virtual std::shared_ptr<IDoInterfaceInternalMethod> GetInternalSetter() = 0;
      };

    } // namespace Cla
  } // namespace CANoe
} // namespace Vector
