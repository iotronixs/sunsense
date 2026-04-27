

/*
 * @file softap_test.ino
 * @brief ESP32 SoftAP (Software Access Point) Setup Example
 * 
 * This code demonstrates how to configure and start a SoftAP on an ESP32 microcontroller.
 * The SoftAP allows the ESP32 to act as a Wi-Fi access point, enabling other devices to connect to it.
 * It sets up a network with a specified SSID and password, configures the channel, visibility, and maximum clients.
 * The code also monitors and prints the number of connected clients periodically.
 * 
 * Key features:
 * - SSID: "Sunsense-AP001"
 * - Password: "12345678" (minimum 8 characters)
 * - Channel: 6 (chosen for potentially cleaner spectrum, range 1-13)
 * - Hidden SSID: false (visible to clients)
 * - Max clients: 4
 * - TX Power: 8.5 dBm (set for moderate transmission power)
 * - Board: ESP32-C3 SuperMini (compact module with ESP32-C3 chip, suitable for low-power IoT applications)
 * - Configuration: Uses WiFi library for ESP32, resets WiFi before setup to ensure clean state.
 * - Serial output: Provides feedback on setup success, IP address, and connected client count.
 * 
 * Hardware Requirements:
 * - ESP32-C3 SuperMini board (or compatible ESP32-C3 module)
 * - USB connection for programming and serial monitoring
 * 
 * Software Requirements:
 * - Arduino IDE with ESP32 board support installed
 * - WiFi library (included in ESP32 core)
 * 
 * Usage:
 * - Upload the code to the ESP32-C3 SuperMini.
 * - Open Serial Monitor at 115200 baud to view output.
 * - Connect devices to the "Sunsense-AP001" network using the password "12345678".
 * - Monitor connected clients in the Serial output.
 * 
 * Notes:
 * - Ensure the channel is not heavily congested for better performance.
 * - The code resets WiFi to avoid issues from previous runs.
 * - IP address is automatically assigned (default SoftAP IP is 192.168.4.1).
 * - This is a basic example; for production, add error handling and security measures.
 * 
 * @author [@jpg.py]
 * @date [2026-27-04]
 */
#include <WiFi.h>

const char* ssid = "Sunsense-AP001"; // SSID (network name)
const char* password = "12345678";  // min 8 chars

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nStarting SoftAP...");

  // 1. Reset WiFi completely (important if you're testing repeatedly)
  WiFi.disconnect(true, true);
  delay(1000);

  WiFi.mode(WIFI_AP);

  WiFi.setTxPower(WIFI_POWER_8_5dBm);  


  // 4. Start SoftAP
  bool result = WiFi.softAP(
    ssid,
    password,
    6,      // channel (1–13, choose clean one)
    false,  // hidden SSID? (false = visible)
    4       // max clients
  );

  if (result) {
    Serial.println("SoftAP started successfully");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("SoftAP FAILED");
  }
}

void loop() {
  // Print connected clients
  Serial.print("Connected clients: ");
  Serial.println(WiFi.softAPgetStationNum());
  delay(3000);
}