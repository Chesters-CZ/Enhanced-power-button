#include <Crypto.h>
#include <AES.h>
#include <string.h>
#include "base64.hpp"
#include "EasyMFRC522.h"

// Heslo k zašifrování
const char   pass[] = "UltraSuperSecret69_!";

EasyMFRC522 mfr(10, 5);

AESSmall128 aes;

const byte   aes_key[] = { 0x34, 0x34, 0x32, 0x38, 0x34, 0x37, 0x32, 0x42, 0x34, 0x42, 0x36, 0x32, 0x35, 0x30, 0x36, 0x35 };

#define BLOCK 1

uint8_t encPass[32] = {0};
unsigned char pass64[255];
unsigned int b64Len;

char stringBuffer[255];

void setup() {
  Serial.begin(9600);

  while (!Serial)
    ;

  if (aes.setKey(aes_key, sizeof(aes_key))) {
    Serial.println(F("Setting key succeeded"));
  } else {
    Serial.println(F("An error occured"));
  }

  Serial.flush();
  aes.encryptBlock(encPass, pass);
  Serial.println(F("encryption over"));
  String s = encPass;
  Serial.println(s);

  b64Len = encode_base64(encPass, sizeof(encPass), pass64);

  s = pass64;

  Serial.print(F("Encryption complete ("));
  Serial.print(b64Len);
  Serial.print(F("B): "));
  Serial.println(s);

  mfr.init();

  display_freeram();
}

void loop() {
  Serial.println();
  Serial.println(F("READY TO WRITE"));

  bool success = false;
  do {
    // returns true if a Mifare tag is detected
    success = mfr.detectTag();
    delay(50); //0.05s
  } while (!success);

  Serial.println(F("TAG DETECTED!"));

  strcpy(stringBuffer, pass64);

  if (mfr.writeFile(BLOCK, "Pass", (byte*)stringBuffer, strlen(stringBuffer) + 1) >= 0) {
    Serial.print(F("WRITING SUCCESSFUL"));
  } else {
    Serial.print(F("WRITING FAILED"));
    Serial.print(mfr.writeFile(BLOCK, "Pass", (byte*)stringBuffer, strlen(stringBuffer) + 1));
  }

  mfr.unselectMifareTag();

  delay(2000);
}

void display_freeram() {
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
}

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0
                    ? (int)&__heap_start : (int) __brkval);
}
