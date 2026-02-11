// This code was generated automatically. Changes to this file may
// cause incorrect behavior and will be lost if the code is regenerated.

#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>

#include "TypeLib/DistributedObjectBase.hpp"

#include "RoomTemperatureControl_Types.hpp"

#include "TypeLib/RoomTemperatureControl/State.hpp"

namespace RoomTemperatureControl {

class IHeating : virtual public Vector::CANoe::TypeLib::DistributedObjectBase
{
public:
  IHeating() = default;

  // -----------------------
  // Data member
  // -----------------------

  Vector::CANoe::TypeLib::DoProvidedData<RoomTemperatureControl::State> CoolingState;
  Vector::CANoe::TypeLib::DoProvidedData<RoomTemperatureControl::State> HeatingState;

protected:
  friend class Vector::CANoe::TypeLib::Accessor;
  void _Init(Vector::CANoe::Cla::IClaService* claService, const std::string& path, const std::string& subMemberPath) override;
  void _WaitForFirstTransmission() override;
};

} // namespace RoomTemperatureControl
