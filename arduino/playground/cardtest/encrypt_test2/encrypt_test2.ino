#include <Crypto.h>
#include <AES.h>
#include <string.h>
#include "base64.hpp"
#include "EasyMFRC522.h"

// Heslo k zašifrování
char pass[] = "UltraSuperSecret69_!";

AESTiny128 aes;

byte aes_key[] = { 0x34, 0x34, 0x32, 0x38, 0x34, 0x37, 0x32, 0x42, 0x34, 0x42, 0x36, 0x32, 0x35, 0x30, 0x36, 0x35 };
byte aes_iv[] = { 0x77, 0x77, 0x77, 0x2e, 0x63, 0x68, 0x65, 0x73, 0x74, 0x65, 0x72, 0x73, 0x2e, 0x63, 0x7a, 0x21 };

#define BLOCK 1
uint8_t encPass[255];
unsigned char pass64[512];
unsigned int b64Len;

char stringBuffer[512];

void setup() {
  Serial.begin(9600);

  if (aes.setKey((int)aes_key, sizeof(aes_key))) {
    Serial.println("Setting key succeeded");
  } else {
    Serial.println("An error occured");
  }

  Serial.flush();
  aes.encryptBlock(encPass, pass);
  Serial.println("encryption success");

  b64Len = encode_base64(encPass, strlen((char*) encPass), pass64);
  pass64[b64Len + 1] = '\0';
  String s = pass64;

  Serial.print("Encryption complete (");
  Serial.print(b64Len);
  Serial.print("B): ");
  Serial.println(s);

}

void loop() {
  Serial.println();
  Serial.println("READY TO WRITE");

  EasyMFRC522 rfid(10, 5);
  rfid.init();

  bool success = false;
  do {
    // returns true if a Mifare tag is detected
    success = rfid.detectTag();
    delay(50); //0.05s
  } while (!success);

  Serial.println("TAG DETECTED!");

  strcpy(stringBuffer, pass64);

  if (rfid.writeFile(BLOCK, "Pass", (byte*)stringBuffer, strlen(stringBuffer) + 1) >= 0) {
    Serial.print("WRITING SUCCESSFUL");
  } else {
    Serial.print("WRITING FAILED");
    Serial.print(rfid.writeFile(BLOCK, "Pass", (byte*)stringBuffer, strlen(stringBuffer) + 1));
  }

  delay(500);

}
