# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from .._vector_typelib import typelib_data_member as typelib_data_member
from .._vector_typelib import typelib_event_member as typelib_event_member
from .._vector_typelib import typelib_field_member as typelib_field_member
from .._vector_typelib import typelib_do as typelib_do
from .._vector_typelib import typelib_base_datatypes as typelib_base_datatypes
from .._vector_typelib import typelib_encodings as typelib_encodings
from .._vector_typelib import typelib_common as typelib_common


from canoe_sil_adapter_runtime.cla import TxTrigger_OnChange, TxTrigger_OnUpdate

class ISensor(typelib_do.DoInterfaceImpl):
    def __init__(self, identifier, embedded_prefix="", do=None, tx_trigger: dict = {}):
        super().__init__(identifier, embedded_prefix, do)

        self._Temperature = typelib_data_member.ConsumedData(self, self._embedded_prefix, 'Temperature', typelib_base_datatypes.DoubleDataType)

    @property
    def Temperature(self) -> typelib_data_member.ConsumedData[float]:
        return self._Temperature

    def _setup_initial_values(self):
        pass

    def _initialize_members(self):
        for member in self._get_members():
            if isinstance(member, typelib_common.ValueEntityWithDataType):
                member._initVE()

    def _get_members(self):
        members = [self._Temperature]
        return members

