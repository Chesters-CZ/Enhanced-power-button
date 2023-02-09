/* Example code
 * by Eward Hage
 */
uint8_t buf[8] = { 0 };   //Keyboard report buffer

#define PIN_W 4 // Pin for w
#define PIN_A 5 // Pin for a
#define PIN_S 6 // Pin for s
#define PIN_D 7 // Pin for d

void setup() {
  Serial.begin(9600); // Setup Serial communication

  //Set pinmode of Input pins
  pinMode(PIN_W, INPUT);
  pinMode(PIN_A, INPUT);
  pinMode(PIN_S, INPUT);
  pinMode(PIN_D, INPUT);
}

void loop() {
//When button representing W is pressed  
  if (digitalRead(PIN_W) == HIGH) { 
    buf[2] = 26;   // W keycode
    Serial.write(buf, 8); // Send keypress
   // Serial.println("W");
    releaseKey();
  }
   
//When button representing A is pressed
  if (digitalRead(PIN_A) == HIGH) {
    buf[2] = 4;   // A keycode
    Serial.write(buf, 8); // Send keypress
   // Serial.println("A");
    releaseKey();
  } 
  
//When button representing S is pressed
  if (digitalRead(PIN_S) == HIGH) {
    buf[2] = 22;   // S keycode
    Serial.write(buf, 8); // Send keypress
   // Serial.println("S");
    releaseKey();
  } 
  
//When button representing D is pressed
  if (digitalRead(PIN_D) == HIGH) {
    buf[2] = 7;   // D keycode
    Serial.write(buf, 8); // Send keypress
  //  Serial.println("D");
    releaseKey();
  } 
  
}
// Function for Key Release
void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Send Release key  
}
