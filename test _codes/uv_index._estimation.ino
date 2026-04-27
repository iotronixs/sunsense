
/**
 * @file uv_index_estimation.ino
 * @brief UV Index Estimation Program for ESP32-C3 Supermini
 * 
 * This Arduino sketch estimates the UV index using an analog UV sensor connected to pin 0 (UV_PIN).
 * It performs baseline calibration during setup by averaging 50 analog readings over 1 second (20ms delays).
 * In the main loop, it reads 20 analog values, converts them to voltage, subtracts the baseline, and calculates
 * the UV index with a multiplier of 15.0, capping it at 11. The result is printed to the serial monitor every second.
 * 
 * Hardware Configuration:
 * - Board: ESP32-C3 Supermini
 * - UV Sensor: Connected to GPIO pin 0 (analog input)
 * - ADC Attenuation: Set to ADC_11db for a range of 0-3.3V
 * - Serial Baud Rate: 115200
 * 
 * @author @jpg.py
 * @date 2026-27-04
 */
#include <Arduino.h>

#define UV_PIN 0

float baseline = 0;

float readUV() {
  float total = 0;
  for (int i = 0; i < 20; i++) {
    total += analogRead(UV_PIN);
    delay(1);
  }

  float voltage = (total / 20) * (3.3 / 4095.0);
  float adjusted = voltage - baseline;
  if (adjusted < 0) adjusted = 0;

  float uv = adjusted * 15.0;
  if (uv > 11) uv = 11;

  return uv;
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);

  float total = 0;
  for (int i = 0; i < 50; i++) {
    total += analogRead(UV_PIN);
    delay(20);
  }

  baseline = (total / 50) * (3.3 / 4095.0);
}

void loop() {
  float uv = readUV();

  Serial.print("UV Index: ");
  Serial.println(uv);

  delay(1000);
}