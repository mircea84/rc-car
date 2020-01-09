//include the RadioHead ASK library
//http://www.airspayce.com/mikem/arduino/RadioHead/
#include <RH_ASK.h>

//#define DEBUG

#ifdef DEBUG
//RH_ASK driver(2000, 11, 10, -1); //(RX / TX) Arduino Mega
#endif

//if DEBUG defined (Arduino Mega is used), comment below line
RH_ASK driver(2000, 4, 3, -1); // (RX / TX) ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85),

#ifdef DEBUG
const byte ledPin =  3;  // led used for debugging
String message = "";
#endif
const byte lightsPin = 0;     // car lights led pin
// Declare enable and phase pins. Details in links below:
// https://www.pololu.com/product/2990
// http://www.ti.com/lit/ds/slvsba4e/slvsba4e.pdf
const int enb = 1; // enable pin
const int pha = 2; // phase pin

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

#ifdef DEBUG
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
#endif

  pinMode(lightsPin, OUTPUT);
  pinMode(pha, OUTPUT);
  pinMode(enb, OUTPUT);
}

#ifdef DEBUG
String parseMsg(unsigned char* msg, uint8_t msgLen) {
  String fullMsg = "";
  for (int i = 0; i < msgLen; i++) {
    fullMsg += (char)msg[i];
  }
  return fullMsg;
}
#endif

void loop()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) // Non-blocking
  {

#ifdef DEBUG
    int i;
    Message with a good checksum received, dump it.
    driver.printBuffer("Got:", buf, 10);
    Serial.println((char*)buf);
    Serial.println(buflen);
    Serial.println(comp);
    message = parseMsg(buf, buflen);
    Serial.println(message);
    Serial.println( strncmp(buf, "1", 1) );
#endif

    //lights
    if ( strncmp(buf, "1", 1) == 0 ) {
      digitalWrite(lightsPin , !digitalRead(lightsPin));
#ifdef DEBUG
      Serial.println("1");
#endif
    } else {
      //forward
      if ( strncmp(buf, "2", 1) == 0 ) {
        digitalWrite(pha, HIGH);
        analogWrite(enb, 255);
#ifdef DEBUG
        Serial.println("2");
#endif
      } else {
        //backward
        if ( strncmp(buf, "3", 1) == 0 ) {
          digitalWrite(pha, LOW);
          analogWrite(enb, 255);
#ifdef DEBUG
          Serial.println("3");
#endif
        } else {
          //stop
          if ( strncmp(buf, "0", 1) == 0 ) {
            analogWrite(enb, 0);
#ifdef DEBUG
            Serial.println("0");
#endif
          }
        }
      }
    }
  }

}
