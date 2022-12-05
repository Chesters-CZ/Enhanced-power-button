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

  randomSeed(analogRead(A0));

  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  tmrpcm.setVolume(5);
}

void loop() {
  while (true) {
    digitalWrite(commsOutPin, HIGH);
    commsIn = digitalRead(commsInPin);
    // pcswitch = 0 if pushed, 1 if released

    if (commsIn == 0) {
      digitalWrite(commsOutPin, LOW);
      Serial.println(F("Received signal"));
      tmrpcm.stopPlayback();
      if (SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
        switch (random(45)) {
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
          case 17:
            tmrpcm.play("startu18.wav");
            Serial.println(F("Played track 18"));
            break;
          case 18:
            tmrpcm.play("startu19.wav");
            Serial.println(F("Played track 19"));
            break;
          case 19:
            tmrpcm.play("startu20.wav");
            Serial.println(F("Played track 20"));
            break;
          case 20:
            tmrpcm.play("startu21.wav");
            Serial.println(F("Played track 21"));
            break;
          case 21:
            tmrpcm.play("startu22.wav");
            Serial.println(F("Played track 22"));
            break;
          case 22:
            tmrpcm.play("startu23.wav");
            Serial.println(F("Played track 23"));
            break;
          case 23:
            tmrpcm.play("startu24.wav");
            Serial.println(F("Played track 24"));
            break;
          case 24:
            tmrpcm.play("startu25.wav");
            Serial.println(F("Played track 25"));
            break;
          case 25:
            tmrpcm.play("startu26.wav");
            Serial.println(F("Played track 26"));
            break;
          case 26:
            tmrpcm.play("startu27.wav");
            Serial.println(F("Played track 27"));
            break;
          case 27:
            tmrpcm.play("startu28.wav");
            Serial.println(F("Played track 28"));
            break;
          case 28:
            tmrpcm.play("startu29.wav");
            Serial.println(F("Played track 29"));
            break;
          case 29:
            tmrpcm.play("startu30.wav");
            Serial.println(F("Played track 30"));
            break;
          case 30:
            tmrpcm.play("startu31.wav");
            Serial.println(F("Played track 31"));
            break;
          case 31:
            tmrpcm.play("startu32.wav");
            Serial.println(F("Played track 32"));
            break;
          case 32:
            tmrpcm.play("startu33.wav");
            Serial.println(F("Played track 33"));
            break;
          case 33:
            tmrpcm.play("startu34.wav");
            Serial.println(F("Played track 34"));
            break;
          case 34:
            tmrpcm.play("startu35.wav");
            Serial.println(F("Played track 35"));
            break;
          case 35:
            tmrpcm.play("startu36.wav");
            Serial.println(F("Played track 36"));
            break;
          case 36:
            tmrpcm.play("startu37.wav");
            Serial.println(F("Played track 37"));
            break;
          case 37:
            tmrpcm.play("startu38.wav");
            Serial.println(F("Played track 38"));
            break;
          case 38:
            tmrpcm.play("startu39.wav");
            Serial.println(F("Played track 39"));
            break;
          case 39:
            tmrpcm.play("startu40.wav");
            Serial.println(F("Played track 40"));
            break;
          case 40:
            tmrpcm.play("startu141.wav");
            Serial.println(F("Played track 41"));
            break;
          case 41:
            tmrpcm.play("startu42.wav");
            Serial.println(F("Played track 42"));
            break;
          case 42:
            tmrpcm.play("startu43.wav");
            Serial.println(F("Played track 43"));
            break;
          case 43:
            tmrpcm.play("startu44.wav");
            Serial.println(F("Played track 44"));
            break;
          case 44:
            tmrpcm.play("startu45.wav");
            Serial.println(F("Played track 45"));
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
