#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// general
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>

// program aux
#define DELAY_MS 1000/30 // 30Hz ish
#define DEBOUNCE_TIME 200
#define WARM_UP_TIME 500

#define FLT_EPSILON 0x0.000002p0 // avoid divide by zero
#define SD_SS_PIN 53

// PCB
#define LED1 2 // 34 is C.2
#define LED2 3 // 35 is C.3
#define LED3 4 // 36 is C.4
#define BTN1 50 // 50 is C.13
#define BTN2 48 // 48 is C.15

// EEPROM
#include <Wire.h>
#define disk1 0x50 
#define EEPROM_ADDRESS 0

// Encoder
#define encoder_cpr 2048 // change to your encoder resolution
#define encoder_quad (encoder_cpr * 4) // quadrature counts is 4 * CPR
#define encoder_angle_step 360.0 / encoder_quad

// OSC
#include <OSCMessage.h>
#define OSC_MS 16

// WebSocket
// #include <WebSocketsServer.h>
// #include <Hash.h>
#define SOCKET_MS 32


#endif
