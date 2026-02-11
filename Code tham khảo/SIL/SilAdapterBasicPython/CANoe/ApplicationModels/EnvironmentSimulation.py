import vector.canoe
from application_layer import RoomTemperatureControl
from application_layer.Panel import Panel
from random import choice


@vector.canoe.measurement_script
class EnvironmentSimulation:
    
    # Counter for how many (timer) steps the average temperature is increased / decreased
    INCREASE_DECREASE_TEMP_COUNTER = 20


    def __init__(self):
        self.increase_decrease_temperature_counter = self.INCREASE_DECREASE_TEMP_COUNTER
        self.decrease_temperature = True
        self.timer = vector.canoe.Timer(0.1, self.increase_decrease_temperature)

    @vector.canoe.on_change(Panel.EnvironmentSimulationActive)
    def on_automatic_mode_changed(self):
        if Panel.EnvironmentSimulationActive.copy() == True:
            RoomTemperatureControl.Sensor1.Temperature = 23
            RoomTemperatureControl.Sensor2.Temperature = 23
            RoomTemperatureControl.Sensor3.Temperature = 23
            self.timer.start()
        else:
            self.timer.cancel()


    def increase_decrease_temperature(self):
        # If counter is still > 0, stay with increasing/decreasing temperature
        if self.increase_decrease_temperature_counter > 0:
            self.increase_decrease_temperature_counter -= 1
        else:
            # Counter for increasing/decreasing time interval/steps is 0, so 
            # change from increasing to decreasing temperature or the other way round
            self.decrease_temperature = not self.decrease_temperature
            self.increase_decrease_temperature_counter = self.INCREASE_DECREASE_TEMP_COUNTER

        tempToChange = choice([RoomTemperatureControl.Sensor1, RoomTemperatureControl.Sensor2, RoomTemperatureControl.Sensor3])

        if self.decrease_temperature:
            tempToChange.Temperature = tempToChange.Temperature.copy() - 1
        else:
            tempToChange.Temperature = tempToChange.Temperature.copy() + 1

        self.timer = vector.canoe.Timer(0.1, self.increase_decrease_temperature)
        self.timer.start()
