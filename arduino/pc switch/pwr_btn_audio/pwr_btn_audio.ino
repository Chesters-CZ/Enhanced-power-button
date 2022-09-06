#include <pcmRF.h>
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>
#include <SD.h>
#include <math.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

const int commsOutPin = 7;
const int commsInPin = 8;
byte commsIn = 0;

void setup() {
  pinMode(commsOutPin, OUTPUT);
  pinMode(commsInPin, INPUT_PULLUP);
  Serial.begin(9600);

  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
}

void loop() {
  while (true) {
  digitalWrite(commsOutPin, HIGH);
    commsIn = digitalRead(commsInPin);
    // pcswitch = 0 if pushed, 1 if released

    if (commsIn == 0) {
      digitalWrite(commsOutPin, LOW);
      Serial.println(F("starting sd card ops"));
      tmrpcm.stopPlayback();
      if (SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
        tmrpcm.setVolume(5);
        switch (round(random(17) - 1)) {
          case 0:
            tmrpcm.play("startup.wav");
            Serial.println(F("Played track 1"));
            break;
          case 1:
            tmrpcm.play("startup2.wav");
            Serial.println(F("Played track 2"));
            break;
          case 2:
            tmrpcm.play("startup3.wav");
            Serial.println(F("Played track 3"));
            break;
          case 3:
            tmrpcm.play("startup4.wav");
            Serial.println(F("Played track 4"));
            break;
          case 4:
            tmrpcm.play("startup5.wav");
            Serial.println(F("Played track 5"));
            break;
          case 5:
            tmrpcm.play("startup6.wav");
            Serial.println(F("Played track 6"));
            break;
          case 6:
            tmrpcm.play("startup7.wav");
            Serial.println(F("Played track 7"));
            break;
          case 7:
            tmrpcm.play("startup8.wav");
            Serial.println(F("Played track 8"));
            break;
          case 8:
            tmrpcm.play("startup9.wav");
            Serial.println(F("Played track 9"));
            break;
          case 9:
            tmrpcm.play("startu10.wav");
            Serial.println(F("Played track 10"));
            break;
          case 10:
            tmrpcm.play("startu11.wav");
            Serial.println(F("Played track 11"));
            break;
          case 11:
            tmrpcm.play("startu12.wav");
            Serial.println(F("Played track 12"));
            break;
          case 12:
            tmrpcm.play("startu13.wav");
            Serial.println(F("Played track 13"));
            break;
          case 13:
            tmrpcm.play("startu14.wav");
            Serial.println(F("Played track 14"));
            break;
          case 14:
            tmrpcm.play("startu15.wav");
            Serial.println(F("Played track 15"));
            break;
          case 15:
            tmrpcm.play("startu16.wav");
            Serial.println(F("Played track 16"));
            break;
          case 16:
            tmrpcm.play("startu17.wav");
            Serial.println(F("Played track 17"));
            break;
          default:
            tmrpcm.play("startup2.wav");
            Serial.println(F("Played default"));
        }
      } else {
        Serial.println("SD failed");
      }
      while (commsIn == 0) {
        commsIn = digitalRead(commsInPin);
      }
      delay(250);


    }
  }
}
