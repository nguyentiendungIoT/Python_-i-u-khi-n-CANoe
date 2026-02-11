// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "IDoInterfaceField.hpp"
#include "IDoInterfaceConsumedMethod.hpp"

namespace Vector {
  namespace CANoe {
    namespace Cla {

      class IDoInterfaceConsumedField : public IDoInterfaceField
      {
      public:
        virtual std::shared_ptr<IDoInterfaceConsumedMethod> GetConsumedGetter() = 0;
        virtual std::shared_ptr<IDoInterfaceConsumedMethod> GetConsumedSetter() = 0;
      };

    } // namespace Cla
  } // namespace CANoe
} // namespace Vector
