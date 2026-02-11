// This code was generated automatically. Changes to this file may
// cause incorrect behavior and will be lost if the code is regenerated.

#pragma once

#include "RoomTemperatureControl_Internal.hpp"

#include "cla/ClaSil.hpp"

namespace Vector {
namespace CANoe {
namespace SilAdapter {

class SilAdapter
{
public:
  SilAdapter();

  RoomTemperatureControl::NamespaceObjects RoomTemperatureControl;

  void Connect();
  void Connect(SilKit::IParticipant* participant);
  void Disconnect();
  void WaitForFirstTransmissions();

private:
  std::shared_ptr<Cla::IClaServiceSil> mClaService;
};

SilAdapter& Get();

} // namespace SilAdapter
} // namespace CANoe
} // namespace Vector
