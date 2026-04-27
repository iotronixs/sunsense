
/*
 * UV Stable Reading Test
 * 
 * This Arduino sketch is designed to read analog values from a UV sensor connected to pin 0 on an ESP32-C3 SuperMini board.
 * It calculates the average of 20 readings to stabilize the output, converts the raw ADC value to voltage, and prints the average voltage to the serial monitor.
 * The ADC attenuation is set to 11dB for a wider input range up to approximately 3.3V.
 * 
 * Hardware Configuration:
 * - Board: ESP32-C3 SuperMini
 * - UV Sensor Pin: GPIO 0 (analog input)
 * - Serial Baud Rate: 115200
 * 
 * Functionality:
 * - readAvg(): Reads 20 analog values with a 1ms delay between each, returns the average.
 * - setup(): Initializes serial communication and sets ADC attenuation.
 * - loop(): Continuously reads average voltage and prints it every second.
 * 
 * Author: @jpg.py
 * Date: 2026-27-04
 */
#include <Arduino.h>

#define UV_PIN 0

float readAvg() {
  float total = 0;
  for (int i = 0; i < 20; i++) {
    total += analogRead(UV_PIN);
    delay(1);
  }
  return total / 20;
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  float raw = readAvg();
  float voltage = raw * (3.3 / 4095.0);

  Serial.print("Avg Voltage: ");
  Serial.println(voltage);

  delay(1000);
}