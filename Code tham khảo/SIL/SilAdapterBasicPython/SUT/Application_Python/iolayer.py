# This file contains functions that access the hardware.
#
# These functions represent the functional system interface of the software under test.
# To establish a communiction with CANoe/CANoe4SW the original source code is substituted
# by calls to the SIL Adapter. Necessary adaptations are marked with "INSTRUMENTATION".

from SilAdapter import RoomTemperatureControl, connect, disconnect
from enum import IntEnum

class State(IntEnum):
    Off = 0,
    On = 1

def initialize_io():
    # ---------- INSTRUMENTATION ----------
    # init_hw()                                              # original source code
    connect()                                                # call to SIL Adapter
    # -------------------------------------

def shut_down_io():
    # ---------- INSTRUMENTATION ----------
    # shut_down_hw()                                         # original source code
    disconnect()                                             # call to SIL Adapter
    # -------------------------------------

def get_sensor_value1() -> float:
    # ---------- INSTRUMENTATION ----------
    # return hw_read_sensor_value1();                        # original source code
    return RoomTemperatureControl.Sensor1.Temperature.copy() # call to SIL Adapter
    # -------------------------------------

def get_sensor_value2() -> float:
    # ---------- INSTRUMENTATION ----------
    # return hw_read_sensor_value2();                        # original source code
    return RoomTemperatureControl.Sensor2.Temperature.copy() # call to SIL Adapter
    # -------------------------------------

def get_sensor_value3() -> float:
    # ---------- INSTRUMENTATION ----------
    # return hw_read_sensor_value3();                        # original source code
    return RoomTemperatureControl.Sensor3.Temperature.copy() # call to SIL Adapter
    # -------------------------------------

def set_heating_state(heatingState: State, coolingState: State) -> None:
    # ---------- INSTRUMENTATION ----------
    # hw_write_switch_heating(heatingState);   # original source code
    # hw_write_switch_cooling(coolingState);   # original source code
    RoomTemperatureControl.Heating.HeatingState = RoomTemperatureControl.State(heatingState)    # call to SIL Adapter
    RoomTemperatureControl.Heating.CoolingState = RoomTemperatureControl.State(coolingState)    # call to SIL Adapter
    # -------------------------------------

