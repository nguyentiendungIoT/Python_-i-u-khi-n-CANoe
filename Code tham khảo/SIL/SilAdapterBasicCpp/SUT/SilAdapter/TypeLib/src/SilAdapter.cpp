// This code was generated automatically. Changes to this file may
// cause incorrect behavior and will be lost if the code is regenerated.

#include "SilAdapter_Internal.hpp"

namespace Vector {
namespace CANoe {
namespace SilAdapter {

SilAdapter::SilAdapter() : mClaService(Cla::CreateClaService())
{
  Init(RoomTemperatureControl, mClaService.get());
}

void SilAdapter::Connect()
{
  mClaService->Connect("SilAdapter");
  WaitForFirstTransmissions();
}

void SilAdapter::Connect(SilKit::IParticipant* participant)
{
  assert(participant != nullptr);
  mClaService->Connect(participant);
  WaitForFirstTransmissions();
}

void SilAdapter::Disconnect()
{
  mClaService->Disconnect();
}

void SilAdapter::WaitForFirstTransmissions()
{
  WaitForFirstTransmission(RoomTemperatureControl);
}

SilAdapter& Get()
{
  static SilAdapter instance;
  return instance;
}
void Connect()
{
  Get().Connect();
}
void Connect(SilKit::IParticipant* participant)
{
  Get().Connect(participant);
}
void Disconnect()
{
  Get().Disconnect();
}

} // namespace SilAdapter
} // namespace CANoe
} // namespace Vector
