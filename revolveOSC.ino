/*
 *  Revolve OSC
 *  -------------
 *  Small project to transmit rotational data from absolute encoder to d3 media server
 *  Data transmission over OSC
 *
 *  Code by Carlos Valente
 *
 *
 */

#include "DEFINITIONS.h"

int32_t pos;

static uint8_t state;   // variable for state control
enum states {GET, STREAM, SETUP};

/* Initialize libraries */
EthernetUDP Udp;

// network stuff
byte MAC[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x07, 0x74 };
IPAddress IP(10, 0, 0, 11);
IPAddress TARGET_IP(10, 0, 0, 255); // Broadcast for now
int PORT = 8888;

void setup(){

  /* Initialize aux */
  state = GET;

  /* Initialize pins */
  // LEDS
  PIOC -> PIO_OER = (1 << LED1) | (1 << LED2) | (1 << LED3);
  REG_PIOC_CODR |= (0x01 << 3); // Turn off the LED using the CODR register

  /* Initialize encoder stuff */
  // Setup Quadrature Encoder with index
  REG_PMC_PCER0 = (1 << 27); // activate clock for TC0
  // select XC0 as clock source and set capture mode
  //REG_TC0_CMR0 = 5; // continous count
  // or
  REG_TC0_CMR0 = (1 << 0) | (1 << 2) | (1 << 8) | (1 << 10); // reset counter on index
  // activate quadrature encoder and position measure mode, no filters
  REG_TC0_BMR = (1 << 8) | (1 << 9) | (1 << 12);
  // enable the clock (CLKEN=1) and reset the counter (SWTRG=1)
  REG_TC0_CCR0 = 5;

  // Enable qdec interrupts for index (setting the first bit high)
  REG_TC0_QIER = TC_QIER_IDX;
  // Enable interrupts on timer1
  NVIC_EnableIRQ(TC0_IRQn);

  /* Initialize libraries */
  Ethernet.begin(MAC, IP);
  Udp.begin(PORT);
  Serial.begin(115200);
}

void loop(){
  state_machine();
  //delay(DELAY_MS);
}

void state_machine(){
  switch (state) {

    case GET:
      read_data();
      state++;
    break;

    case STREAM:
      send_osc();
      send_socket();
      state = 0;
    break;

    case SETUP:
      state = 0;
    break;

    default:
    // something went wrong and state machine is set to invalid state
    state = GET;
    break;
  }
}

void read_data(){
  pos = REG_TC0_CV0;
  if (pos < 0) { pos += encoder_quad; }

}

void send_osc(){
  // a bit dirty

  float angle = encoder_angle_step * pos;
  OSCMessage msg("/rev/rot");

  msg.add(angle).add(pos);

  Udp.beginPacket(TARGET_IP, PORT);
  msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message

}

void send_socket(){
  // nothing yet, for websockets
}


void TC0_Handler(){

  noInterrupts();
  if(REG_TC0_QISR & 0x01){
    REG_PIOC_SODR |= (0x01 << LED1); // PIO_SODR set pin high
  }
  interrupts();
}

void reset_home(){

  noInterrupts();

  // get current position

  // delay??

  // if button is no longer pressed reset home position to current

  // if the button is still pressed after a while enter setup mode

  interrupts();

}
