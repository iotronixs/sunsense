#include <Arduino.h>

#define LED 8   // change if GPIO pins are used for LED

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  Serial.println("ESP Test Started");
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("LED ON");
  delay(500);

  digitalWrite(LED, LOW);
  Serial.println("LED OFF");
  delay(500);
}