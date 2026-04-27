

/**
 * @file webserver_test.ino
 * @brief ESP32 Web Server for Sensor Dashboard
 * 
 * This Arduino sketch sets up an ESP32 microcontroller as a WiFi Access Point (AP) and hosts a simple web server.
 * The server serves an HTML dashboard that displays real-time sensor values read from an analog pin (SENSOR_PIN).
 * The dashboard updates every second via AJAX requests to the "/data" endpoint.
 * 
 * Key features:
 * - WiFi AP mode with configurable SSID and password.
 * - Web server on port 80 with two routes: "/" for the main page and "/data" for sensor data.
 * - Responsive HTML UI with dark theme styling.
 * - Automatic sensor value fetching and display.
 * 
 * @author @jpg.py
 * @date 2026-04-27
 * 
 * @section limitations Limitations
 * - Only supports one sensor pin (analog input on pin 0).
 * - No authentication or security measures beyond basic AP password.
 * - Limited to 4 concurrent connections in AP mode.
 * - Sensor value is raw analog read (0-4095 for ESP32), not calibrated or processed.
 * - No error handling for WiFi disconnections or sensor failures.
 * - Hardcoded HTML in the code; not dynamically generated.
 * 
 * @section advancements Advancements
 * - Uses ESP32's built-in WiFi capabilities for easy setup without external router.
 * - Real-time updates via JavaScript fetch API, providing a live dashboard.
 * - Low-power operation suitable for battery-powered projects.
 * - Extensible: can add more routes or sensors by modifying the code.
 * - Compatible with ESP32-C3 and similar variants.
 * 
 * @section usage How to Use
 * 1. Upload this sketch to an ESP32 board using Arduino IDE (ensure ESP32 board support is installed).
 * 2. Connect a sensor (e.g., potentiometer or light sensor) to analog pin 0 (SENSOR_PIN).
 * 3. Power on the ESP32; it will create a WiFi AP named "ESP32_C3_AP" with password "12345678".
 * 4. Connect a device (phone, laptop) to this WiFi network.
 * 5. Open a web browser and navigate to the AP's IP address (printed in Serial monitor, typically 192.168.4.1).
 * 6. View the sensor dashboard; values update every second.
 * 7. Monitor Serial output for debugging (e.g., AP start confirmation).
 * 
 * Dependencies:
 * - WiFi.h (ESP32 WiFi library)
 * - WebServer.h (ESP32 WebServer library)
 * 
 * Hardware Requirements:
 * - ESP32-c3 supermini microcontroller 
 * - Analog sensor connected to GPIO 0
 * 
 * Notes:
 * - Adjust WiFi settings (ssid, password) as needed for security.
 * - Modify SENSOR_PIN if using a different analog input.
 * - For production, consider adding HTTPS, authentication, and data validation.
 */
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "sunsense-AP001"; // SSID (network name)
const char* password = "12345678";

WebServer server(80);

#define SENSOR_PIN 0

// -------- HTML UI --------
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Sensor Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial;
      text-align: center;
      background: #0f172a;
      color: #e2e8f0;
    }
    .card {
      background: #1e293b;
      padding: 20px;
      margin: 50px auto;
      width: 300px;
      border-radius: 15px;
      box-shadow: 0 0 20px rgba(0,0,0,0.5);
    }
    h1 {
      color: #38bdf8;
    }
    .value {
      font-size: 40px;
      margin-top: 20px;
      color: #22c55e;
    }
  </style>
</head>
<body>

  <div class="card">
    <h1>Sensor Value</h1>
    <div class="value" id="sensor">--</div>
  </div>

<script>
function fetchData() {
  fetch("/data")
    .then(response => response.text())
    .then(data => {
      document.getElementById("sensor").innerHTML = data;
    });
}

// update every 1 second
setInterval(fetchData, 1000);

// initial call
fetchData();
</script>

</body>
</html>
)rawliteral";

// -------- ROUTES --------
void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleData() {
  int sensorValue = analogRead(SENSOR_PIN);
  server.send(200, "text/plain", String(sensorValue));
}

void setup() {
  Serial.begin(115200);

  // Reset WiFi properly
  WiFi.disconnect(true, true);
  delay(1000);

  WiFi.mode(WIFI_AP);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);

  WiFi.softAP(ssid, password, 6, false, 4);

  Serial.println("AP Started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Routes
  server.on("/", handleRoot);
  server.on("/data", handleData);

  server.begin();
}

void loop() {
  server.handleClient();
}