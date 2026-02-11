// This code was generated automatically. Changes to this file may
// cause incorrect behavior and will be lost if the code is regenerated.

#include <future>
#include <cassert>

#include "TypeLib/RoomTemperatureControl/ISensor.hpp"
#include "TypeLib/Accessor.hpp"
#include "TypeLib/Deserializer.hpp"
#include "TypeLib/Serializer.hpp"
#include "TypeLib/ThreadContext.hpp"
using namespace Vector::CANoe::TypeLib;

namespace RoomTemperatureControl {

void ISensor::_Init(Vector::CANoe::Cla::IClaService* claService, const std::string& path, const std::string& subMemberPath)
{
  DistributedObjectBase::_Init(claService, path, subMemberPath);

  Accessor::Init(Temperature, DistributedObjectBase::mDo.get(), subMemberPath + "Temperature");
}

void ISensor::_WaitForFirstTransmission()
{
  DistributedObjectBase::_WaitForFirstTransmission();

  Accessor::WaitForFirstTransmission(Temperature);
}

} // namespace RoomTemperatureControl
