
/**
 * @file sunsense.ino
 * @brief SunSense UV Monitoring Device Firmware
 * 
 * This Arduino sketch implements a portable UV exposure monitoring device called SunSense.
 * It uses an ESP32-C3 SuperMini board to read UV sensor data, calculate exposure doses based on SPF,
 * display information on a 1.8" ST7735 TFT screen, handle user inputs via buttons, and provide haptic
 * feedback through a vibration motor. The device tracks UV index, accumulates dose over time,
 * estimates remaining safe exposure time, and alerts users to high UV conditions or the need to
 * reapply sunscreen. Sessions can be saved to non-volatile storage for later analysis via a dashboard.
 * 
 * Key features:
 * - UV sensor calibration and reading with averaging for stability.
 * - SPF selection screen for user input.
 * - Real-time monitoring with color-coded UV display (green/yellow/red).
 * - Dose accumulation using effective UV (averaged over 30-second windows).
 * - Time estimation based on SPF-adjusted dose limits.
 * - Danger alerts for prolonged high UV exposure (>6.5 UV index for >1 hour).
 * - Motor vibration alerts for reapplication reminders.
 * - Session saving for durations >=20 minutes.
 * - Reset functionality via button combination.
 * - Serial output for debugging and mirroring UI data.
 * 
 * Hardware components:
 * - ESP32-C3 SuperMini microcontroller.
 * - Adafruit ST7735 1.8" TFT display (160x128 pixels).
 * - UV sensor connected to analog pin 0 (ADC).
 * - Two buttons: UP (pin 8) and OK (pin 9), with pull-up resistors.
 * - Vibration motor on pin 5.
 * - SPI interface for TFT (pins 2, 3, 7, 6, 10).
 * - TFT backlight on pin 4.
 * 
 * Limitations:
 * - UV calculation is empirical and may not be highly accurate without calibration against a reference sensor.
 * - Dose model assumes linear accumulation and may not account for all environmental factors (e.g., reflection, clothing).
 * - TFT display updates are not optimized for power efficiency; continuous operation drains battery quickly.
 * - Button debouncing is basic and may miss rapid presses.
 * - Storage is limited; only basic session data is saved, no advanced analytics.
 * - No real-time clock; time is based on millis(), which resets on power cycle.
 * - Indoor/outdoor detection is simplistic (based on UV threshold), potentially inaccurate.
 * - Motor alerts are fixed duration and may not be noticeable in noisy environments.
 * 
 * Future advancements:
 * - Integrate GPS for location-based UV adjustments.
 * - Add Bluetooth/WiFi for data syncing to a mobile app.
 * - Implement more sophisticated dose models (e.g., considering skin type, time of day).
 * - Enhance UI with graphs or historical data visualization.
 * - Add battery level monitoring and low-power sleep modes.
 * - Improve sensor accuracy with temperature compensation.
 * - Expand storage for more detailed logs and cloud integration.
 * - Add audio alerts or LED indicators for better accessibility.
 * - Support multiple languages and customizable alerts.
 * 
 * @author -@jpg.py
 * @date 2026-27-04
 * @hardware ESP32-C3 SuperMini
 * 
 * Dependencies:
 * - Arduino.h (built-in with Arduino IDE)
 * - Adafruit_GFX.h (from Adafruit GFX Library)
 * - Adafruit_ST7735.h (from Adafruit ST7735 Library)
 * - SPI.h (built-in with Arduino IDE)
 * - storage.h (custom, must be in the same directory as this sketch)
 * - dashboard.h (custom, must be in the same directory as this sketch)
 * 
 * Installation Guide for Libraries:
 * 1. Open the Arduino IDE.
 * 2. Go to Sketch > Include Library > Manage Libraries...
 * 3. In the Library Manager, search for "Adafruit GFX" and install the latest version (by Adafruit).
 * 4. Search for "Adafruit ST7735" and install the latest version (by Adafruit).
 * 5. The built-in libraries (Arduino.h and SPI.h) are included with the Arduino IDE and do not require separate installation.
 * 6. For custom libraries (storage.h and dashboard.h), ensure they are placed in the same directory as this .ino file. These are project-specific and not available via Library Manager.
 * 7. Ensure the ESP32 board support is installed in Arduino IDE (Tools > Board > Boards Manager > search for "esp32" and install).
 * 8. Select the ESP32C3 Dev Module as the board in Tools > Board.
 * 
 * Note: The custom libraries (storage.h and dashboard.h) are essential for session management and data handling. They must be implemented according to the project's requirements and placed correctly for the sketch to compile and run successfully.
 * - Arduino.h
 * - Adafruit_GFX.h
 * - Adafruit_ST7735.h
 * - SPI.h
 * - storage.h (custom, for session saving)
 * - dashboard.h (custom, for data handling)
 * 
 * 
 */
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

//==== Dashboard ====
#include "storage.h"
#include "dashboard.h"
unsigned long sessionStartTime = 0;
float maxUV = 0;


// ===== TFT =====
#define TFT_CS   7
#define TFT_RST  10
#define TFT_DC   6
#define TFT_BL   4

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ===== PINS =====
#define BTN_UP 8
#define BTN_OK 9
#define MOTOR 5
#define UV_PIN 0

// ===== STATE =====
enum ScreenState { SELECT_SPF, MONITOR };
ScreenState screenState = SELECT_SPF;

int spf = 30;

// ===== BUTTON =====
unsigned long lastUp = 0;
unsigned long lastOk = 0;
#define DEBOUNCE 200

// ===== UV MODEL =====
float baseline = 0;
float uvIndex = 0;
float uvDose = 0;
float doseLimit = 0;
float prevUV = 0;
float displayTime = 120;

float uvBuffer = 0;
int uvSamples = 0;
unsigned long lastWindowUpdate = 0;

float avgUV = 0;

// ===== TIME =====
unsigned long lastTime = 0;

//====== Danger Alerts =====
unsigned long highUVStart = 0;
bool inHighUV = false;
bool dangerTriggered = false;


// ===== MOTOR =====
bool motorState = false;
unsigned long motorTimer = 0;
unsigned long lastMotorTrigger = 0;

bool resetHandled = false;


// ===== CENTER BASE =====
int CX = 80;

// ================= SENSOR =================
float readUV() {
  float total = 0;
  for (int i = 0; i < 25; i++) {
    total += analogRead(UV_PIN);
    delay(1);
  }

  float avg = total / 25;
  float voltage = avg * (3.3 / 4095.0);

  float adjusted = voltage - baseline;
  if (adjusted < 0) adjusted = 0;

  float uv = adjusted * 15.0 * 1.12;
  if (uv > 11) uv = 11;

  return uv;
}

void calibrateSensor() {
  float total = 0;
  for (int i = 0; i < 50; i++) {
    total += analogRead(UV_PIN);
    delay(20);
  }
  baseline = (total / 50) * (3.3 / 4095.0);

  Serial.print("Baseline: ");
  Serial.println(baseline);
}

// ================= BUTTON =================
void handleButtons() {

  if (digitalRead(BTN_UP) == LOW && millis() - lastUp > DEBOUNCE) {
    lastUp = millis();

    if (screenState == SELECT_SPF) {
      spf += 5;
      if (spf > 60) spf = 20;

      Serial.print("SPF Changed: ");
      Serial.println(spf);
    }
  }

  if (digitalRead(BTN_OK) == LOW && millis() - lastOk > DEBOUNCE) {
    lastOk = millis();

    if (screenState == SELECT_SPF) {
      screenState = MONITOR;

      sessionStartTime = millis();
      maxUV = 0;

      uvDose = 0;
      doseLimit = 120.0 * (spf / 30.0);
      lastTime = millis();

      Serial.println("SPF Confirmed → Monitoring Started");

      tft.fillScreen(ST77XX_BLACK);
    }
  }

  // RESET
  if (digitalRead(BTN_UP) == LOW && digitalRead(BTN_OK) == LOW && !resetHandled){
    resetHandled = true;
  Serial.println("Reset Triggered");

  int duration = (millis() - sessionStartTime) / 60000;

if (duration >= 20) {

  Session s;
  s.avgUV = avgUV;
  s.maxUV = maxUV;
  s.duration = duration;
  s.spf = spf;

  saveSession(s);

  Serial.println("Session Saved");
}

  // reset state
  screenState = SELECT_SPF;
  spf = 30;

  // reset UV model
  uvDose = 0;
  prevUV = 0;
  displayTime = 120;

  // reset timing
  lastTime = millis();

  // recalibrate sensor
  calibrateSensor();

  Serial.println("System Reset → Recalibrated");

  delay(500);
  tft.fillScreen(ST77XX_BLACK);
}
if (digitalRead(BTN_UP) == HIGH && digitalRead(BTN_OK) == HIGH) {
  resetHandled = false;
}

}

// ================= MOTOR =================
void motorPulse(int duration) {
  motorState = true;
  digitalWrite(MOTOR, HIGH);
  motorTimer = millis() + duration;
  lastMotorTrigger = millis();

  Serial.println("Motor Triggered");
}

void updateMotor() {
  if (motorState && millis() > motorTimer) {
    digitalWrite(MOTOR, LOW);
    motorState = false;
  }
}

// ================= SPLASH =================
void showSplash() {

  for (int i = 0; i < 255; i += 20) {
    tft.fillScreen(ST77XX_BLACK);
    uint16_t c = tft.color565(i, i, i);

    tft.setTextColor(c);
    tft.setTextSize(2);

    // 🔧 ADJUST HERE
    tft.setCursor(CX - 60, 50);   // "CreatED"
    tft.print("CreatED");

    tft.setCursor(CX - 65, 80);   // "Presents"
    tft.print("Presents");

    delay(40);
  }

  delay(1000);

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);

  // 🔧 ADJUST HERE
  tft.setCursor(CX - 60, 60);   // "SunSense"
  tft.print("SunSense");

  tft.setCursor(CX - 35, 90);   // "V1.0"
  tft.print("V1.0");

  delay(1500);

  tft.fillScreen(ST77XX_BLACK);
}

// ================= SPF SCREEN =================
void drawSPFScreen() {

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  // 🔧 ADJUST HERE
  tft.setCursor(CX - 50, 20);   // "Select"
  tft.print("Select");

  tft.setCursor(CX - 32, 43);   // "SPF"
  tft.print("SPF");

  tft.setTextSize(4);

  // 🔧 ADJUST HERE
  tft.setCursor(CX - 35, 75);   // SPF number
  tft.print(spf);

  tft.setTextSize(1);

  // (Already centered — leave as is)
  tft.setCursor(CX - 40, 120);
  tft.print("UP change");

  tft.setCursor(CX - 40, 135);
  tft.print("OK confirm");
}

// ================= MONITOR UI =================
void drawMonitorUI(float uv, float timeLeft) {

  // ===== DANGER SCREEN =====
if (dangerTriggered) {

  tft.fillScreen(ST77XX_RED);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  tft.setCursor(CX - 60, 60);
  tft.print("DANGER!");

  tft.setCursor(CX - 70, 90);
  tft.print("Too much UV");

  return;  // skip normal UI
}

// normal UI
tft.fillRect(0, 0, 160, 100, ST77XX_BLACK);

  uint16_t color = ST77XX_GREEN;
  if (uv > 7) color = ST77XX_RED;
  else if (uv > 5) color = ST77XX_YELLOW;

  tft.setTextSize(2);

  tft.setTextColor(color);

  // 🔧 ADJUST HERE
  tft.setCursor(CX - 25, 25);   // "UV"
  tft.print("UV");

  // 🔧 ADJUST HERE
  tft.setCursor(CX - 30, 52);  // UV value
  tft.print(uv, 1);

  tft.setTextColor(ST77XX_WHITE);

  // 🔧 ADJUST HERE
  tft.setCursor(CX - 70, 85);  // "Time"
  tft.print("Time left");

  // 🔧 ADJUST HERE
  tft.fillRect(0, 110, 160, 40, ST77XX_BLACK);


  tft.setCursor(CX - 35, 115);  // Time value
  tft.print((int)timeLeft);
  tft.print("m");

  // ===== SERIAL MIRROR =====
  Serial.print("UV: ");
  Serial.print(uv);

  Serial.print(" | SPF: ");
  Serial.print(spf);

  Serial.print(" | Time Left: ");
  Serial.print(timeLeft);

  Serial.println(" min");
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  initStorage();
  startDashboard();

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(MOTOR, OUTPUT);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  SPI.begin(2, -1, 3);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(2);

  showSplash();

  calibrateSensor();
  drawSPFScreen();
}

// ================= LOOP =================
void loop() {
  handleDashboard(uvIndex, displayTime, spf);
  handleButtons();
  uvIndex = readUV();
  if (uvIndex > maxUV) maxUV = uvIndex;

  // ===== HIGH UV TRACKING =====
if (uvIndex > 6.5) {

  if (!inHighUV) {
    inHighUV = true;
    highUVStart = millis();
  }

  // check if exposed > 1 hour
  if ((millis() - highUVStart) > 3600000) { // 1 hour
    dangerTriggered = true;
  }

} else {
  inHighUV = false;
  highUVStart = 0;
  dangerTriggered = false;
}

  if (screenState == SELECT_SPF) {
    drawSPFScreen();
    delay(150);
    return;
  }

  float dt = (millis() - lastTime) / 60000.0;
  lastTime = millis();

  // ===== SMOOTH UV =====
  float uvSmooth = uvIndex * 0.7 + prevUV * 0.3;
  prevUV = uvSmooth;

  // ===== ACCUMULATE WINDOW =====
  uvBuffer += uvSmooth;
  uvSamples++;

  // every 30 seconds update average
  if (millis() - lastWindowUpdate > 30000) {

    avgUV = uvBuffer / uvSamples;

    uvBuffer = 0;
    uvSamples = 0;
    lastWindowUpdate = millis();
  }

  // ===== USE AVERAGE UV (NOT INSTANT UV)
  float effectiveUV = (avgUV > 0) ? avgUV : uvSmooth;

  float exposureRate;

  // outdoor scaling
  if (effectiveUV >= 0.5) {
    exposureRate = effectiveUV / 3.0;
  }
  else {
    // indoor: degrade by TIME, not UV
    exposureRate = 1.0;   // full depletion in ~120 min
  }

  uvDose += exposureRate * dt;
  if (uvDose > doseLimit) uvDose = doseLimit;

  float calcTime = (doseLimit - uvDose); // (exposureRate + 0.05);
  if (calcTime < 0) calcTime = 0;
  if (calcTime > doseLimit) calcTime = doseLimit;

  displayTime = 0.6 * displayTime + 0.4 * calcTime;

  drawMonitorUI(uvIndex, displayTime);

  if (dangerTriggered && millis() - lastMotorTrigger > 5000) {
    motorPulse(400);
    tft.fillRect(0, 0, 160, 30, ST77XX_BLACK);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(CX - 70, 5);
    tft.print("Reapply Sunscreen!");
  }

  updateMotor();

  delay(120);
}
