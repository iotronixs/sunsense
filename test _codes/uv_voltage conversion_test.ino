

/**
 * @file uv_voltage_conversion_test.ino
 * @brief UV Voltage Conversion Test for ESP32-C3 SuperMini
 * 
 * This Arduino sketch reads analog values from a UV sensor connected to pin 0 (UV_PIN) on an ESP32-C3 SuperMini board.
 * It converts the raw ADC reading to voltage using a reference of 3.3V and a 12-bit ADC resolution (4095 max value).
 * The calculated voltage is printed to the serial monitor every 500ms.
 * 
 * Hardware Configuration:
 * - Board: ESP32-C3 SuperMini
 * - ADC Attenuation: 11dB (set for 0-3.3V input range)
 * - UV Sensor: Connected to GPIO 0 (analog input)
 * - Serial Baud Rate: 115200
 * 
 * @author @jpg.py
 * @date 2026-27-04
 */
#include <Arduino.h>

#define UV_PIN 0

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  int raw = analogRead(UV_PIN);
  float voltage = raw * (3.3 / 4095.0);

  Serial.print("Voltage: ");
  Serial.println(voltage);

  delay(500);
}