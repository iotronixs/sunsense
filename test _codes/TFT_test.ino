

/**
 * @file TFT_test.ino
 * @brief ST7735 TFT Display Initialization and Test Program
 * 
 * @author [@jpg.py]
 * @date [created on 2024-25-04]
 * @version 1.0
 * 
 * @description
 * This program initializes and tests a ST7735 TFT display module connected to an Arduino board.
 * It performs basic display operations including screen fill with various colors and text rendering.
 * The program uses SPI communication protocol with manually configured pin assignments.
 * 
 * @hardware
 * - Microcontroller: ESP32-C3 Supermini
 * - Display Module: Adafruit ST7735 TFT Display 1.8 SPI 128x160 Module
 * - Communication Protocol: SPI
 * 
 * @pin_configuration
 * | Function               | Pin | Direction |
 * |------------------------|-----|-----------|
 * | TFT_CS (Chip Select)   | 7   | Output    |
 * | TFT_DC (Data/Command)  | 6   | Output    |
 * | TFT_RST (Reset)        | 10  | Output    |
 * | TFT_BL (Backlight)     | 4   | Output    |
 * | SPI_SCK (Clock)        | 2   | Output    |
 * | SPI_MOSI/DIN (Data)    | 3   | Output    |
 * | SPI_MISO (Data)        | -1  | Input     |
 * 
 * Note: MISO is not used in this configuration as the display does not require it for basic operations.
 * Note: Ensure that the SPI pins are correctly connected according to the defined pin configuration for proper communication with the display.
 * Note: The backlight pin (TFT_BL) is controlled separately to allow for manual backlight management.
 * Note: Use +5v for the display power supply to ensure proper operation, as the display may not function correctly at 3.3v. or need to short the jumper at the back of the display module
 * 
 * @functionality
 * - Initializes serial communication at 115200 baud
 * - Enables TFT backlight
 * - Initializes SPI communication with custom pin configuration
 * - Initializes display with BLACKTAB configuration
 * - Sets display rotation to 1
 * - Displays "TEST" text on black background
 * - Cycles through color screens (RED, GREEN, BLUE, BLACK)
 * - All operations include appropriate delays for visual confirmation
 * 
 * 
 * @Possible_malfunctions
 * - Display not initializing: Check wiring connections, ensure correct power supply, and verify pin configuration.
 * - Display shows incorrect colors: Verify that the correct initialization function (INITR_BLACKTAB) is used and that the display is properly connected. use INITR_GREENTAB if you have the green tab version of the display
 * - No text displayed: Ensure that the text color is set to a visible color against the background and that the cursor position is correctly set.
    * - Backlight not working: Check the connection of the backlight pin (TFT_BL) and ensure it is set to HIGH in the code.
 */
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>


// NEW PIN CONFIG (FIXED)
#define TFT_CS   7
#define TFT_RST  10
#define TFT_DC   6
#define TFT_BL   4

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting TFT...");

  // Backlight ON
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Force SPI pins (CRITICAL)
  SPI.begin(2, -1, 3, 7);  
  // SCK=2, MOSI=3

  // Try different init (VERY IMPORTANT)
  tft.initR(INITR_BLACKTAB);

  // orientation
  tft.setRotation(1);

  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(10, 20);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.println("TEST");

  delay(2000);

  tft.fillScreen(ST77XX_RED);
  delay(1000);

  tft.fillScreen(ST77XX_GREEN);
  delay(1000);

  tft.fillScreen(ST77XX_BLUE);
  delay(1000);

  tft.fillScreen(ST77XX_BLACK);
  delay(2000);
  tft.fillScreen(ST77XX_BLACK);
  delay(2000);
  

  Serial.println("Done");
}

void loop() {}