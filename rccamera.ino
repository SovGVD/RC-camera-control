/*
sovgvd@gmail.com
(copy-past from BaronPilot and MultiWii)

D2 - RC input
D3 - n-fet output
D13 - LED

example for panasonic cameras
http://pp.vk.me/c424228/v424228196/26ba/Kbwjld7OEbA.jpg

*/

#define SERIAL_COM_SPEED 115200
int rawIn;
static uint32_t rcTime;
static uint32_t currentTime;
  
  #define LEDPIN_PINMODE             pinMode (13, OUTPUT);
  #define LEDPIN_OFF                 PORTB &= ~(1<<5);
  #define LEDPIN_ON                  PORTB |= (1<<5);

  #define SHUTTER_PINMODE            pinMode (3, OUTPUT);
  #define SHUTTER_OFF                PORTD &= ~(1<<3);
  #define SHUTTER_ON                 PORTD |= (1<<3);


byte newbit,oldbit,changed;
unsigned long startIn;
unsigned long time;
int blockedRX=0;

#define MASKPCINT0 (1<<2)
ISR(PCINT2_vect)
{
  time=micros(); 
  newbit=PIND;
  changed=newbit^oldbit;
  if (changed&MASKPCINT0)
    if (newbit&MASKPCINT0) startIn=time;
    else rawIn=time-startIn;
  oldbit=newbit;
  blockedRX=0;
}


void setup() {
  //Serial.begin(SERIAL_COM_SPEED);
  LEDPIN_PINMODE
  pinMode(2, INPUT);
  PCICR |= (1 << PCIE2);
  PCMSK2 = (1 << PCINT18);
  rawIn=1499;
  delay(200);
}

void loop () {
  if (currentTime > (rcTime + 20000) ) { // 50Hz
    //Serial.println(rawIn);
    if (rawIn>1500) {
      LEDPIN_ON
      SHUTTER_ON
    } else {
      LEDPIN_OFF
      SHUTTER_OFF
    }
    rcTime = currentTime; 
  }
  currentTime = micros();
}
