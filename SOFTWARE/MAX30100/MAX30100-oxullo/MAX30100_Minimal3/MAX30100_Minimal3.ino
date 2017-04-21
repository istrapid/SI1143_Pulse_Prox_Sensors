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
#include "protocol.h"
#include "States.h"
#include "simulations.h"

byte      inByte        = 0x00;   // Byte sent from ControlSystem
byte      _case         = 0x00;   // Simulation Scenario #

void setup() {
  BTooth.begin(SPEED);
  pinMode(BLUEPIN, OUTPUT);     // Set SmartBasic Pin as output
  digitalWrite(BLUEPIN, HIGH);  // Set SmartBasic to HIGH to switch BT Antenna

  // Initialize the PulseOximeter instance and register a function callback for whenever a beat is detected
  pox.begin();
  pox.setOnBeatDetectedCallback(onBeatDetected);


  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display(); // Crucial for displaying data (use after every display.print())
  delay( 500 );

  display.clearDisplay();       // Clear the buffer.
  display.setTextSize(1);       // Set the text size
  display.setTextColor(WHITE);  // Set the text color

  display.println("Initializing...");
  display.print("Please wait!");
  display.display();
  delay(2500);
  display.clearDisplay();
  display.setTextSize(2);

  if (pox.getPartID() > 0) readyState = READY;
  else readyState = NOTREADY;

}

void loop() {
  // Make sure to call update as fast as possible
  pox.update();

  if (BTooth.available()) {
    inByte = BTooth.read();

    switch ( inByte ) {
      case SOH :
        if ( readyState == READY ) {
          connectState  = CONNECTED;
          BTooth.write( ACK );  delay(10);
          display.print("Paired");
          display.display();
          delay( 1000 );
        }

        else BTooth.write( NAK );
        break;

      case ENQ :
        if ( readyState == READY ) {
          connectState  = CONNECTED;
          BTooth.write( ACK );  delay(10);
        }

        else BTooth.write( NAK );
        break;

      case SIM :
        mode = 1;

        if ( readyState == READY ) {
          connectState  = CONNECTED;
          BTooth.write( ACK );  delay(10);
          while (BTooth.available() == 0) ;

          if (BTooth.available() > 0) {
            _case = BTooth.read();  delay(10);
            BTooth.write(ACK);
          }
        }

        else BTooth.write( NAK );
        break;

      case NRMOP :
        mode = 0;

        if ( readyState == READY ) {
          connectState  = CONNECTED;
          BTooth.write( ACK );  delay(10);
        } break;

      case ACK :
      case NAK :
      case CAN :
        break;

      default :
        //Serial.print( (char)inByte );
        break;
    } delay(10);
  }

  // Operation mode switch
  switch (mode) {
    case 0 :
      normalOp();
      break;

    case 1 :
      startSim(_case);
      break;

    case 2 :
      // Do something...
      break;
  } inByte = 0x00;
}

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
  // Serial.println("Beat!");
}

