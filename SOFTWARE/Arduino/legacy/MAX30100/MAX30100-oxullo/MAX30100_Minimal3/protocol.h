/// Required libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/// Display Configuration
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

/// Instantiate peripherals
PulseOximeter pox;
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/// PulseOximeter variables
#define REPORTING_PERIOD_MS     1000
uint32_t  tsLastReport  = 0;

/// Arduino Variables
#define         BLUEPIN     2                 // SmartBasic Pin to switch BT communication ON/OFF

/// BT Configuration
#define         SPEED       115200            // BaudRate of BT Antenna
#define         BTooth      Serial

/// ASCII Byte Codes -- used for communication protocol
#define         SOH                 0x01      // Start of Heading                                   [resp: --- | ---]
#define         ENQ                 0x05      // Enquiry: "Are you ready for commands?"             [resp: ACK | NAK]
#define         EOT                 0x04      // End of Transmission                                [resp: ACK | NAK]
#define         ACK                 0x06      // Positive Acknowledgement
#define         NAK                 0x15      // Negative Acknowledgement
#define         CAN                 0x18      // Cancel Current Command                             [resp: ACK | NAK]

#define         DC1                 0x11      // Device Control: Diagnostic Functions               [resp: ACK | NAK]
#define         DC1_DEBUGON         0x00      //    Debug Mode ON                                   [resp: ACK | NAK]
#define         DC1_DEBUGOFF        0x01      //    Debug Mode OFF                                  [resp: ACK | NAK]

#define         NRMOP               0x20      // Normal Operation Mode                              [resp: ACK | NAK]
#define         SIM                 0x21      // Simulation Mode                                    [resp: ACK | NAK]
#define         SIM_000             0x30      //    Simulate Scenario 0
#define         SIM_001             0x31      //    Simulate Scenario 1

