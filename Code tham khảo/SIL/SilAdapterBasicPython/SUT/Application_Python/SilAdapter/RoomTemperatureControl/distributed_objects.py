# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from . import ISensor_module as _0_RoomTemperatureControl_ISensor_module
from . import IHeating_module as _0_RoomTemperatureControl_IHeating_module
from canoe_sil_adapter_runtime.cla import TxTrigger_OnChange, TxTrigger_OnUpdate
import atexit

Sensor1 = _0_RoomTemperatureControl_ISensor_module.ISensor(
    'RoomTemperatureControl::Sensor1',
    tx_trigger={
    })
Sensor2 = _0_RoomTemperatureControl_ISensor_module.ISensor(
    'RoomTemperatureControl::Sensor2',
    tx_trigger={
    })
Sensor3 = _0_RoomTemperatureControl_ISensor_module.ISensor(
    'RoomTemperatureControl::Sensor3',
    tx_trigger={
    })
Heating = _0_RoomTemperatureControl_IHeating_module.IHeating(
    'RoomTemperatureControl::Heating',
    tx_trigger={
    })

_distributed_objects = (
  Sensor1,
  Sensor2,
  Sensor3,
  Heating,
)

_distributed_object_container = (
)

for do in _distributed_objects:
    do._initialize_members()
    do._setup_initial_values()

for doContainer in _distributed_object_container:
    doContainer._init_do_container_elements()

def _setup_DO_initial_values():
  pass
_setup_DO_initial_values()
