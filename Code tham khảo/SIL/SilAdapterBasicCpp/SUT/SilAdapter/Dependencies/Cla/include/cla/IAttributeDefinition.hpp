// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>
#include <string>

#include "ClaMacros.hpp"
#include "DataTypes.hpp"

namespace Vector {
  namespace CANoe {
    namespace Cla {

      class IAttributeDefinition
      {
      public:
        virtual ~IAttributeDefinition() = default;

        virtual std::string GetPath() const = 0;
        virtual AttributeDefinitionType GetDataType() const = 0;
      };

    } // namespace Cla
  } // namespace CANoe
} // namespace Vector
