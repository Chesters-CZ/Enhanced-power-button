#include <cww_MorseTx.h>

#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...

TMRpcm tmrpcm;

const int commsOutPin = 7;
const int commsInPin = 8;
int totalTracks = -1;
byte commsIn = 0;


void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Audio start");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);

  digitalWrite(LED_BUILTIN, LOW);
  delay(250);

  pinMode(commsOutPin, OUTPUT);
  pinMode(commsInPin, INPUT_PULLUP);

  

  if (!SD.begin(SD_ChipSelectPin)){
        Serial.println("SD could not be initialized.");
    cww_MorseTx morse(0, 10, 9, 1000);
    morse.send("NO SD");
    while (true){
    }
  }
  
  File root = SD.open("/");
  File currentFile;

  Serial.println(currentFile.name());
  while (true) {
    currentFile = root.openNextFile();
    String fileName = currentFile.name();
    Serial.println(fileName);
    fileName.toUpperCase();
    if (! currentFile) {
      break;
    } else if (currentFile.isDirectory()) {
      continue;
    } else if(!fileName.equals("BOOT.WAV")) {
      totalTracks++;
    }
    currentFile.close();
  }


  Serial.print(totalTracks);
  Serial.println(" tracks found");


  if (totalTracks < 0) {
    Serial.println("Not enough tracks, cannot continue.");
    cww_MorseTx morse(0, 10, 9, 1000);
    morse.send("NO WAV");
    while (true) {
    }
  }

  tmrpcm.speakerPin = 9;
  tmrpcm.setVolume(5);

  randomSeed(analogRead(A0));
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
