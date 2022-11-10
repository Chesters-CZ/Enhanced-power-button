
#include "AESLib.h"
#include "base64.hpp"
#include "EasyMFRC522.h"



#define MAX_STRING_SIZE 100  // size of the char array that will be written to the tag
#define BLOCK 1              // initial block, from where the data will be stored in the tag

// Key and IV replaced
byte aes_key[] = { 0x34, 0x34, 0x32, 0x38, 0x34, 0x37, 0x32, 0x42, 0x34, 0x42, 0x36, 0x32, 0x35, 0x30, 0x36, 0x35 };
byte aes_iv[] = { 0x77, 0x77, 0x77, 0x2e, 0x63, 0x68, 0x65, 0x73, 0x74, 0x65, 0x72, 0x73, 0x2e, 0x63, 0x7a, 0x21 };

// Keep password length below 50
char pass[] = "UltraSuperSecret69_!";
unsigned char encPass[] = {0};
unsigned char pass64[] = {0};

char stringBuffer[MAX_STRING_SIZE];

AESLib aes;

EasyMFRC522 rfid(10, 5);

void setup() {
  Serial.begin(9600);

  rfid.init();

  Serial.println("Encrypting...");

  aes.set_paddingmode((paddingMode)0);

  uint16_t msgLen = sizeof(pass);
  uint16_t encLen = aes.encrypt((byte*)pass, msgLen, (byte*)encPass, aes_key, sizeof(aes_key), aes_iv);

  unsigned int b64Len = encode_base64(encPass, strlen((char*) encPass), pass64);

  String s = pass64;
  Serial.print("Encryption complete : ");
  Serial.println(s);
}

void loop() {
  Serial.println();
  Serial.println("READY TO WRITE");

  bool success = false;
  do {
    // returns true if a Mifare tag is detected
    success = rfid.detectTag();
    delay(50); //0.05s
  } while (!success);

  Serial.println("TAG DETECTED!");


  // ERROR HERE
  stringBuffer[MAX_STRING_SIZE];
  strcpy(stringBuffer, pass64);

  if (rfid.writeFile(BLOCK, "Pass", (byte*)stringBuffer, strlen(stringBuffer) + 1) == 0) {
    Serial.print("WRITING SUCCESSFUL");
  } else {
        Serial.print("WRITING FAILED");
  }

  delay(500);
}
