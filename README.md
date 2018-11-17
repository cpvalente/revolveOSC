Revolve OSC
-------------

Transmit rotational data from incremental encoder to d3 media server over OSC

Strategy
-------------
State machine handles the following states
1. GET
- Device reads data from encoder at refresh speed
2. STREAM
- Data is pushed over OSC and websocket
3. SETUP
- In setup mode device serves webpage on its static IP
- Websocket display current encoder value
- Allow entering setup mode
-- change IP address
-- change OSC message name field
-- reset EEPROM data

Notes
-------------
Two onboard buttons allow for entering debug mode and set custom home position
There are 4 onboard LEDs
- Red - Power In
- Green - Calibrated
- Blue 1 - NIU
- Blue 2 - NIU


Hardware
-------------
Encoder - Kubler Incremental Encoder 2048 ppr
https://no.rs-online.com/web/p/rotary-encoders/6666899/

Processing - Arduino Due with Ethernet 2 shield
https://store.arduino.cc/arduino-due
https://store.arduino.cc/arduino-ethernet-shield-2-retail

IC - AM26C32 Differential Line Receiver

Libraries
-------------
Arduino Ethernet2
OSC - https://github.com/CNMAT/OSC
WebSocket - https://github.com/verycollective/ArduinoWebSockets

LINKS
-------------
Due Pin Mapping - https://www.arduino.cc/en/Hacking/PinMappingSAM3X
Due Pin Mapping Graphical - https://forum.arduino.cc/index.php?topic=132130.0
Hardware Quadrature in Due - https://forum.arduino.cc/index.php?topic=140205.0

License

This project is licensed under the MIT License - see the LICENSE.md file for details
