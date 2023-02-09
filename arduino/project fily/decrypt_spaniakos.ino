
#include <AES.h>
#include <AES_config.h>
#include "base64.hpp"
#include "EasyMFRC522.h"
#include <CzechKeyboard_QWERTZ.h>


AES aes;

EasyMFRC522 rfid(10, 5);

// Tímhle se to dešifruje   "Sem je to AES128"sem 192"sem 256"
byte *key = (unsigned char*)"4428472B4B625065";

// Tímhle se to osolí (doopravdy se dají dvě tyhle věci za sebe ale pššt)
unsigned long long int my_iv = 22176790;

int padedLength = 32; // vždycky šifrovat do dvou segmentů


void setup() {
  Serial.begin (9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  delay(500);
  rfid.init();
  Keyboard.begin();
}

void loop() {
  String y = key;
  if (y.length() == 16) {
    prectiADesifruj(128);
  }
  else if (y.length() == 24) {
    prectiADesifruj(192);
  }
  else if (y.length() == 32) {
    prectiADesifruj(256);
  }
  else {
    Serial.print("ZVOLENA NEPLATNÁ DÉLKA KLÍČE (");
    Serial.print(sizeof(key));
    Serial.println(")");
    Serial.println("PODPOROVANÉ DÉLKY KLÍČE JSOU:");
    Serial.println("16 ZNAKŮ PRO AES128 (VYSOKÁ ÚROVEŇ ZABEZPEČENÍ)");
    Serial.println("24 ZNAKŮ PRO AES192 (VELMI VYSOKÁ ÚROVEŇ ZABEZPEČENÍ)");
    Serial.println("32 ZNAKŮ PRO AES256 (EXTRÉMNĚ VYSOKÁ ÚROVEŇ ZABEZPEČENÍ)");

    while (true) {
    }

  }
}

void prectiADesifruj (int bits)
{
  aes.iv_inc();
  byte iv [N_BLOCK] ;
  byte cipher [padedLength] ;
  byte check [padedLength] ;

  char pass64[64] = {0};
  char passLen[4] = {0};
  char logOffPref[5] = {0};

  String s;
  int block = 0;
  int newBlock;

  byte tagID[4] = {0};

  Serial.println();
  Serial.println(F("READY TO READ"));

  bool success = false;
  do {
    // returns true if a Mifare tag is detected
    success = rfid.detectTag(tagID);
    delay(50); //0.05s
  } while (!success);

  delay(100); // Ensures the chip is close enough


  Serial.println(F("TAG DETECTED!"));
  Serial.print(F("READING PASSWORD... "));
  block = rfid.readFile(1, "Pass", (byte*)pass64, 64);
  if (block >= 0) {
    Serial.println(F("SUCCESS"));
    s = pass64;
    Serial.println(s);

    Serial.print(F("READING LENGTH... "));
    block = rfid.readFile(40, "PLen", (byte*)passLen, 64);
    if (block >= 0) {
      Serial.println(F("SUCCESS"));
      s = passLen;
      Serial.println(s);

      Serial.print(F("READING LOGOFF PREFERENCE... "));
      newBlock = rfid.readFile(50, "LogOff", (byte*)logOffPref, 64);
      if (newBlock >= 0) {
        Serial.println(F("SUCCESS"));
        s = passLen;
        Serial.println(s);
      }
      else {
        Serial.print("READING FAILED");
        Serial.println(newBlock);
      }

    }
    else {
      Serial.print("READING FAILED");
      Serial.println(block);
    }

    decode_base64(pass64, cipher);

    aes.set_IV(my_iv);
    aes.get_IV(iv);
    aes.do_aes_decrypt(cipher, padedLength, check, key, bits, iv);

    s = passLen;

    Serial.println(s.toInt());
    String result = check;
    result = result.substring(0, s.toInt());
    Serial.println(result);
    Keyboard.print(result);
  } else {
    Serial.print("READING FAILED ");
    Serial.println(block);
  }

  s = logOffPref;
  if (s.charAt(0) == 't') {
    String s1;

    rfid.unselectMifareTag(true);
    delay(50);

    byte newTagID[4] = {0};
    byte badReads = 0;

    do {
      for (byte IDsegment : newTagID) {
        IDsegment = 0;
      }

      success = rfid.detectTag(newTagID);

      if (success) {
        for (byte i = 2; i < 4; i++) {
          if (tagID[i] != newTagID[i]) {
            success = false;
            break;
          }
        }

        Serial.print("Old tag ID: ");
        for (byte i = 2; i < 4; i++) {
          Serial.print(String(tagID[i]));
          Serial.print(", ");
        }
        Serial.println();

        Serial.print("New tag ID: ");
        for (byte i = 2; i < 4; i++) {
          Serial.print(String(newTagID[i]));
          Serial.print(", ");
        }
        Serial.println();
      }

      if (success) {
        Serial.println("Good read!");
        badReads = 0;
      } else {
        Serial.print("Bad read: ");
        badReads++;
        Serial.println(badReads);
      }

      rfid.unselectMifareTag(true);
      delay(50);
    } while (badReads < 10);


    Serial.print("Old tag ID: ");
    for (byte i = 2; i < 4; i++) {
      Serial.print(String(tagID[i]));
          Serial.print(", ");
    }
    Serial.println();

    Serial.print("New tag ID: ");
    for (byte i = 2; i < 4; i++) {
      Serial.print(String(newTagID[i]));
          Serial.print(", ");
    }
    Serial.println();

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('l');
    delay(25);
    Keyboard.releaseAll();
  }

  rfid.unselectMifareTag();
  delay(3000);
}
