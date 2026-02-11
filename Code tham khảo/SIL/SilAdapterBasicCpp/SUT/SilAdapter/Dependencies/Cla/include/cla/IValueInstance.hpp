// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "ISerializer.hpp"
#include "IDeserializer.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

class IValueInstance
{
public:
  //! \returns the Serializer for this ValueInstance. The returned pointer is valid throughout the lifetime of this ValueInstance
  virtual ISerializer* GetSerializer() const = 0;
  //! \returns the Deserializer for this ValueInstance. The returned pointer is valid throughout the lifetime of this ValueInstance
  virtual IDeserializer* GetDeserializer() const = 0;
};

}
}
}