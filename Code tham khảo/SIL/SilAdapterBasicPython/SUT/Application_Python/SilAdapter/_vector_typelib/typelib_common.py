# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from enum import IntEnum
import canoe_sil_adapter_runtime.cla
from typing import TypeVar, Generic, Callable, List, Any
from . import typelib_do as typelib_do
from . import typelib_base_datatypes as typelib_base_datatypes

T = TypeVar("T")
T2 = TypeVar("T2")
T3 = TypeVar("T3")
T4 = TypeVar("T4")

class MemberType(IntEnum):
    Consumed = 0
    Provided = 1
    Internal = 2


class ValueStateEnum(IntEnum):
    """Indicates whether a value has been measured."""
    OfflineValue=0
    MeasurementValue=1


def getCreateCallbackFunction(CallbackFunctionType, CallbackFunctionInterface):
    class CallbackInterfaceImplementation(CallbackFunctionInterface):
        def __init__(self, callback):
            super(CallbackInterfaceImplementation, self).__init__()
            self.callback = callback

        def call(self, *args, **kwargs):
            self.callback(*args, **kwargs)

    def createCallbackFunction(callback):
        callbackInterface = CallbackInterfaceImplementation(callback)
        callbackFunction = CallbackFunctionType(callbackInterface.__disown__())
        return callbackFunction

    return createCallbackFunction


createValueEntityCallbackFunction = getCreateCallbackFunction(canoe_sil_adapter_runtime.cla.ValueEntityCallbackFunction, canoe_sil_adapter_runtime.cla.ValueEntityCallbackFunctionInterface)


class ValueEntity:
    def __init__(self, value_entity: object):
        self._ve = value_entity
        self._on_change_map = {}
        self._on_update_map = {}

    def _register_handler(self, map, veChangeType, callback):
        if (callback not in map.keys()):
            cbWrapper = createValueEntityCallbackFunction(lambda ve: callback())
            cbHandle = self._ve.RegisterCallback(cbWrapper, veChangeType)
            map[callback] = cbHandle

    def _unregister_handler(self, map, callback):
        if (callback in map.keys()):
            self._ve.UnregisterCallback(map[callback])
            map.pop(callback)


class ValueEntityWithDataType(ValueEntity):
    def __init__(self, doInterfaceImpl: typelib_do.DoInterfaceImpl, embedded_prefix: str, do_member_name: str, value_entity: object, dt, stored_raw):
        super().__init__(value_entity)
        self._doInterfaceImpl = doInterfaceImpl
        self._embedded_prefix = embedded_prefix
        self._do_member_name = do_member_name
        self._doMemberPath = embedded_prefix + do_member_name
        self._dt = dt
        self._deserializer = self._ve.GetDeserializer()
        self._serializer = self._ve.GetSerializer()
        self._stored_raw = stored_raw

    def _initVE(self):
        try:
            self._serializeValue(self._dt.initial_value())
        except:
            # Enum definition is empty, cannot serialize initial value
            pass

    def _serializeValue(self, value):
        self._serializer.BeginSerialization()
        try:
            self._dt.serialize(self._serializer, value, self._stored_raw)
        except Exception as e:
            raise
        finally:
            # ensure the serializer is in a valid state
            self._serializer.EndSerialization()

    def _deserializeValue(self):
        self._deserializer.BeginDeserialization()
        try:
            val = self._dt.deserialize(self._deserializer, self._stored_raw)
        except:
            raise
        finally:
            # ensure the deserializer is in a valid state
            self._deserializer.EndDeserialization()
        return val

    @property
    def path(self):
        return self._doInterfaceImpl._doPath + "." + self._doMemberPath


class DataObjPropsBase:
    def _create_data_obj(self, dt_obj, value_entity: object, property_name):
        path = self._branch_path.copy()
        path.append(property_name)
        return dt_obj(value_entity, self._value_entity_wrapper, path)


class OnChangeMixin:
    def register_on_change_handler(self, callback: Callable[[], None]):
        self._register_handler(self._on_change_map, canoe_sil_adapter_runtime.cla.ValueUpdateMode_OnChange, callback)

    def unregister_on_change_handler(self, callback: Callable[[], None]):
        self._unregister_handler(self._on_change_map, callback)


class OnUpdateMixin:
    def register_on_update_handler(self, callback: Callable[[], None]):
        self._register_handler(self._on_update_map, canoe_sil_adapter_runtime.cla.ValueUpdateMode_OnUpdate, callback)

    def unregister_on_update_handler(self, callback: Callable[[], None]):
        self._unregister_handler(self._on_update_map, callback)


class CopyMixin(Generic[T]):
    def copy(self) -> T:
        return self._deserializeValue()


class SetMixin(Generic[T]):
    def _set_value(self, val: T) -> None:
        self._serializeValue(val)


class DataObjSetMixin():
    
    def _set_node_value(self, node_name: str, path: List[str], value: Any):
        ve_val = self._value_entity_wrapper._deserializeValue()
        node_val = ve_val

        for node in path:
            if isinstance(node, int):
                node_val = node_val[node]
            else:                
                if(hasattr(node_val, "discriminator")):
                    current_discriminator = getattr(node_val, "discriminator")
                    enum_class = type(current_discriminator)
                    new_discriminator = getattr(enum_class, node)
                    if(current_discriminator != new_discriminator):
                        initial_value = getattr(node_val, "_union_member_types")[new_discriminator].initial_value()
                        setattr(node_val, node, initial_value)
                
                node_val = getattr(node_val,node)
                    
        if isinstance(node_name, int):
            node_val[node_name] = value
        elif hasattr(node_val, "discriminator") and isinstance(node_val, typelib_base_datatypes.UnionType):
            current_discriminator = getattr(node_val, "discriminator")
            enum_class = type(current_discriminator)
            new_discriminator = getattr(enum_class, node_name)
            node_val._set_value(new_discriminator, value)
        else:
            setattr(node_val, node_name, value)

        setattr(self._value_entity_wrapper._doInterfaceImpl, self._value_entity_wrapper._do_member_name, ve_val)

    def _set_child_node_value(self, node_name: str, value: Any):
        self._set_node_value(node_name, self._branch_path, value)


class ReadOnlyConvMixin(Generic[T]):
    @property
    def impl_value(self) -> T:
        return self._deserializeValue().impl_value


class ReadWriteConvMixin(ReadOnlyConvMixin[T], Generic[T]):
    @ReadOnlyConvMixin.impl_value.setter
    def impl_value(self, val: T):
        newValue = self._dt()
        newValue.impl_value = val
        self._serializeValue(newValue)


class ReadOnlyPhysMixin(Generic[T]):
    @property
    def phys_value(self) -> T:
        return self._deserializeValue().phys_value


class ReadWritePhysMixin(ReadOnlyPhysMixin[T], Generic[T]):
    @ReadOnlyPhysMixin.phys_value.setter
    def phys_value(self, val: T):
        newValue = self._dt()
        newValue.phys_value = val
        self._serializeValue(newValue)


class ReadOnlyRawTypeMixin(Generic[T]):
    @property
    def raw_value(self) -> T:
        return self._deserializeValue().raw_value


class ReadWriteRawTypeMixin(ReadOnlyRawTypeMixin[T], Generic[T]):
    @ReadOnlyRawTypeMixin.raw_value.setter
    def raw_value(self, val: T):
        newValue = self._dt()
        newValue.raw_value = val
        self._serializeValue(newValue)


class ReadOnlySymbValueMixin(Generic[T]):
    @property
    def symb_value(self) -> T:
        return self._deserializeValue().symb_value


class ReadWriteSymbValueMixin(ReadOnlySymbValueMixin[T], Generic[T]):
    @ReadOnlySymbValueMixin.symb_value.setter
    def symb_value(self, val: T):
        newValue = self._dt()
        newValue.symb_value = val
        self._serializeValue(newValue)


class SubmemberBase(DataObjSetMixin):
    def __init__(self, value_entity: object):
        self._ve = value_entity

    def _deserializeValue(self) -> T:
        ve_val = self._value_entity_wrapper._deserializeValue()
        for child_node in self._branch_path:
            if isinstance(child_node, int):
                ve_val = ve_val[child_node]
            else:
                ve_val = getattr(ve_val,child_node)
        return ve_val


class SimpleSubmember(SubmemberBase):
    def __init__(self, value_entity: object, value_entity_wrapper, branch_path):
        super().__init__(value_entity)
        self._value_entity_wrapper = value_entity_wrapper
        self._branch_path = branch_path
        ve_dt = self._value_entity_wrapper._dt
        for child_node in self._branch_path:
            if isinstance(child_node, int):
                ve_dt = ve_dt()._element_dt
            elif issubclass(ve_dt, typelib_base_datatypes.UnionType):
                ve_dt = [y for x,y in ve_dt._union_member_types.items() if x.name==child_node][0]
            else:
                ve_dt = ve_dt._struct_member_types[child_node]
        self._dt = ve_dt    
    
    def _serializeValue(self, value) -> T:
        self._set_node_value(self._branch_path[-1], self._branch_path[0:-2], value) 


class ComplexSubmember(SubmemberBase, CopyMixin[T], Generic[T]):
    pass


class ReadOnlySubmember(SimpleSubmember, CopyMixin[T], Generic[T]):
    pass


class ReadWriteSubmember(SimpleSubmember, CopyMixin[T], Generic[T]):
    pass


