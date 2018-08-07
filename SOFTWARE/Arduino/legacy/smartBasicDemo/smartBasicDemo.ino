/*
 * smartBasicDemo.ino
 * 
 * The following script was designed to demo the capabilities of the Smart Basic FTDI breakout built by SparkFun
 * The script has been designed to be integrated "as-is" to the control algorithms used in PD3D's smart devices
 * 
 * Fluvio L Lobo Fenoglietto 10/05/2016
 */

#include "protocol.h"
#include "states.h"

byte      inByte = 0x00;

void setup()
{
  BTooth.begin( SPEED );
  pinMode( BLUESWITCH, OUTPUT );                // Define the switch digital pin as an output
  digitalWrite( BLUESWITCH, HIGH );             // Automatically set the switch pin HIGH to communicate through the host after disconnecting the USB
  readyState = READY;
}

void loop()
{
  // if we get a valid byte, read analog ins:
  if ( BTooth.available() > 0 )
  {
    inByte = BTooth.read();                     // get incoming byte

    switch ( inByte )
    {
      //
      //  *** Perform a systems check via remote trigger
      //
      case CHK :
      break;

      //
      //  *** Check system status
      //
      case ENQ :
        if ( readyState == READY )
        {
          BTooth.write( ACK );
        }
        else
        {
          BTooth.write( NAK );
        }
      break;
      case EOT :
      case ACK :
      case NAK :
      case CAN :
      break;
      default :
      break;
    }

  }
  
  inByte = 0x00;
}

