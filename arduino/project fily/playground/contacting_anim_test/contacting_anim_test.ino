
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define SERIAL_BUFFER_SIZE 16

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char nano [] PROGMEM = {
  // 'NANO, 11x18px
  0x1f, 0x00, 0xf1, 0xe0, 0x8e, 0x20, 0xa0, 0xa0, 0x80, 0x20, 0xa0, 0xa0, 0x80, 0x20, 0xa0, 0xa0,
  0x80, 0x20, 0xa0, 0xa0, 0x80, 0x20, 0xa0, 0xa0, 0x8a, 0x20, 0xa0, 0xa0, 0x8e, 0x20, 0xae, 0xa0,
  0x80, 0x20, 0xff, 0xe0
};

const unsigned char uno [] PROGMEM = {
  // 'UNO, 15x17px
  0x38, 0x78, 0xef, 0xce, 0x80, 0x02, 0xa0, 0x0a, 0xa0, 0x0a, 0xaa, 0xaa, 0xa0, 0x0a, 0xa0, 0x0a,
  0xa0, 0x0a, 0xa0, 0x0a, 0xa0, 0x0a, 0xa0, 0x0a, 0xa0, 0x0a, 0xa3, 0x8a, 0xa3, 0x8a, 0x80, 0x02,
  0xff, 0xfe
};


byte frame = 0;


void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
}

void loop() {
  frame = 89;
  drawUnoAndNano();
  // digitalWrite(commsOutPin, HIGH);
  while (true) {
    if (frame % 89 == 0) {
      frame = frame - 89;
      drawUnoAndNano();
    }
    display.drawPixel((frame % 89) + 18, 13, BLACK);
    display.drawPixel((frame % 89) + 19, 13, WHITE);
    display.drawPixel((frame % 89) + 20, 13, WHITE);
    display.drawPixel((frame % 89) + 21, 13, WHITE);
    display.display();
delay(100);
    frame++;
    // commsIn = digitalRead(commsInPin);
  }
}

void drawUnoAndNano() {
  display.clearDisplay();
  display.drawBitmap(7, 4, nano, 11, 18, WHITE);
  display.drawBitmap(110, 4, nano, 11, 18, WHITE);
  display.setTextSize(1);
  display.setCursor(18, 24);
  display.println(F("contacting nano"));
  display.drawPixel(108, 30, WHITE);
  display.drawPixel(110, 30, WHITE);
  display.drawPixel(112, 30, WHITE);
  display.display();
}
