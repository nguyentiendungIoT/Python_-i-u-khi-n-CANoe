#
# Copyright (c) Vector Informatik GmbH. All rights reserved.
#
# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from typing import TypeVar, Generic
from . import typelib_common as typelib_common
from . import typelib_do as typelib_do


T = TypeVar("T")
T2 = TypeVar("T2")
T3 = TypeVar("T3")
T4 = TypeVar("T4")


class ProvidedDataBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, typelib_common.SetMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger):
        member = doInterfaceImpl._do.GetProvidedData(embedded_prefix +  do_member_name, tx_trigger)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, stored_raw)


class ConsumedDataBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, Generic[T]):
    def __init__(self, doInterfaceImpl, embedded_prefix, do_member_name, dt):
        member = doInterfaceImpl._do.GetConsumedData(embedded_prefix +  do_member_name)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, True)


class InternalDataBase(typelib_common.ValueEntityWithDataType, typelib_common.OnUpdateMixin, typelib_common.OnChangeMixin, typelib_common.SetMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl, embedded_prefix, do_member_name, dt):
        member = doInterfaceImpl._do.GetInternalData(embedded_prefix +  do_member_name)
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, member.GetValue(), dt, False)


class ProvidedData(ProvidedDataBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt, stored_raw, tx_trigger)


class ConsumedData(ConsumedDataBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt)


class InternalData(InternalDataBase[T], typelib_common.CopyMixin[T], Generic[T]):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix, do_member_name, dt):
        super().__init__(doInterfaceImpl, embedded_prefix, do_member_name, dt)






