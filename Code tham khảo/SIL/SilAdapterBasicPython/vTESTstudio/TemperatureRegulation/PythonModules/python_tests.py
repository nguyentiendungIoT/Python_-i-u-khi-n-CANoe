import vector.canoe
import vector.canoe.tfs
import vector.canoe.threading
import formatting
from application_layer import RoomTemperatureControl
import sys_pars

@vector.canoe.tfs.export
@vector.canoe.tfs.test_function
def set_sensor_values(sensor1_value: float, sensor2_value: float, sensor3_value: float):
  # Set sensor values
  vector.canoe.tfs.Report.test_step("Setting sensor values to " + ", ".join(str(x) for x in [sensor1_value, sensor2_value, sensor3_value]))
  RoomTemperatureControl.Sensor1.Temperature = sensor1_value
  RoomTemperatureControl.Sensor2.Temperature = sensor2_value
  RoomTemperatureControl.Sensor3.Temperature = sensor3_value

@vector.canoe.tfs.export
@vector.canoe.tfs.test_case("Threshold Behavior")
def threshold_behavior(pThreshold: float, pIsHeatingThreshold: int):
  # Set threshold value
  set_sensor_values(pThreshold, pThreshold, pThreshold)

  # Wait for cyclic update of application
  vector.canoe.threading.wait_for_timeout(sys_pars.MaxReactionTime.copy())

  # Check no heating, no cooling
  check_heating_state(expected_cooling=0, expected_heating=0)

  if pIsHeatingThreshold == 1:
    # Check switch to heating
    # Decrease sensor value 1 by 1°C
    vector.canoe.tfs.Report.test_step("Setting sensor value 1 to " + str(pThreshold - 1))
    RoomTemperatureControl.Sensor1.Temperature = pThreshold - 1

    # Wait for cyclic update of application
    vector.canoe.threading.wait_for_timeout(sys_pars.MaxReactionTime.copy())

    # Check heating on
    check_heating_state(expected_cooling=0, expected_heating=1)
  else:
    # check switch to cooling
    # Increase sensor value 1 by 1°C
    vector.canoe.tfs.Report.test_step("Setting sensor value 1 to " + str(pThreshold + 1))
    RoomTemperatureControl.Sensor1.Temperature = pThreshold + 1

    # Wait for cyclic update of application
    vector.canoe.threading.wait_for_timeout(sys_pars.MaxReactionTime.copy())

    # Check cooling on
    check_heating_state(expected_cooling=1, expected_heating=0)

def check_heating_state(expected_cooling: int, expected_heating: int):
  expected_state = formatting.get_heating_state_string(expected_cooling, expected_heating)
  vector.canoe.tfs.Report.test_step("Checking heating and cooling states. Expected: " + expected_state)

  # Validate heating state
  actual_state = formatting.get_heating_state_string(RoomTemperatureControl.Heating.CoolingState.impl_value, RoomTemperatureControl.Heating.HeatingState.impl_value)
  if expected_cooling == RoomTemperatureControl.Heating.CoolingState.impl_value and expected_heating == RoomTemperatureControl.Heating.HeatingState.impl_value:
    # Report passing check
    vector.canoe.tfs.Report.test_step_pass("Heating and cooling states were: " + actual_state)
  else:
    # Report failing check
    vector.canoe.tfs.Report.test_step_fail("Heating and cooling states were: " + actual_state)
