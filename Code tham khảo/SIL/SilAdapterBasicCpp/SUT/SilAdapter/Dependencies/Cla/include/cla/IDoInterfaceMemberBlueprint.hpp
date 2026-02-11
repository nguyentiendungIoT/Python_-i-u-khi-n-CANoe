// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>

#include "DataTypes.hpp"
#include "IDoBlueprint.hpp"
#include "IVirtualNetwork.hpp"

namespace Vector {
  namespace CANoe {
    namespace Cla {

      class IDoInterfaceMemberBlueprint : public IDoBlueprint
      {
      public:
        virtual BlueprintResult SetVirtualNetwork(std::shared_ptr<IVirtualNetwork> vn) = 0;
        virtual BlueprintResult RemoveVirtualNetwork() = 0;
      };

    }
  }
}