# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from .._vector_typelib import typelib_data_member as typelib_data_member
from .._vector_typelib import typelib_event_member as typelib_event_member
from .._vector_typelib import typelib_field_member as typelib_field_member
from .._vector_typelib import typelib_do as typelib_do
from .._vector_typelib import typelib_base_datatypes as typelib_base_datatypes
from .._vector_typelib import typelib_encodings as typelib_encodings
from .._vector_typelib import typelib_common as typelib_common

from . import State_module as _0_RoomTemperatureControl_State_module

from canoe_sil_adapter_runtime.cla import TxTrigger_OnChange, TxTrigger_OnUpdate

class IHeating(typelib_do.DoInterfaceImpl):
    def __init__(self, identifier, embedded_prefix="", do=None, tx_trigger: dict = {}):
        super().__init__(identifier, embedded_prefix, do)

        self._CoolingState = typelib_data_member.ProvidedData(self, self._embedded_prefix, 'CoolingState', typelib_base_datatypes.EnumDataType(_0_RoomTemperatureControl_State_module.State,  _0_RoomTemperatureControl_State_module.State.OFF, 8), False, tx_trigger.get('CoolingState', TxTrigger_OnUpdate))
        self._HeatingState = typelib_data_member.ProvidedData(self, self._embedded_prefix, 'HeatingState', typelib_base_datatypes.EnumDataType(_0_RoomTemperatureControl_State_module.State,  _0_RoomTemperatureControl_State_module.State.OFF, 8), False, tx_trigger.get('HeatingState', TxTrigger_OnUpdate))

    @property
    def CoolingState(self) -> typelib_data_member.ProvidedData[_0_RoomTemperatureControl_State_module.State]:
        return self._CoolingState

    @CoolingState.setter
    def CoolingState(self, value: _0_RoomTemperatureControl_State_module.State):
        self._CoolingState._set_value(value)

    @property
    def HeatingState(self) -> typelib_data_member.ProvidedData[_0_RoomTemperatureControl_State_module.State]:
        return self._HeatingState

    @HeatingState.setter
    def HeatingState(self, value: _0_RoomTemperatureControl_State_module.State):
        self._HeatingState._set_value(value)

    def _setup_initial_values(self):
        pass

    def _initialize_members(self):
        for member in self._get_members():
            if isinstance(member, typelib_common.ValueEntityWithDataType):
                member._initVE()

    def _get_members(self):
        members = [self._CoolingState, self._HeatingState]
        return members

