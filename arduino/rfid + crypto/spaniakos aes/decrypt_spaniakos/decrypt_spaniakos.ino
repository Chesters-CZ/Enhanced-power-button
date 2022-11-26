#include <AES.h>
#include <AES_config.h>
#include <printf.h>
#include "base64.hpp"
#include "EasyMFRC522.h"


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
}

void loop() {
  prectiADesifruj(128);
}

void prectiADesifruj (int bits)
{
  aes.iv_inc();
  byte iv [N_BLOCK] ;
  byte plain_p[padedLength];
  byte cipher [padedLength] ;
  byte check [padedLength] ;
  /*
    unsigned long ms = micros ();
    aes.set_IV(my_iv);
    aes.get_IV(iv);
    aes.do_aes_encrypt(plain, plainLength, cipher, key, bits, iv);
    Serial.print("Encryption took: ");
    Serial.println(micros() - ms);
  */

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

  if (rfid.readFile(1, "Pass", (byte*)pass64, 64) >= 0) {
    Serial.print("READING SUCCESSFUL ");
    String s = pass64;
    Serial.print(s);
  } else {
    Serial.print("READING FAILED");
  }
decode_base64(pass64, cipher);
  rfid.unselectMifareTag();
  delay(3000);


  unsigned long   ms = micros ();
  aes.set_IV(my_iv);
  aes.get_IV(iv);
  aes.do_aes_decrypt(cipher, padedLength, check, key, bits, iv);
  Serial.print("Decryption took: ");
  Serial.println(micros() - ms);
  printf("\nCIPHER:");
  aes.printArray(cipher, (bool)false);
  printf("\nCHECK :");
  aes.printArray(check, (bool)true);
  printf("\nIV    :");
  aes.printArray(iv, 16);
  printf("\n============================================================\n");
}
