# 4WD-RC-Auto
This project includes two modules:
1. The code for the auto's microcontroller.
2. The code for the remote control unit's microcontroller.

## Hardware components:      
For the auto:
1. Nodemcu ESP12-E dev board (based on esp8266). Datasheet in references.
2. Motor shield with 2 channels. Datasheet in references.
3. 4 DC motors (connected to 4 wheels). Wheels on each side are conntected to the same channel on the motor shield.
4. LC 18650 2499mAh 3.7V Li-ion battery. ("UltraFire")
5. 2 Voltage regulators for controling the voltage sent to the microcontroller (4.5V)  and to the motor shield (14.5V).
6. A fuse connected directly to the battery's + end.

For the remote control unit:
1. Wemos Lolin32 lite dev board (based on esp32). Datasheet in references.
2. Two joysticks (each joystick has two potentiometer for x and y axis controll). Joystick values are read using 10-bit ADC.

## Libraries
Websockets impelmentation for arduino. -- add links --
