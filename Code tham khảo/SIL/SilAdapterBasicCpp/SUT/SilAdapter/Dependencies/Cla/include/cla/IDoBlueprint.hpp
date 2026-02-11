// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "DataTypes.hpp"
#include "IAttributeDefinition.hpp"

namespace Vector {
  namespace CANoe {
    namespace Cla {

      class IDoBlueprint
      {
      public:
        virtual std::pair<std::uint64_t, BlueprintResult> GetAttributeValueUInt64(std::shared_ptr<IAttributeDefinition> attribute) = 0;
        virtual std::pair<std::int64_t, BlueprintResult> GetAttributeValueInt64(std::shared_ptr<IAttributeDefinition> attribute) = 0;
        virtual std::pair<std::double_t, BlueprintResult> GetAttributeValueDouble(std::shared_ptr<IAttributeDefinition> attribute) = 0;
        virtual std::pair<std::string, BlueprintResult> GetAttributeValueString(std::shared_ptr<IAttributeDefinition> attribute) = 0;
          
        virtual BlueprintResult SetAttributeValueUInt64(std::shared_ptr<IAttributeDefinition> attribute, std::uint64_t value) = 0;
        virtual BlueprintResult SetAttributeValueInt64(std::shared_ptr<IAttributeDefinition> attribute, std::int64_t value) = 0;
        virtual BlueprintResult SetAttributeValueDouble(std::shared_ptr<IAttributeDefinition> attribute, std::double_t value) = 0;
        virtual BlueprintResult SetAttributeValueString(std::shared_ptr<IAttributeDefinition> attribute, const std::string& value) = 0;

        virtual BlueprintResult RemoveAttributeValue(std::shared_ptr<IAttributeDefinition> attribute) = 0;
      };

    }
  }
}