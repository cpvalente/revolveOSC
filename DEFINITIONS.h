#ifndef DEFINITIONS_H
#define DEFINITIONS_H




// general
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>

// program aux
#define DELAY_MS 1000/30 // 30Hz ish
#define FLT_EPSILON 0x0.000002p0 // avoid divide by zero
#define SD_SS_PIN 53

// PCB
#define LED1 2 // 34 is C.2
#define LED2 3 // 35 is C.3
#define LED3 4 // 36 is C.4

// Encoder
#define encoder_cpr 2048 // change to your encoder resolution
#define encoder_quad (encoder_cpr * 4) // quadrature counts is 4 * CPR

// OSC
#include <OSCMessage.h>

// WebSocket
// #include <WebSocketsServer.h>
// #include <Hash.h>


#endif
