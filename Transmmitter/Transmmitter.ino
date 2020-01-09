//include the RadioHead ASK library
//http://www.airspayce.com/mikem/arduino/RadioHead/
#include <RH_ASK.h>

//#define DEBUG

RH_ASK driver(2000, 3, 4, -1); //(RX / TX)
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2

const byte lightsBut = 0;     // the number of the pushbutton pin   //msg=1
const byte forwardBut = 2;     // the number of the pushbutton pin  //msg=2
const byte backwardBut = 1;     // the number of the pushbutton pin //msg=3

byte buttonFlag = 0;         // variable for reading the pushbutton status
byte forwardState = 0;
byte backwardState = 0;
byte lastforwardState = 0;
byte lastbackwardState = 0;
const char *msg;


void setup()
{
#ifdef DEBUG
  Serial.begin(9600);	  // Debugging only
  Serial.println("start");
#endif
  if (!driver.init())
#ifdef DEBUG
    Serial.println("init failed");
#else
    ;
#endif

  // initialize intrerrupts to be used for car lights button. To be used for ATTiny85
  GIMSK = 0b00100000;    // turns on pin change interrupts
  PCMSK = 0b00000001;    // turn on interrupts on pins PB0
  sei();                 // enables interrupts

#ifdef DEBUG
  pinMode(ledPin, OUTPUT);
#endif
  pinMode(lightsBut, INPUT);
  pinMode(forwardBut, INPUT);
  pinMode(backwardBut, INPUT);
  //intrerrupts when using Arduino
  //attachInterrupt(digitalPinToInterrupt(lightsBut), lights_ISR, RISING);  //  function for creating external interrupts at pin2 on Rising (LOW to HIGH)
}

//lights button pin 0
//void lights_ISR() // to be used for Arduino implementation
ISR(PCINT0_vect)
{
  buttonFlag = 1;
  msg = "1"; // lights
}


void sendData(const char *data) {
  driver.send((uint8_t *)msg, strlen(msg));
#ifdef DEBUG
  Serial.print("Message sent: ");
  Serial.println(msg);
#endif
  driver.waitPacketSent();
  delay(200);
  buttonFlag = 0;
}

void loop()
{
  // read the state of the pushbutton value
  forwardState = digitalRead(forwardBut);
  backwardState = digitalRead(backwardBut);
  //Serial.println(buttonState);

  //forward button pin 2
  if (forwardState != lastforwardState) {
    //Serial.println(forwardState);
    if (forwardState == HIGH) {
      buttonFlag = 1;
      msg = "2"; // forward
      //Serial.println(forwardState);
    } else {
      buttonFlag = 1;
      msg = "0"; // stop
      //Serial.println(forwardState);
    }
  }
  lastforwardState = forwardState;

  //backward button pin 1
  if (backwardState != lastbackwardState) {
    //Serial.println(backwardState);
    if (backwardState == HIGH) {
      buttonFlag = 1;
      msg = "3"; // backward
      //Serial.println(backwardState);
    } else {
      buttonFlag = 1;
      msg = "0"; // stop
      //Serial.println(backwardState);
    }
  }
  lastbackwardState = backwardState;

  if (buttonFlag) {
    sendData(msg);
  }

}
