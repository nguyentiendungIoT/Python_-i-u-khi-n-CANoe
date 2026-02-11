# This file was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

set(SIL_ADAPTER_NAME SilAdapter)

set(TYPELIB_PUBLIC_HEADERS
    include/SilAdapter/SilAdapter.hpp
    include/TypeLib/RoomTemperatureControl.hpp
    include/TypeLib/RoomTemperatureControl/RoomTemperatureControl_Types.hpp
    include/TypeLib/RoomTemperatureControl/ISensor.hpp
    include/TypeLib/RoomTemperatureControl/IHeating.hpp
    include/TypeLib/RoomTemperatureControl/State.hpp
)

set(TYPELIB_INTERNAL_HEADERS
    src/SilAdapter_Internal.hpp
    src/RoomTemperatureControl_Internal.hpp
)

set(TYPELIB_SOURCES
    src/SilAdapter.cpp
    src/RoomTemperatureControl.cpp
    src/RoomTemperatureControl/ISensor.cpp
    src/RoomTemperatureControl/IHeating.cpp
)
