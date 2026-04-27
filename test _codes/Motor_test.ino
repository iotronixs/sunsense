
/**
 * @file Motor_test.ino
 * @brief Arduino sketch for testing a motor connected to pin 5.
 * 
 * This program demonstrates basic motor control by turning the motor on and off
 * in a loop, followed by a pulse test. It uses the Arduino framework to control
 * a digital output pin connected to a motor. The motor is activated for 2 seconds,
 * deactivated for 2 seconds, and then pulsed for 200 milliseconds before repeating.
 * Serial output is used to log the motor states for debugging purposes.
 * 
 * @author [@jpg.py]
 * @date [2026-27-04]
 * @version 1.0
 * 
 * @note This code is intended for educational or testing purposes. Ensure the motor
 * is properly connected to avoid damage. The motor pin is defined as 5, but can be
 * adjusted as needed.
 * 
 * @hardware_requirements
 * - Esp32-c3 supermini 
 * - Motor connected to digital pin 5
 * - Serial monitor for output (baud rate 115200)
 * 
 * @dependencies
 * - Arduino.h (included via Arduino IDE)
 */
#include <Arduino.h>

#define MOTOR 5

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR, OUTPUT);
}

void loop() {

  Serial.println("Motor ON");
  digitalWrite(MOTOR, HIGH);
  delay(2000);

  Serial.println("Motor OFF");
  digitalWrite(MOTOR, LOW);
  delay(2000);

  // ===== PULSE TEST =====
  //change delay accordingly to test different pulse durations
  Serial.println("Pulse");
  digitalWrite(MOTOR, HIGH);
  delay(200);
  digitalWrite(MOTOR, LOW);
  delay(2000);
}