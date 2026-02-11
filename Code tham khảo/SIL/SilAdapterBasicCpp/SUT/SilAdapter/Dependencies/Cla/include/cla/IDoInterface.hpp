// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>
#include <string>

#include "ClaMacros.hpp"
#include "IDoInterfaceBlueprint.hpp"
#include "IDoInterfaceVEMember.hpp"
#include "IDoInterfaceConsumedMethod.hpp"
#include "IDoInterfaceProvidedMethod.hpp"
#include "IDoInterfaceInternalMethod.hpp"
#include "IDoInterfaceConsumedField.hpp"
#include "IDoInterfaceProvidedField.hpp"
#include "IDoInterfaceInternalField.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IDoInterface
{
public:
  virtual ~IDoInterface() = default;

  virtual std::shared_ptr<IDoInterfaceVEMember> GetConsumedData(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceVEMember> GetProvidedData(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceVEMember> GetInternalData(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceVEMember> GetConsumedEvent(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceVEMember> GetProvidedEvent(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceVEMember> GetInternalEvent(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceConsumedField> GetConsumedField(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceProvidedField> GetProvidedField(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceInternalField> GetInternalField(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceConsumedMethod> GetConsumedMethod(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceProvidedMethod> GetProvidedMethod(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInterfaceInternalMethod> GetInternalMethod(const std::string& path) = 0;

  virtual std::shared_ptr<IDoInterfaceBlueprint> CreateBlueprint(bool reversed) = 0;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
