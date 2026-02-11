// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>
#include <string>

#include "ClaMacros.hpp"
#include "DataTypes.hpp"
#include "IDoConsumedData.hpp"
#include "IDoProvidedData.hpp"
#include "IDoConsumedEvent.hpp"
#include "IDoProvidedEvent.hpp"
#include "IDoConsumedField.hpp"
#include "IDoProvidedField.hpp"
#include "IDoConsumedMethod.hpp"
#include "IDoProvidedMethod.hpp"

#ifdef _CANOECLA
#include "IDoInternalData.hpp"
#include "IDoInternalEvent.hpp"
#include "IDoInternalField.hpp"
#include "IDoInternalMethod.hpp"
#endif

namespace Vector {
namespace CANoe {
namespace Cla {

class IDo
{
public:
  virtual ~IDo() = default;


  virtual std::shared_ptr<IDoConsumedData> GetConsumedData(const std::string& path) = 0;
  virtual std::shared_ptr<IDoProvidedData> GetProvidedData(const std::string& path, TxTrigger txTrigger) = 0;
  virtual std::shared_ptr<IDoConsumedEvent> GetConsumedEvent(const std::string& path) = 0;
  virtual std::shared_ptr<IDoProvidedEvent> GetProvidedEvent(const std::string& path, TxTrigger txTrigger) = 0;
  virtual std::shared_ptr<IDoConsumedField> GetConsumedField(const std::string& path) = 0;
  virtual std::shared_ptr<IDoProvidedField> GetProvidedField(const std::string& path, TxTrigger txTrigger) = 0;
  virtual std::shared_ptr<IDoConsumedMethod> GetConsumedMethod(const std::string& path) = 0;
  virtual std::shared_ptr<IDoProvidedMethod> GetProvidedMethod(const std::string& path) = 0;
  
  virtual std::string GetPath() = 0;
  
#ifdef _CANOECLA
  virtual bool IsValid() = 0;
  
  virtual std::shared_ptr<IDoInternalData> GetInternalData(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInternalEvent> GetInternalEvent(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInternalField> GetInternalField(const std::string& path) = 0;
  virtual std::shared_ptr<IDoInternalMethod> GetInternalMethod(const std::string& path) = 0;

  virtual DynamicDOErrorCode Destroy() = 0;
#endif
};

#ifndef _CANOECLA
CLAFUNCTIONDEF std::shared_ptr<IDo> GetDo(const std::string& path);
#endif

} // namespace Cla
} // namespace CANoe
} // namespace Vector
