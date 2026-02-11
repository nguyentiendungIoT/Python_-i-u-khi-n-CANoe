#
# Copyright (c) Vector Informatik GmbH. All rights reserved.
#
# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

import canoe_sil_adapter_runtime.cla

import weakref
from typing import TypeVar, Callable, List, Generic
from enum import IntEnum
from . import typelib_common as typelib_common
from . import typelib_base_datatypes as typelib_base_datatypes

createConsumerCallContextCallbackFunction = typelib_common.getCreateCallbackFunction(canoe_sil_adapter_runtime.cla.ConsumerCallContextCallbackFunction, canoe_sil_adapter_runtime.cla.ConsumerCallContextCallbackFunctionInterface)
createProviderImplementationCallbackFunction = typelib_common.getCreateCallbackFunction(canoe_sil_adapter_runtime.cla.ProviderImplementationCallbackFunction, canoe_sil_adapter_runtime.cla.ProviderImplementationCallbackFunctionInterface)


T = TypeVar("T")
T2 = TypeVar("T2")
T3 = TypeVar("T3")
T4 = TypeVar("T4")


class ParamInOutEnum(IntEnum):
    In = 0
    Out = 1
    InOut = 2


class ParamDef:
    def __init__(self, name: str, datatype: object, inout_type: ParamInOutEnum, is_opt: bool):
        self.name = name
        self.datatype = datatype
        self.inout_type = inout_type
        self.is_opt = is_opt


def list_to_ParamDef(param_list):
    return [ParamDef(*y) for y in param_list]


def _conv_serialization_param(serializer, effdt, optDt, val):
    conversableValue = effdt()
    conversableValue.impl_value = val
    optDt.serialize(serializer, conversableValue, False)


def _is_conv_type(dt) -> bool:
    return isinstance(dt, type) and issubclass(dt, typelib_base_datatypes.DataTypeWithConversion)


def _is_primitive_type(dt) -> bool:
    return dt in (typelib_base_datatypes.BoolDataType, typelib_base_datatypes.DoubleDataType, typelib_base_datatypes.FloatDataType,
        typelib_base_datatypes.StringDataType) or isinstance(dt, typelib_base_datatypes.PrimitiveInt) or \
           isinstance(dt, typelib_base_datatypes.EnumDataType) or _is_conv_type(dt)


def _get_datatype_considering_optionals(dt, isOpt):
    if isOpt:  # isoptional
        if _is_conv_type(dt):
            return typelib_base_datatypes.OptionalConversionDataType(dt)
        else:
            return typelib_base_datatypes.OptionalDataType(dt)
    else:
        return dt



def _deserializeInputParametersAndCreateComplexOutParameters(deserializer, prototype, stored_raw):
    returnList = []
    for paramDef in prototype._param_list:
        val = None

        if paramDef.inout_type != ParamInOutEnum.Out:
            dt = _get_datatype_considering_optionals(paramDef.datatype, paramDef.is_opt)
            val = dt.deserialize(deserializer, stored_raw)
        else:
            val = paramDef.datatype.initial_value()

        if _is_conv_type(paramDef.datatype):
            val = val.impl_value

        if (_is_primitive_type(paramDef.datatype) or paramDef.is_opt):
            if paramDef.inout_type == ParamInOutEnum.Out:
                outParamVal = OutParameter()
                outParamVal.value = val
                val = outParamVal
            elif paramDef.inout_type == ParamInOutEnum.InOut:
                val = InOutParameter(val)

        returnList.append(val)

    return returnList


def _serializeOutputParametersComplexInPlace(serializer, prototype, returnValue, inParamsAndComplexOutParams):
    paramIterator = iter(inParamsAndComplexOutParams)

    if prototype._return_datatype is not None:
        if _is_conv_type(prototype._return_datatype):
            _conv_serialization_param(serializer, prototype._return_datatype, prototype._return_datatype, returnValue)
        else:
            prototype._return_datatype.serialize(serializer, returnValue, False)

    for paramDef in prototype._param_list:
        currentParam = next(paramIterator)

        convType = _is_conv_type(paramDef.datatype)
        primitiveType = _is_primitive_type(paramDef.datatype)

        if paramDef.inout_type != ParamInOutEnum.In:
            if primitiveType or paramDef.is_opt:
                val = currentParam.value
            else:
                val = currentParam

            optDt = _get_datatype_considering_optionals(paramDef.datatype, paramDef.is_opt)

            if convType:
                _conv_serialization_param(serializer, paramDef.datatype, optDt, val)
            else:
                optDt.serialize(serializer, val, False)


def _serializeParameters(serializer, method_prototype, skipped_param_type, values):
        values_iter = iter(values)      

        serializer.BeginSerialization()

        if skipped_param_type == ParamInOutEnum.In and method_prototype._return_datatype is not None:
            method_prototype._return_datatype.serialize(serializer, next(values_iter), False)
      
        for paramDef in method_prototype._param_list:
            if paramDef.inout_type != skipped_param_type:
                optDt = _get_datatype_considering_optionals(paramDef.datatype, paramDef.is_opt)
                convType = _is_conv_type(paramDef.datatype)
                param_value = next(values_iter)
                
                if convType and not isinstance(param_value, typelib_base_datatypes.DataTypeWithConversion):
                    _conv_serialization_param(serializer, paramDef.datatype, optDt, param_value)
                else:
                    optDt.serialize(serializer, param_value, False)

        serializer.EndSerialization()

def _get_impl_values(method_prototype, skipped_param_type, values):
    values_iter = iter(values)
    ret_values = []

    if skipped_param_type == ParamInOutEnum.In and method_prototype._return_datatype is not None:
        val = next(values_iter)
        if _is_conv_type(method_prototype._return_datatype):
            val = val.impl_value
        ret_values.append(val)
      
    for paramDef in method_prototype._param_list:
        if paramDef.inout_type != skipped_param_type:
            dt = _get_datatype_considering_optionals(paramDef.datatype, paramDef.is_opt)
            val = next(values_iter)
            if _is_conv_type(paramDef.datatype):
                val = val.impl_value
            ret_values.append(val)

    return ret_values

def _deserializeParameters(deserializer, method_prototype, skipped_param_type, stored_raw):   
    outParamValues = []
        
    deserializer.BeginDeserialization()

    if skipped_param_type == ParamInOutEnum.In and method_prototype._return_datatype is not None:
        outParamValues.append(method_prototype._return_datatype.deserialize(deserializer, stored_raw))
      
    for paramDef in method_prototype._param_list:
        if paramDef.inout_type != skipped_param_type:
            dt = _get_datatype_considering_optionals(paramDef.datatype, paramDef.is_opt)
            outParamValues.append(dt.deserialize(deserializer, stored_raw))

    deserializer.EndDeserialization()

    return outParamValues

    
class OutParameter(Generic[T]):
    def __init__(self):
        self._value = None

    @property
    def value(self) -> T:
        return self._value

    @value.setter
    def value(self, val: T):
        self._value = val


class InOutParameter(Generic[T], OutParameter[T]):
    def __init__(self, value=None):
        super().__init__()
        self._value = value


class CallAsyncCallback:
    def __init__(self, cco, callback, pyMethod):
        self._cco = cco
        self._callback = callback
        self._pyMethod = pyMethod

    def __call__(self):
        if (self._callback is not None) and (self._cco.GetCallState() is not canoe_sil_adapter_runtime.cla.CallState_Discarded):
            deserializer = self._cco.GetOutputParametersDeserializer()
            outParamValues = _deserializeParameters(deserializer, self._pyMethod._method_prototype, ParamInOutEnum.In, self._pyMethod._stored_rx_raw)
            outParamValuesImpl = _get_impl_values(self._pyMethod._method_prototype, ParamInOutEnum.In, outParamValues)
            self._callback(*outParamValuesImpl)

        self._callback = None
        self._cco = None


class MethodBase():

    _method_prototype = None
    CallContext: Callable = lambda cco: None
    
    def __init__(self, doInterfaceImpl, doMethodPath, method):
        self._method = method
        self._doMethodPath = doMethodPath
        self._doInterfaceImpl = doInterfaceImpl

    @property
    def path(self):
        return self._doInterfaceImpl._doPath + "." + self._doMethodPath

class ProvidedMethod(MethodBase, ):
    _stored_rx_raw = True
    def __init__(self, do, doMethodPath, method):
        super(ProvidedMethod, self).__init__(do, doMethodPath, method)

    @staticmethod
    def _CallImplementationCallback(providedmethod_weakref, cco, callback):
        if providedmethod_weakref():
            providedmethod = providedmethod_weakref()
            deserializer = cco.GetInputParametersDeserializer()
            deserializer.BeginDeserialization()
            inParamsAndComplexOutParams = _deserializeInputParametersAndCreateComplexOutParameters(deserializer, providedmethod._method_prototype, providedmethod._stored_rx_raw)
            deserializer.EndDeserialization()

            returnValue = callback(*inParamsAndComplexOutParams)

            serializer = cco.GetOutputParametersSerializer()
            serializer.BeginSerialization()
            _serializeOutputParametersComplexInPlace(serializer, providedmethod._method_prototype, returnValue, inParamsAndComplexOutParams)
            serializer.EndSerialization()

    def _DoSetCallHandler(self, callback):
        if callback is None:
            self._method.SetCallback(canoe_sil_adapter_runtime.cla.ProviderImplementationCallbackFunction(None))
        else:
            self_weakref = weakref.ref(self)
            cbWrapper = createProviderImplementationCallbackFunction(lambda cco: ProvidedMethod._CallImplementationCallback(self_weakref, cco, callback))
            self._method.SetCallback(cbWrapper)


class ConsumedMethod(MethodBase, ):
    _stored_rx_raw = True
    def __init__(self, do, doMethodPath, method):
        super(ConsumedMethod, self).__init__(do, doMethodPath, method)

    def _DoCallAsync(self, callback, inParams):
        cco = self._method.CreateCallContext()

        serializer = cco.GetInputParametersSerializer()
        _serializeParameters(serializer, self._method_prototype, ParamInOutEnum.Out, inParams)
        cbWrapper = createConsumerCallContextCallbackFunction(CallAsyncCallback(cco, callback, self))
        cco.CallAsync(cbWrapper)


