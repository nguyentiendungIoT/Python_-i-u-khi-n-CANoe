from . import typelib_method_member
from typing import TypeVar, Generic
from . import typelib_common as typelib_common
from . import typelib_do as typelib_do

T = TypeVar("T")
T2 = TypeVar("T2")
T3 = TypeVar("T3")
T4 = TypeVar("T4")
T5 = TypeVar("T5", bound=typelib_method_member.MethodBase)
T6 = TypeVar("T6", bound=typelib_method_member.MethodBase)


class ProvidedFieldBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, typelib_common.SetMixin[T], Generic[T]):
    def __init__(self: object, doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type, setter_type, has_event):
        member = doInterfaceImpl._do.GetProvidedField(embedded_prefix +  do_member_name, tx_trigger)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, stored_raw)
        if getter_type != None:
            self._getter = getter_type(do=doInterfaceImpl, doMethodPath=f"{self._doMemberPath}.Get", method=member.GetProvidedGetter())    
        if setter_type != None:
            self._setter = setter_type(do=doInterfaceImpl, doMethodPath=f"{self._doMemberPath}.Set", method=member.GetProvidedSetter())
        if has_event:
            self._event = member.GetProvidedEvent()


class ConsumedFieldBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, Generic[T]):
    def __init__(self: object, doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, setter_type, has_event):
        member = doInterfaceImpl._do.GetConsumedField(embedded_prefix +  do_member_name)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, True)
        if getter_type != None:
            self._getter = getter_type(do=doInterfaceImpl, doMethodPath=f"{self._doMemberPath}.Get", method=member.GetConsumedGetter())    
        if setter_type != None:
            self._setter = setter_type(do=doInterfaceImpl, doMethodPath=f"{self._doMemberPath}.Set", method=member.GetConsumedSetter())
        if has_event:
            self._event = member.GetConsumedEvent()

            
class InternalFieldBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, typelib_common.SetMixin[T], Generic[T]):
    def __init__(self: object, doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, setter_type, has_event):
        member = doInterfaceImpl._do.GetInternalField(embedded_prefix +  do_member_name)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, False)
        if getter_type != None:
            self._getter = getter_type(do=doInterfaceImpl, doMethodPath=f"{self._doMemberPath}.Get", method=member.GetInternalGetter())    
        if setter_type != None:
            self._setter = setter_type(do=doInterfaceImpl, doMethodPath=f"{self._doMemberPath}.Set", method=member.GetInternalSetter())
        if has_event:
            self._event = member.GetInternalEvent()



class GetterMixin(Generic[T]):
    @property
    def get(self) -> T:
        return self._getter


class SetterMixin(Generic[T]):
    @property
    def set(self) -> T:
        return self._setter


class NotifyMixin:
    # def subscribe(self):
    #     pass

    # def unsubscribe(self):
    #     pass
    pass


class ProvidedGetSetNotifyField(ProvidedFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], GetterMixin[T5], NotifyMixin, Generic[T, T5, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type: T5, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type, setter_type, True)


class ConsumedGetSetNotifyField(ConsumedFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], GetterMixin[T5], NotifyMixin, Generic[T, T5, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type: T5, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, setter_type, True)


class InternalGetSetNotifyField(InternalFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], GetterMixin[T5], NotifyMixin, Generic[T, T5, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type: T5, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, setter_type, True)


class ProvidedGetSetField(ProvidedFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], GetterMixin[T5], Generic[T, T5, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type: T5, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type, setter_type, False)


class ConsumedGetSetField(ConsumedFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], GetterMixin[T5], Generic[T, T5, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type: T5, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, setter_type, False)


class InternalGetSetField(InternalFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], GetterMixin[T5], Generic[T, T5, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type: T5, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, setter_type, False)


class ProvidedGetNotifyField(ProvidedFieldBase[T], typelib_common.CopyMixin[T], GetterMixin[T5], NotifyMixin, Generic[T, T5]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type: T5):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type, None, True)


class ConsumedGetNotifyField(ConsumedFieldBase[T], typelib_common.CopyMixin[T], GetterMixin[T5], NotifyMixin, Generic[T, T5]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type: T5):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, None, True)


class InternalGetNotifyField(InternalFieldBase[T], typelib_common.CopyMixin[T], GetterMixin[T5], NotifyMixin, Generic[T, T5]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type: T5):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, None, True)


class ProvidedGetField(ProvidedFieldBase[T], typelib_common.CopyMixin[T], GetterMixin[T5], Generic[T, T5]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type: T5):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, getter_type, None, False)


class ConsumedGetField(ConsumedFieldBase[T], typelib_common.CopyMixin[T], GetterMixin[T5], Generic[T, T5]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type: T5):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, None, False)


class InternalGetField(InternalFieldBase[T], typelib_common.CopyMixin[T], GetterMixin[T5], Generic[T, T5]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type: T5):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, getter_type, None, False)


class ProvidedSetNotifyField(ProvidedFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], NotifyMixin, Generic[T, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, None, setter_type, True)


class ConsumedSetNotifyField(ConsumedFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], NotifyMixin, Generic[T, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, None, setter_type, True)


class InternalSetNotifyField(InternalFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], NotifyMixin, Generic[T, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, None, setter_type, True)


class ProvidedSetField(ProvidedFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], Generic[T, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, None, setter_type, False)


class ConsumedSetField(ConsumedFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], Generic[T, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, None, setter_type, False)


class InternalSetField(InternalFieldBase[T], typelib_common.CopyMixin[T], SetterMixin[T6], Generic[T, T6]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, setter_type: T6):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, None, setter_type, False)


class ProvidedNotifyField(ProvidedFieldBase[T], typelib_common.CopyMixin[T], NotifyMixin, Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, None, None, True)


class ConsumedNotifyField(ConsumedFieldBase[T], typelib_common.CopyMixin[T], NotifyMixin, Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, None, None, True)


class InternalNotifyField(InternalFieldBase[T], typelib_common.CopyMixin[T], NotifyMixin, Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, None, None, True)


class ProvidedField(ProvidedFieldBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger, None, None, False)


class ConsumedField(ConsumedFieldBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, None, None, False)


class InternalField(InternalFieldBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, None, None, False)



