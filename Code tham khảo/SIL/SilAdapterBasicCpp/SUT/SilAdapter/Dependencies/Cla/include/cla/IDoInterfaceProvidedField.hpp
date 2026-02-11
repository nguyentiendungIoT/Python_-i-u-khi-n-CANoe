// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "IDoInterfaceField.hpp"
#include "IDoInterfaceProvidedMethod.hpp"

namespace Vector {
  namespace CANoe {
    namespace Cla {

      class IDoInterfaceProvidedField : public IDoInterfaceField
      {
      public:
        virtual std::shared_ptr<IDoInterfaceProvidedMethod> GetProvidedGetter() = 0;
        virtual std::shared_ptr<IDoInterfaceProvidedMethod> GetProvidedSetter() = 0;
      };

    } // namespace Cla
  } // namespace CANoe
} // namespace Vector
