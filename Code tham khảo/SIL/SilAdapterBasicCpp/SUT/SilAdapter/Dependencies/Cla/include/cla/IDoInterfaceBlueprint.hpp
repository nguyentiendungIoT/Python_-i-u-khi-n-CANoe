// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>
#include <string>

#include "DataTypes.hpp"
#include "IDo.hpp"
#include "IDoBlueprint.hpp"
#include "IDoInterfaceMemberBlueprint.hpp"
#include "IVirtualNetwork.hpp"

namespace Vector {
  namespace CANoe {
    namespace Cla {

      class IDoInterfaceBlueprint : public IDoBlueprint
      {
      public:
        virtual std::shared_ptr<IDoInterfaceBlueprint> GetEmbeddedMemberBlueprint(const std::string& memberName) = 0;
        virtual std::shared_ptr<IDoInterfaceMemberBlueprint> GetMemberBlueprint(const std::string& memberName) = 0;

        virtual BlueprintResult CopyFromObject(std::shared_ptr<IDo> distObj) = 0;
        virtual DynamicDOErrorCode CreateObject(const std::string& path) = 0;

        virtual BlueprintResult AddVirtualNetwork(std::shared_ptr<IVirtualNetwork> vn) = 0;
        virtual BlueprintResult RemoveVirtualNetwork(std::shared_ptr<IVirtualNetwork> vn) = 0;
      };

    }
  }
}