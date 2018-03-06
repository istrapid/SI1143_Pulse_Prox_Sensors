/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <SPI.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define REPORTING_PERIOD_MS     1000


// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/*
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

  For Arduino users just use the following pins for various port settings
  Or use port 0 for traditional SDA (A4) and SCL (A5)
  Connect pins with 10k resistors in series

  JeeNode users just set the appropriate port

  JeeNode Port  SCL ('duino pin)  SDA ('duino pin)
  0             18 (A5)       19 (A4)
  1             4             14 (A0)
  2             5             15 (A1)
  3             6             16 (A2)
  4             7             17 (A3)
*/


// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
   // Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);

    Serial.println("Initializing MAX30100");
    // Initialize the PulseOximeter instance and register a beat-detected callback
    pox.begin();
    pox.setOnBeatDetectedCallback(onBeatDetected);


    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC);
    // init done

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(2000);

    // Clear the buffer.
    display.clearDisplay();


    //display.begin(SSD1306_SWITCHCAPVCC, 0x3D); // initialize with the I2C addr 0x3C (for the 128x32)
    display.setTextSize(2); // Set the text size
    display.setTextColor(WHITE); // Set the text color

}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    int s = pox.getSpO2();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("HR,");
        Serial.print(pox.getHeartRate());
        Serial.print(",");
        Serial.print("SPO2,");
        Serial.print(pox.getSpO2());
        Serial.print(",");
        Serial.print("TEMP,");
        Serial.print(pox.getTemperature());
        Serial.println(",");

        tsLastReport = millis();

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(pox.getHeartRate(), 1);
        display.println(" bpm");
        display.print(pox.getSpO2());
        display.println(" SPO2");
        display.print(pox.getTemperature(), 2);
        display.println(" C");
        display.display();
    }
}
