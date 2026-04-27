
/**
 * @file Uv_callibration.ino
 * @brief UV Sensor Calibration and Reading Program for ESP32-C3 SuperMini
 * 
 * This Arduino sketch is designed to calibrate and read UV sensor data using an ESP32-C3 SuperMini board.
 * It performs an initial calibration to establish a baseline voltage reading from the UV sensor connected to analog pin 0.
 * In the main loop, it reads the raw analog value, converts it to voltage, subtracts the baseline, and ensures non-negative values.
 * The adjusted voltage is then printed to the serial monitor for monitoring UV intensity.
 * 
 * Hardware Configuration:
 * - Board: ESP32-C3 SuperMini
 * - UV Sensor: Connected to analog pin 0 (UV_PIN defined as 0) GYML8511
 * - ADC Attenuation: Set to ADC_11db for a full-scale voltage range of approximately 0-3.3V
 * - Serial Communication: Baud rate 115200 for debugging output
 * 
 * Calibration Process:
 * - Takes 50 analog readings with 20ms delays between each.
 * - Computes the average raw value and converts it to voltage using the formula: voltage = raw * (3.3 / 4095.0)
 * - Stores this as the baseline for subtraction in readings.
 * 
 * Loop Operation:
 * - Reads analog value from UV_PIN.
 * - Converts to voltage.
 * - Subtracts baseline and clamps to zero if negative.
 * - Prints the adjusted voltage to Serial every second.
 * 
 * @author [@jpg.py]
 * @date [2026-27-04]
 * 
 * Dependencies:
 * - Arduino.h (for ESP32 Arduino framework)
 * 
 * Notes:
 * - Ensure the UV sensor is properly connected and powered.
 * - Calibration assumes stable environmental conditions.
 * - Adjust delay values if needed for different sampling rates.
 * connect UV sensor to 3.3v and GND, and the output to pin 0 (UV_PIN) for accurate readings. dont connect to 5v as it may damage the sensor or the board.
 */
#include <Arduino.h>

#define UV_PIN 0

float baseline = 0;

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);

  Serial.println("Calibrating...");

  float total = 0;
  for (int i = 0; i < 50; i++) {
    total += analogRead(UV_PIN);
    delay(20);
  }

  baseline = (total / 50) * (3.3 / 4095.0);

  Serial.print("Baseline: ");
  Serial.println(baseline);
}

void loop() {
  int raw = analogRead(UV_PIN);
  float voltage = raw * (3.3 / 4095.0);

  float adjusted = voltage - baseline;
  if (adjusted < 0) adjusted = 0;

  Serial.print("Adjusted: ");
  Serial.println(adjusted);

  delay(1000);
}