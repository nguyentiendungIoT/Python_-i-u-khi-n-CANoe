// This code was generated automatically. Changes to this file may
// cause incorrect behavior and will be lost if the code is regenerated.

#include <future>
#include <cassert>

#include "TypeLib/RoomTemperatureControl/IHeating.hpp"
#include "TypeLib/Accessor.hpp"
#include "TypeLib/Deserializer.hpp"
#include "TypeLib/Serializer.hpp"
#include "TypeLib/ThreadContext.hpp"
using namespace Vector::CANoe::TypeLib;

namespace RoomTemperatureControl {

void IHeating::_Init(Vector::CANoe::Cla::IClaService* claService, const std::string& path, const std::string& subMemberPath)
{
  DistributedObjectBase::_Init(claService, path, subMemberPath);

  Accessor::Init(CoolingState, DistributedObjectBase::mDo.get(), subMemberPath + "CoolingState", TxTrigger::OnUpdate);
  Accessor::Init(HeatingState, DistributedObjectBase::mDo.get(), subMemberPath + "HeatingState", TxTrigger::OnUpdate);
}

void IHeating::_WaitForFirstTransmission()
{
  DistributedObjectBase::_WaitForFirstTransmission();

}

} // namespace RoomTemperatureControl
