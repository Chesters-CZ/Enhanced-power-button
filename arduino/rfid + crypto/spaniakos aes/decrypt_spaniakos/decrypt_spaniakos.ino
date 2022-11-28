#include <AES.h>
#include <AES_config.h>
#include <printf.h>
#include "base64.hpp"
#include "EasyMFRC522.h"
#include <Keyboard.h>


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
  printf_begin();
  delay(500);
  rfid.init();
  Keyboard.begin();
}

void loop() {
  prectiADesifruj(128);
}

void prectiADesifruj (int bits)
{
  aes.iv_inc();
  byte iv [N_BLOCK] ;
  byte cipher [padedLength] ;
  byte check [padedLength] ;

  char pass64[64] = {0};

  Serial.println();
  Serial.println(F("READY TO READ"));

  bool success = false;
  do {
    // returns true if a Mifare tag is detected
    success = rfid.detectTag();
    delay(50); //0.05s
  } while (!success);


  Serial.println(F("TAG DETECTED!"));
  Serial.print(F("READING PASSWORD "));
  if (rfid.readFile(1, "Pass", (byte*)pass64, 64) >= 0) {
    Serial.print(F("SUCCESS"));
    String s = pass64;
    Serial.println(s);

    decode_base64(pass64, cipher);

    aes.set_IV(my_iv);
    aes.get_IV(iv);
    aes.do_aes_decrypt(cipher, padedLength, check, key, bits, iv);

    String result = check;
    Serial.println(result);
    Keyboard.print(result);
  } else {
    Serial.print("READING FAILED");
  }

  rfid.unselectMifareTag();
  delay(3000);
}
