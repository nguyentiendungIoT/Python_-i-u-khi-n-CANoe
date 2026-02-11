#
# Copyright (c) Vector Informatik GmbH. All rights reserved.
#
# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from typing import TypeVar, Generic, Callable
from collections import UserList
from . import typelib_encodings as typelib_encodings
from enum import Enum

T = TypeVar("T")
T2 = TypeVar("T2")
T3 = TypeVar("T3")
T4 = TypeVar("T4")


class VoidDataType:
    @staticmethod
    def initial_value():
        return None
    
    @staticmethod
    def serialize(serializer, value, stored_raw):
        pass

    @staticmethod
    def deserialize(deserializer, stored_raw) -> None:
        pass


class PrimitiveInt:

    def __init__(self, bitSize: int, signed: bool):
        self.bitSize = bitSize
        self.signed = signed

    def serialize(self, serializer, value, stored_raw):
        if (self.signed):
            serializer.SerializeInt64(value, self.bitSize)
        else:
            serializer.SerializeUint64(value, self.bitSize)

    def deserialize(self, serializer, stored_raw) -> int:
        if (self.signed):
            return serializer.DeserializeInt64(self.bitSize)
        else:
            return serializer.DeserializeUint64(self.bitSize)

    @staticmethod
    def initial_value():
        return 0

    def _pythonDataType(self, val):
        return round(val)


class FloatDataType:
    _pythonDataType = float

    @staticmethod
    def initial_value():
        return 0.0

    @staticmethod
    def serialize(serializer, value, stored_raw):
        serializer.SerializeFloat(value)

    @staticmethod
    def deserialize(deserializer, stored_raw) -> float:
        return deserializer.DeserializeFloat()


class DoubleDataType:
    _pythonDataType = float

    @staticmethod
    def initial_value():
        return 0.0

    @staticmethod
    def serialize(serializer, value, stored_raw):
        serializer.SerializeDouble(value)

    @staticmethod
    def deserialize(deserializer, stored_raw) -> float:
        return deserializer.DeserializeDouble()


class EnumDataType(Generic[T]):
    def __init__(self, enumType, initialValue, bitSize):
        self._pythonDataType = enumType
        self.initialValue = initialValue
        self.bit_size = bitSize

    def initial_value(self):
        return self.initialValue

    def serialize(self, serializer, value, stored_raw):
        if isinstance(value, self._pythonDataType):
            serializer.SerializeInt64(value.value, self.bit_size)
        else:
            raise ValueError(f"{value} hat nicht den Typ {self._pythonDataType.__name__}.")

    def deserialize(self, deserializer, stored_raw) -> T:
        if len(self._pythonDataType) > 0:
            return self._pythonDataType(deserializer.DeserializeInt64(self.bit_size))
        else:
            raise ReadEmptyEnumError(f"Auf den Wert des Enums kann nicht zugegriffen werden, da keine Elemente definiert sind: {self._pythonDataType.__name__}.")


class BoolDataType:
    _pythonDataType = bool

    @staticmethod
    def initial_value():
        return False

    @staticmethod
    def serialize(serializer, value, stored_raw):
        serializer.SerializeBool(value)

    @staticmethod
    def deserialize(deserializer, stored_raw) -> bool:
        return deserializer.DeserializeBool()


class StringDataType:
    _pythonDataType = str

    @staticmethod
    def initial_value():
        return ""

    @staticmethod
    def serialize(serializer, value, stored_raw):
        serializer.SerializeString(value)

    @staticmethod
    def deserialize(deserializer, stored_raw) -> str:
        return deserializer.DeserializeString()


class BytesDataType:
    _pythonDataType = bytearray
     
    @staticmethod
    def initial_value():
        return bytearray()

    @staticmethod
    def serialize(serializer, value : bytearray, stored_raw):
        serializer.SerializeBytes(value)

    @staticmethod
    def deserialize(deserializer, stored_raw) -> bytearray:
        return deserializer.DeserializeBytes()


Int8DataType = PrimitiveInt(8, True)
Int16DataType = PrimitiveInt(16, True)
Int32DataType = PrimitiveInt(32, True)
Int64DataType = PrimitiveInt(64, True)

UInt8DataType = PrimitiveInt(8, False)
UInt16DataType = PrimitiveInt(16, False)
UInt32DataType = PrimitiveInt(32, False)
UInt64DataType = PrimitiveInt(64, False)


class UnionType(Generic[T]):
    def __init__(self, initial_value, initial_discriminator_value):
        self._discriminator: T = initial_discriminator_value
        self._value = initial_value

    def _get_value(self, discriminator):
        if self._discriminator == discriminator:
            return self._value
        else:
            raise TypeError(f"Read access of not currently selected union member ({discriminator}!={self._discriminator}).")

    def _set_value(self, discriminator, val):
        self._discriminator = discriminator
        self._value = val

    @property
    def discriminator(self) -> T:
        return self._discriminator

    @staticmethod
    def _serialize(actual_type: Callable, serializer, val, stored_raw):
        serializer.BeginUnion(val.discriminator.value)
        actual_type._union_member_types[val.discriminator].serialize(serializer, val._value, stored_raw)
        serializer.EndUnion()

    @staticmethod
    def _deserialize(actual_type: Callable, discriminator_type, deserializer, stored_raw):
        new_value = actual_type()
        UnionType._deserialize_in_place(actual_type, discriminator_type, deserializer, new_value, stored_raw)
        return new_value

    @staticmethod
    def _deserialize_in_place(actual_type, discriminator_type, deserializer, value, stored_raw):
        value._discriminator = discriminator_type(deserializer.BeginUnion())
        value._value = actual_type._union_member_types[value._discriminator].deserialize(deserializer, stored_raw)
        deserializer.EndUnion()


class StructDataType:
    @staticmethod
    def _serialize(actual_type: Callable, serializer, value, stored_raw):
        serializer.BeginStruct()
        for memberName, datatype in actual_type._struct_member_types.items():
          datatype.serialize(serializer, getattr(value, memberName), stored_raw)
        serializer.EndStruct()

    @staticmethod
    def _deserialize(actual_type: Callable, deserializer, stored_raw):
        newValue = actual_type()
        StructDataType._deserialize_in_place(actual_type, deserializer, newValue, stored_raw)
        return newValue

    @staticmethod
    def _deserialize_in_place(actual_type: Callable, deserializer, value, stored_raw):
        deserializer.BeginStruct()
        for memberName, datatype in actual_type._struct_member_types.items():
            setattr(value, memberName, datatype.deserialize(deserializer, stored_raw))
        deserializer.EndStruct()    


class Array_Base(Generic[T], UserList):
    def __init__(self, num_elements, element_dt):
        self._num_elements = num_elements
        self._element_dt = element_dt
        super().__init__([self._element_dt.initial_value() for k in range(self._num_elements)])

    def __getitem__(self, i) -> T:
        return self.data[i]

    def __setitem__(self, i, item: T):
        self.data[i] = item

    def append(self, item):
        raise TypeError("Append is not available for array types.")

    def remove(self, item):
        raise TypeError("Remove is not available for array types.")

    def __delitem__(self, i):
        raise Exception("Deleting an item is not possible for array types.")

    @staticmethod
    def _serialize(serializer, values, element_dt, stored_raw):
        serializer.BeginArray(len(values))
        for val in values:
            element_dt.serialize(serializer, val, stored_raw)
        serializer.EndArray()

    @staticmethod
    def _deserialize(deserializer, array_dt, element_dt, stored_raw):
        newValue = array_dt()
        Array_Base._deserialize_in_place(deserializer, element_dt, newValue, stored_raw)
        return newValue

    @staticmethod
    def _deserialize_in_place(deserializer, element_dt, value, stored_raw):
        len = deserializer.BeginArray()
        for i in range(0, len):
            value[i] = element_dt.deserialize(deserializer, stored_raw)
        deserializer.EndArray()


class List_Base(Generic[T], UserList):

    def __init__(self, min, max, element_dt, has_conversion=False):
        self._min_size = min
        self._max_size = max
        self._element_dt = element_dt
        self._has_conversion = has_conversion
        super().__init__([self._element_dt.initial_value() for k in range(self._min_size)])

    def append(self, item: T):
        if self._has_conversion:
            raise TypeError("This method is not supported for encoded lists. Use resize() instead.")
        else:
            if(self._max_size == 0 or len(self.data) < self._max_size):
                self.data.append(item)
            else:
                raise IndexError(f"Max size: {self._max_size} is reached.")

    def remove(self, item: T):
        if self._has_conversion:
            raise TypeError("This method is not supported for encoded lists. Use resize() instead.")
        else:
            if len(self.data) > self._min_size:
                self.data.remove(item)
            else:
                raise IndexError(f"Min size: {self._min_size} is reached.")

    def resize(self, count):
        if(self._min_size <= count and (self._max_size == 0 or self._max_size >= count)):
            if len(self.data) < count:
                for n in range(len(self.data), count):
                    self.data.append(self._element_dt.initial_value())
            if len(self.data) > count:
                self.data = self.data[:-(len(self.data)-count)]
        else:
            raise ValueError("Count is out of range.")

    def __getitem__(self, i) -> T:
        return self.data[i]

    def __setitem__(self, i, item: T):
        self.data[i] = item

    def __delitem__(self, i):
        if len(self.data) > self._min_size:
            del self.data[i]
        else:
            raise IndexError("Min size is reached.")

    @staticmethod
    def _serialize(serializer, values, element_dt, stored_raw):
        serializer.BeginArray(len(values))
        for val in values:
            element_dt.serialize(serializer, val, stored_raw)
        serializer.EndArray()

    @staticmethod
    def _deserialize(deserializer, list_dt, element_dt, stored_raw):
        newValue = list_dt()
        List_Base._deserialize_in_place(deserializer, element_dt, newValue, stored_raw)
        return newValue

    @staticmethod
    def _deserialize_in_place(deserializer, element_dt, value, stored_raw):
        len = deserializer.BeginArray()
        value.resize(len)
        for i in range(0, len):
            value[i] = element_dt.deserialize(deserializer, stored_raw)
        deserializer.EndArray()


class DataTypeWithConversion(Generic[T]):
    _rawDt = None
    _physDt = None
    _implDt = None
    _implConv = None
    _rawConv = None  

    def __init__(self):
        self._value: T = self._implDt.initial_value()
        self._stored_type = 0 #impl

    @property
    def impl_value(self) -> T:
        if self._value == None:
          return None
        elif self._stored_type==2: #raw -> phys -> impl
            phys_value = self._physDt._pythonDataType(self._rawConv.Encode(self._value))            
            return self._implDt._pythonDataType(self._implConv.Decode(phys_value))
        elif self._stored_type==1: #phys -> impl
            return self._implDt._pythonDataType(self._implConv.Decode(self._value))
        else:
            return self._implDt._pythonDataType(self._value) # impl


    @impl_value.setter
    def impl_value(self, val: T):
        self._value = val
        self._stored_type = 0


class DataTypePhysMixin(Generic[T]):
    @property
    def phys_value(self) -> T2:
        if self._value == None:
          return None
        elif self._stored_type==2: # raw -> phs
            return self._physDt._pythonDataType(self._rawConv.Encode(self._value)) 
        elif self._stored_type==1: #phys 
            return self._physDt._pythonDataType(self._value)
        else: #impl -> phys
            return self._physDt._pythonDataType(self._implConv.Encode(self._value))

    @phys_value.setter
    def phys_value(self, val: T2):      
        self._value = val
        self._stored_type = 1


class DataTypeRawTypeMixin(Generic[T]):    
    @property
    def raw_value(self) -> T:
        if self._value == None:
          return None
        elif self._stored_type==2: # raw
            return self._rawDt._pythonDataType(self._value)
        elif self._stored_type==1: # phys -> raw
            return self._rawDt._pythonDataType(self._rawConv.Decode(self._value))
        else:
            phys_value = self._rawDt._pythonDataType(self._implConv.Encode(self._value))
            return self._rawDt._pythonDataType(self._rawConv.Decode(phys_value))
    
    @raw_value.setter
    def raw_value(self, val: T):        
        self._stored_type = 2
        self._value = val


class DataTypeSymbValueMixin(Generic[T]):
    @property
    def symb_value(self) -> T:
        if self._value == None:
          return None
        
        try:
            return self._symbDt._pythonDataType(self.impl_value)
        except Exception as e:
            return None

    @symb_value.setter
    def symb_value(self, val: T):
        self.impl_value = val.value


class DataTypeWithConversionRaw(DataTypeWithConversion[T], DataTypeRawTypeMixin[T2], Generic[T, T2]):
    def __init__(self):
        super().__init__()


class DataTypeWithConversionPhys(DataTypeWithConversion[T], DataTypePhysMixin[T2], Generic[T, T2]):
    def __init__(self):
        super().__init__()


class DataTypeWithConversionPhysRaw(DataTypeWithConversion[T], DataTypePhysMixin[T2], DataTypeRawTypeMixin[T3], Generic[T, T2, T3]):
    def __init__(self):
        super().__init__()

        
class DataTypeWithConversionPhysSymb(DataTypeWithConversion[T], DataTypePhysMixin[T2], DataTypeSymbValueMixin[T3], Generic[T, T2, T3]):
    def __init__(self):
        super().__init__()


class DataTypeWithConversionPhysRawSymb(DataTypeWithConversion[T], DataTypePhysMixin[T2], DataTypeRawTypeMixin[T3], DataTypeSymbValueMixin[T4], Generic[T, T2, T3, T4]):
    def __init__(self):
        super().__init__()


class OptionalDataType:
    def __init__(self, innerDataType):
        self._innerDataType = innerDataType

    def deserialize(self, deserializer, stored_raw):
        ret = None
        if (deserializer.BeginOptional()):
            ret = self._innerDataType.deserialize(deserializer, stored_raw)
        deserializer.EndOptional()
        return ret

    def serialize(self, serializer, val, stored_raw):
        serializer.BeginOptional(val is not None)
        if (val is not None):
            self._innerDataType.serialize(serializer, val, stored_raw)
        serializer.EndOptional()


class OptionalConversionDataType:
    def __init__(self, innerDataType):
        self._innerDataType = innerDataType

    def deserialize(self, deserializer, stored_raw):
        ret = None
        if (deserializer.BeginOptional() is True):
            ret = self._innerDataType.deserialize(deserializer, stored_raw)
        else:
            ret = self._innerDataType()
            ret._value = None
        deserializer.EndOptional()
        return ret

    def serialize(self, serializer, val, stored_raw):
        serializer.BeginOptional(val._value is not None)
        if (val._value is not None):
            self._innerDataType.serialize(serializer, val, stored_raw)
        serializer.EndOptional()


def makeConcreteDataTypeWithConversionCommon(implConv, rawConv, implDt, physDt, rawDt, symbDt, BaseType):

    class ConcreteDataTypeWithConversion(BaseType):
        _rawDt = rawDt
        _physDt = physDt
        _implDt = implDt
        _symbDt = symbDt
        _implConv = implConv
        _rawConv = rawConv

        @staticmethod
        def initial_value():
            return ConcreteDataTypeWithConversion()                  

        @staticmethod
        def serialize(serializer, val, stored_raw): 
            if stored_raw and ConcreteDataTypeWithConversion._rawDt != None:
                if val._stored_type==2: # already raw
                   ConcreteDataTypeWithConversion._rawDt.serialize(serializer, val._value, stored_raw)     
                else: # phys -> raw
                   raw_value = ConcreteDataTypeWithConversion._rawDt._pythonDataType(ConcreteDataTypeWithConversion._rawConv.Decode(val.phys_value))
                   ConcreteDataTypeWithConversion._rawDt.serialize(serializer, raw_value, stored_raw)
            else:
                if val._stored_type==0: #already impl
                   ConcreteDataTypeWithConversion._implDt.serialize(serializer, val._value, stored_raw)     
                else: #phys -> impl                   
                   impl_value = ConcreteDataTypeWithConversion._implDt._pythonDataType(ConcreteDataTypeWithConversion._implConv.Decode(val.phys_value))
                   ConcreteDataTypeWithConversion._implDt.serialize(serializer, impl_value, stored_raw)

        @staticmethod
        def deserialize(deserializer, stored_raw):
            newValue = ConcreteDataTypeWithConversion()
            if stored_raw and ConcreteDataTypeWithConversion._rawDt != None:
                rawValue = ConcreteDataTypeWithConversion._rawDt.deserialize(deserializer, stored_raw)
                newValue._value = ConcreteDataTypeWithConversion._rawDt._pythonDataType(rawValue)
                newValue._stored_type = 2 #raw
            else:
                implValue = ConcreteDataTypeWithConversion._implDt.deserialize(deserializer, stored_raw)
                newValue._value = ConcreteDataTypeWithConversion._implDt._pythonDataType(implValue)
                newValue._stored_type = 0 #impl
            return newValue

    return ConcreteDataTypeWithConversion

   

def makeConcreteDataTypeWithConversionRaw(implConv, rawConv, implDt, physDt, rawDt):
    return makeConcreteDataTypeWithConversionCommon(implConv, rawConv, implDt, physDt, rawDt, None, DataTypeWithConversionRaw)


def makeConcreteDataTypeWithConversionPhys(implConv, implDt, physDt):
    return makeConcreteDataTypeWithConversionCommon(implConv, typelib_encodings.IdentityEncoding, implDt, physDt, implDt, None, DataTypeWithConversionPhys)


def makeConcreteDataTypeWithConversionPhysRaw(implConv, rawConv, implDt, physDt, rawDt):
    return makeConcreteDataTypeWithConversionCommon(implConv, rawConv, implDt, physDt, rawDt, None, DataTypeWithConversionPhysRaw)


def makeConcreteDataTypeWithConversionPhysSymb(implConv, implDt, physDt, symbDt):
    return makeConcreteDataTypeWithConversionCommon(implConv, None, implDt, physDt, None, symbDt, DataTypeWithConversionPhysSymb)


def makeConcreteDataTypeWithConversionPhysRawSymb(implConv, rawConv, implDt, physDt, rawDt, symbDt):
    return makeConcreteDataTypeWithConversionCommon(implConv, rawConv, implDt, physDt, rawDt, symbDt, DataTypeWithConversionPhysRawSymb)

class ReadEmptyEnumError(Exception):
    pass
