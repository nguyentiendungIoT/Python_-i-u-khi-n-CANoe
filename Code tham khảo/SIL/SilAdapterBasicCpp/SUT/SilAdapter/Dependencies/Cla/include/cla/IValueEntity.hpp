// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <functional>
#include <memory>

#include "DataTypes.hpp"
#include "ISerializer.hpp"
#include "IDeserializer.hpp"

#ifdef _CANOECLA
#include "IValueInstance.hpp"
#endif

namespace Vector {
namespace CANoe {
namespace Cla {

class IValueEntity {
public:
  virtual ~IValueEntity() = default;

  //! \returns the Serializer for this ValueEntity. The returned pointer is valid throughout the lifetime of this ValueEntity
  virtual ISerializer* GetSerializer() const = 0;
  //! \returns the Deserializer for this ValueEntity. The returned pointer is valid throughout the lifetime of this ValueEntity
  virtual IDeserializer* GetDeserializer() const = 0;

  virtual bool IsValid() const = 0;
  virtual ValueState GetState() const = 0;
  virtual Time GetLastUpdateTime() const = 0;
  virtual Time GetLastChangeTime() const = 0;

  virtual CallbackHandle RegisterCallback(std::function<void(IValueEntity&)> callback, ValueUpdateMode mode) = 0;
  virtual void UnregisterCallback(CallbackHandle handle) = 0;

#ifdef _CANOECLA
  virtual uint64_t GetChangeCount() const = 0;
  virtual uint64_t GetUpdateCount() const = 0;
  virtual void ClearChangeFlag() = 0;
  virtual void ClearUpdateFlag() = 0;
  virtual void ResetValueState() = 0;

  virtual std::shared_ptr<IValueInstance> CreateValueInstance() = 0;
#endif
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
