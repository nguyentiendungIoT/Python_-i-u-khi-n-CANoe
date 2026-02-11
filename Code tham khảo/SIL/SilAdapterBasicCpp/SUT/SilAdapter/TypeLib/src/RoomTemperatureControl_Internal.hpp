// This code was generated automatically. Changes to this file may
// cause incorrect behavior and will be lost if the code is regenerated.

#pragma once

#include "TypeLib/RoomTemperatureControl.hpp"

#include "cla/IClaServiceSil.hpp"

namespace RoomTemperatureControl {

struct NamespaceObjects
{
  Vector::CANoe::TypeLib::DistributedObject<ISensor> Sensor1;
  Vector::CANoe::TypeLib::DistributedObject<ISensor> Sensor2;
  Vector::CANoe::TypeLib::DistributedObject<ISensor> Sensor3;
  Vector::CANoe::TypeLib::DistributedObject<IHeating> Heating;
};

void Init(NamespaceObjects& objects, Vector::CANoe::Cla::IClaServiceSil* claService);
void WaitForFirstTransmission(NamespaceObjects& objects);

} // namespace RoomTemperatureControl
