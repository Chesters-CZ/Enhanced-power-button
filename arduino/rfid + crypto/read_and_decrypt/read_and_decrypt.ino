#include <Crypto.h>
#include <AES.h>
#include <string.h>
#include "base64.hpp"
#include "EasyMFRC522.h"

#define BLOCK 1

byte aes_key[] = { 0x34, 0x34, 0x32, 0x38, 0x34, 0x37, 0x32, 0x42, 0x34, 0x42, 0x36, 0x32, 0x35, 0x30, 0x36, 0x35 };

AESSmall128 aes;
EasyMFRC522 rfid(10, 5);

char cardContent[512];
unsigned char decoded;
uint8_t encPass[255];
char pass[255];

void setup() {
  Serial.begin(9600);
  rfid.init();
}

void loop() {
  Serial.println();
  Serial.println("READY TO READ");

  bool success = false;
  do {
    // returns true if a Mifare tag is detected
    success = rfid.detectTag();
    delay(50); //0.05s
  } while (!success);

  Serial.println("TAG DETECTED!");

  if (rfid.readFile(BLOCK, "Pass", (byte*)cardContent, 512) >= 0) {
    Serial.print("READING SUCCESSFUL ");
    String s = cardContent;
    Serial.print(s);
  } else {
    Serial.print("READING FAILED");
  }

  Serial.println();


  decoded = base64_to_binary(cardContent);

memcpy(encPass, decoded, sizeof(decoded));

  if (aes.setKey((int)aes_key, sizeof(aes_key))) {
    Serial.println("Setting key succeeded");
  } else {
    Serial.println("An error occured");
  }

  aes.decryptBlock(pass, encPass);

  Serial.print("Password decoded: ");
  String s = pass;
  Serial.println(s);
}
