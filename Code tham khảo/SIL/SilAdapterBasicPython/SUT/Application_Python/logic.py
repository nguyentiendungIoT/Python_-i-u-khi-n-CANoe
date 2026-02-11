from iolayer import State, get_sensor_value1, get_sensor_value2, get_sensor_value3, set_heating_state

HEATING_THRESHOLD = 18.0
COOLING_THRESHOLD = 23.0
BOOL_TO_ON_OFF_MAP = {True: State.On,
                      False: State.Off}

write_to_console_counter = 0

def get_heating_state(temperature: float) -> State:
    return BOOL_TO_ON_OFF_MAP[temperature < HEATING_THRESHOLD]
    

def get_cooling_state(temperature: float) -> State:
    return BOOL_TO_ON_OFF_MAP[temperature > COOLING_THRESHOLD]


def print_status_to_console(sensor1: float, sensor2: float, sensor3: float, heating_state: State, cooling_state: State) -> None:
    print("Sensor1: " + str(sensor1) +
          ", Sensor2: " + str(sensor2) +
          ", Sensor3: " + str(sensor3) +
          ", heating is " + heating_state.name +
          ", cooling is " + cooling_state.name)


def calc_temperature(value1: float, value2: float, value3: float) -> float:
    return (value1 + value2 + value3) / 3


def control_room_temperature():
    global write_to_console_counter

    sensor1 = get_sensor_value1()
    sensor2 = get_sensor_value2()
    sensor3 = get_sensor_value3()

    temperature = calc_temperature(sensor1, sensor2, sensor3) 

    heating_state = get_heating_state(temperature)
    cooling_state = get_cooling_state(temperature)
    
    set_heating_state(heating_state, cooling_state)

    if write_to_console_counter >= 9:
        print_status_to_console(sensor1, sensor2, sensor3, heating_state, cooling_state)
        write_to_console_counter = 0
    else:
        write_to_console_counter += 1
