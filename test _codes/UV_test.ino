

/**
 * @file UV_test.ino
 * @brief This Arduino sketch reads analog values from a UV sensor connected to pin 0 on an ESP32-C3 SuperMini board.
 *        It initializes serial communication at 115200 baud and sets the ADC attenuation to 11dB for better sensitivity.
 *        In the main loop, it continuously reads the raw analog value, prints it to the serial monitor, and waits 500ms before the next reading.
 *        This is a basic test code for UV sensor functionality, useful for debugging and calibration purposes.
 * @author @jpg.py
 * @date 2026-27-04
 * @board ESP32-C3 SuperMini
 * @config ADC attenuation: 11dB (analogSetAttenuation(ADC_11db))
 * @pin UV_PIN: 0 (analog input for UV sensor)
 */
#include <Arduino.h>

#define UV_PIN 0

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  int raw = analogRead(UV_PIN);

  Serial.print("RAW: ");
  Serial.println(raw);

  delay(500);
}