from logic import control_room_temperature
from iolayer import initialize_io, shut_down_io
from time import sleep

print("Room Temperature Control application, version 1.0")

initialize_io()

while True:
    control_room_temperature()
    sleep(0.1)

shut_down_io()