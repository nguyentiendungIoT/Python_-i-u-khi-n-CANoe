// This code was generated automatically. Changes to this file may
// cause incorrect behavior and will be lost if the code is regenerated.

#include "TypeLib/Accessor.hpp"
#include "TypeLib/TypeLibUtil.hpp"

#include "SilAdapter_Internal.hpp"
#include "RoomTemperatureControl_Internal.hpp"

#include "TypeLib/RoomTemperatureControl.hpp"

namespace RoomTemperatureControl {

Vector::CANoe::TypeLib::DistributedObject<ISensor>& Sensor1 = Vector::CANoe::SilAdapter::Get().RoomTemperatureControl.Sensor1;
Vector::CANoe::TypeLib::DistributedObject<ISensor>& Sensor2 = Vector::CANoe::SilAdapter::Get().RoomTemperatureControl.Sensor2;
Vector::CANoe::TypeLib::DistributedObject<ISensor>& Sensor3 = Vector::CANoe::SilAdapter::Get().RoomTemperatureControl.Sensor3;
Vector::CANoe::TypeLib::DistributedObject<IHeating>& Heating = Vector::CANoe::SilAdapter::Get().RoomTemperatureControl.Heating;

static void PreInit_Sensor1(RoomTemperatureControl::ISensor& object, Vector::CANoe::Cla::IDo* _do)
{
  Vector::CANoe::TypeLib::MaybeUnused(object, _do);
}

static void PostInit_Sensor1(RoomTemperatureControl::ISensor& object)
{
  Vector::CANoe::TypeLib::MaybeUnused(object);
}

static void Init_Sensor1(ISensor& object, Vector::CANoe::Cla::IClaServiceSil* claService)
{
  const auto _do = claService->GetDo("RoomTemperatureControl::Sensor1");
  PreInit_Sensor1(object, _do.get());
  Vector::CANoe::TypeLib::Accessor::Init(object, claService, "RoomTemperatureControl::Sensor1", "");
  PostInit_Sensor1(object);
}

static void PreInit_Sensor2(RoomTemperatureControl::ISensor& object, Vector::CANoe::Cla::IDo* _do)
{
  Vector::CANoe::TypeLib::MaybeUnused(object, _do);
}

static void PostInit_Sensor2(RoomTemperatureControl::ISensor& object)
{
  Vector::CANoe::TypeLib::MaybeUnused(object);
}

static void Init_Sensor2(ISensor& object, Vector::CANoe::Cla::IClaServiceSil* claService)
{
  const auto _do = claService->GetDo("RoomTemperatureControl::Sensor2");
  PreInit_Sensor2(object, _do.get());
  Vector::CANoe::TypeLib::Accessor::Init(object, claService, "RoomTemperatureControl::Sensor2", "");
  PostInit_Sensor2(object);
}

static void PreInit_Sensor3(RoomTemperatureControl::ISensor& object, Vector::CANoe::Cla::IDo* _do)
{
  Vector::CANoe::TypeLib::MaybeUnused(object, _do);
}

static void PostInit_Sensor3(RoomTemperatureControl::ISensor& object)
{
  Vector::CANoe::TypeLib::MaybeUnused(object);
}

static void Init_Sensor3(ISensor& object, Vector::CANoe::Cla::IClaServiceSil* claService)
{
  const auto _do = claService->GetDo("RoomTemperatureControl::Sensor3");
  PreInit_Sensor3(object, _do.get());
  Vector::CANoe::TypeLib::Accessor::Init(object, claService, "RoomTemperatureControl::Sensor3", "");
  PostInit_Sensor3(object);
}

static void PreInit_Heating(RoomTemperatureControl::IHeating& object, Vector::CANoe::Cla::IDo* _do)
{
  Vector::CANoe::TypeLib::MaybeUnused(object, _do);
}

static void PostInit_Heating(RoomTemperatureControl::IHeating& object)
{
  Vector::CANoe::TypeLib::MaybeUnused(object);
}

static void Init_Heating(IHeating& object, Vector::CANoe::Cla::IClaServiceSil* claService)
{
  const auto _do = claService->GetDo("RoomTemperatureControl::Heating");
  PreInit_Heating(object, _do.get());
  Vector::CANoe::TypeLib::Accessor::Init(object, claService, "RoomTemperatureControl::Heating", "");
  PostInit_Heating(object);
}

void Init(NamespaceObjects& objects, Vector::CANoe::Cla::IClaServiceSil* claService)
{
  Vector::CANoe::TypeLib::MaybeUnused(objects, claService);
  Init_Sensor1(objects.Sensor1, claService);
  Init_Sensor2(objects.Sensor2, claService);
  Init_Sensor3(objects.Sensor3, claService);
  Init_Heating(objects.Heating, claService);
}

void WaitForFirstTransmission(NamespaceObjects& objects)
{
  Vector::CANoe::TypeLib::MaybeUnused(objects);
  Vector::CANoe::TypeLib::Accessor::WaitForFirstTransmission(objects.Sensor1);
  Vector::CANoe::TypeLib::Accessor::WaitForFirstTransmission(objects.Sensor2);
  Vector::CANoe::TypeLib::Accessor::WaitForFirstTransmission(objects.Sensor3);
  Vector::CANoe::TypeLib::Accessor::WaitForFirstTransmission(objects.Heating);
}

} // namespace RoomTemperatureControl
