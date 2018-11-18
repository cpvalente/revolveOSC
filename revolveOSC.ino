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

/* Initialize libraries */
EthernetUDP Udp;

/* aux */
uint32_t nowTime, lastTime;

// osc
uint32_t lastBroadcast;

static uint8_t state;   // variable for state control
enum states {GET, STREAM, SETUP};

/* periphericals */
bool buttonState1, buttonState2;
bool lastButtonState1, lastButtonState2;
uint32_t lastTimeDebounce;

/* encoder stuff */
int pos, pos2, posOffset;
bool bGotZero;

// network stuff
byte MAC[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x07, 0x74 };
IPAddress IP(10, 0, 0, 11);
IPAddress TARGET_IP(10, 0, 0, 255); // Broadcast for now
int PORT = 8888;

void setup(){

  /* Initialize aux */
  state = GET;
  bGotZero = false;
  posOffset = 0;

  // buttons default HIGH
  buttonState1 = buttonState2 = true;
  lastButtonState1 = lastButtonState2 = true;

  lastTimeDebounce = millis() + WARM_UP_TIME;

  /* Initialize pins */
  // LEDS
  PIOC -> PIO_OER = (1 << LED1) | (1 << LED2) | (1 << LED3);
  REG_PIOC_CODR |= (0x01 << 3); // Turn off the LED using the CODR register
  // Buttons
  pinMode(BTN1, INPUT); digitalWrite(BTN1, HIGH);
  pinMode(BTN2, INPUT); digitalWrite(BTN2, HIGH);

  /* Initialize encoder stuff */
  // Setup Quadrature Encoder with index
  REG_PMC_PCER0 = (1 << 27); // activate clock for TC0
  // select XC0 as clock source and set capture mode
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

  if (millis() - lastTimeDebounce >= DEBOUNCE_TIME){
    check_buttons();
    lastTimeDebounce = millis();
  }

  state_machine();
  //delay(DELAY_MS);
}

void check_buttons(){
  // buttons are LOW when pressed
  buttonState1 = digitalRead(BTN1);
  buttonState2 = digitalRead(BTN2);

  // first button sets 0 position
  if (!buttonState1) {
    // set flag
    if (!lastButtonState1) {
      lastButtonState1 = false;
    } else {
      // on second go set home
      set_home();
      // reset flag
      lastButtonState1 = true;
    }
  }

  // second button does nothing yet
  //if (!buttonstate2) {
    // nothing yet
  //}

}
void state_machine(){
  switch (state) {

    case GET:
      read_data();
      state++;
      // always stream for now
      // break;
    case STREAM:
      send_osc();
      send_socket();
      state = GET;
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

  pos2 = pos + posOffset;
  if (pos2 < 0) { pos2 += encoder_quad; }
}

void send_osc(){
  // a bit dirty

  float angle = encoder_angle_step * pos;
  float angle2 = encoder_angle_step * pos2;

  OSCMessage msg("/rev/rot");
  msg.add(angle).add(angle2).add(pos).add(pos2).add(posOffset).add(bGotZero);

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
    bGotZero = true;  // set flag
  }

  interrupts();
}

void set_home(){

  noInterrupts();
  // get current position
  read_data();

  if (!buttonState2) {
    posOffset = 0;
  } else {
    posOffset = 0 - pos;
  }
  interrupts();

}
