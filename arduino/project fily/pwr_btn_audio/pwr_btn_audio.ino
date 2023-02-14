#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...

TMRpcm tmrpcm;

const int commsOutPin = 7;
const int commsInPin = 8;
const int totalTracks = 69 + 1;
byte commsIn = 0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  Serial.begin(9600);
  
  pinMode(commsOutPin, OUTPUT);
  pinMode(commsInPin, INPUT_PULLUP);

  tmrpcm.speakerPin = 9;
  tmrpcm.setVolume(5);

  randomSeed(analogRead(A0));

  SD.begin(SD_ChipSelectPin);
  Serial.println("Audio start");
  tmrpcm.play("BOOT.WAV");
}

void loop() {
  digitalWrite(commsOutPin, HIGH);
  commsIn = digitalRead(commsInPin);
  // pcswitch = 0 if pushed, 1 if released



  if (commsIn == 0) {
    digitalWrite(commsOutPin, LOW);
    Serial.println(F("Received signal"));
    tmrpcm.stopPlayback();

    int rng = random(totalTracks);
    Serial.print("Playing ");
    String fileName = "STARTU" + zeropadDD(rng) + ".WAV";
    Serial.println(fileName);
    char fileNameAsChar[13];
    fileName.toCharArray(fileNameAsChar, 13);
    tmrpcm.play(fileNameAsChar);


    while (commsIn == 0) {
      commsIn = digitalRead(commsInPin);
    }
    delay(250);
  }
}

String zeropadDD(int i) {
  if (i < 10) {
    return "0" + String(i);
  } else {
    return String(i);
  }
}
