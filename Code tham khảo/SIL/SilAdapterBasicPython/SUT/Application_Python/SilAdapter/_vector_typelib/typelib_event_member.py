# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from typing import TypeVar, Generic
from . import typelib_common as typelib_common
from . import typelib_do as typelib_do


T = TypeVar("T")
T2 = TypeVar("T2")
T3 = TypeVar("T3")
T4 = TypeVar("T4")


class ProvidedEventBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, typelib_common.SetMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger):
        member = doInterfaceImpl._do.GetProvidedEvent(embedded_prefix +  do_member_name, tx_trigger)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, stored_raw)


class ConsumedEventBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, Generic[T]):
    def __init__(self, doInterfaceImpl, embedded_prefix, do_member_name, dt):
        member = doInterfaceImpl._do.GetConsumedEvent(embedded_prefix +  do_member_name)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, True)


class InternalEventBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, typelib_common.SetMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl, embedded_prefix, do_member_name, dt):
        member = doInterfaceImpl._do.GetInternalEvent(embedded_prefix +  do_member_name)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, False)


class TriggerMixin:
    def trigger(self) -> None:
        self._serializeValue(self._dt.initial_value())


class ProvidedEvent(ProvidedEventBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger)


class ConsumedEvent(ConsumedEventBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt)


class InternalEvent(InternalEventBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt)



class ConsumedVoidEvent(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, doInterfaceImpl._do.GetConsumedEvent(embedded_prefix +  do_member_name).GetValue(), dt, True)


class ProvidedVoidEvent(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, TriggerMixin):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger):
        self._stored_raw = stored_raw
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, doInterfaceImpl._do.GetProvidedEvent(embedded_prefix +  do_member_name, tx_trigger).GetValue(), dt, True)


class InternalVoidEvent(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, TriggerMixin):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, doInterfaceImpl._do.GetInternalEvent(embedded_prefix +  do_member_name).GetValue(), dt, False)

