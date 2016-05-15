/*
  Pixie reads data in at 115.2k serial, 8N1.
  Byte order is R1, G1, B1, R2, G2, B2, ... where the first triplet is the
  color of the LED that's closest to the controller. 1ms of silence triggers
  latch. 2 seconds silence (or overheating) triggers LED off (for safety).

  Do not look into Pixie with remaining eye!
*/
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "EEPROM.h"
#include "SoftwareSerial.h"
#include "Adafruit_Pixie.h"

#include "TheReaderUniverse.h"
#include <SPI.h>
#include <SD.h>


#define NUMPIXELS 10 // Number of Pixies in the strip
#define PIXIEPIN  6 // Pin number for SoftwareSerial output
Adafruit_LiquidCrystal lcd(0);

SoftwareSerial pixieSerial(-1, PIXIEPIN);

Adafruit_Pixie strip = Adafruit_Pixie(NUMPIXELS, &pixieSerial);
// Alternately, can use a hardware serial port for output, e.g.:
// Adafruit_Pixie strip = Adafruit_Pixie(NUMPIXELS, &Serial1);
const int analogInPin = A2;  // Analog input pin used as a watchdog
int watchdogValue = 0;        // value read from the pot

int i;
int boucle;
int address = 0;
//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

long filePosition = EEPROMReadlong(0);

TheReaderUniverse reader_universe;
long int lastest_line_bytes[2];

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set up the LCD's number of rows and columns:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("saved cursor pos");
  lcd.setCursor(0, 1);
  lcd.print(filePosition);

  pixieSerial.begin(115200); // Pixie REQUIRES this baud rate
  // Serial1.begin(115200);  // <- Alt. if using hardware serial port
  strip.setBrightness(255);  // Adjust as necessary to avoid blinding
  Serial.println("Blue!");

  reader_universe = TheReaderUniverse("ONEPIXEL.TXT");
  
  Serial.println(EEPROMReadlong(0));
  Serial.println(EEPROMReadlong(4));
  
  lastest_line_bytes[0]=EEPROMReadlong(0);
  lastest_line_bytes[1]=EEPROMReadlong(4);
   
  reader_universe.SetLinesRead(lastest_line_bytes[0]);
  reader_universe.SetBytesRead(lastest_line_bytes[1]);
}

void loop() {

  long int xy_RGB[5];


  bool ended_boolean;
  ended_boolean = reader_universe.fill_sequence_online(xy_RGB);


  Serial.print(reader_universe.GetLinesRead());
  Serial.print(" ");
  Serial.println(reader_universe.GetBytesRead());

  Serial.print(xy_RGB[0]); Serial.print(" "); Serial.print(xy_RGB[1]); Serial.print(" ");
  Serial.print(xy_RGB[2]); Serial.print(" "); Serial.print(xy_RGB[3]); Serial.print(" ");
  Serial.print(xy_RGB[4]); Serial.println();

  //delay(50);

   
  /*
    int red= random(255);
    int blue=random(255);
    int green=random(255);
    int light=random(255);
    strip.setBrightness(200);
    for(i=0; i< NUMPIXELS; i++)
      strip.setPixelColor(i, int (red/5), int (blue/5), int (green/5));
    strip.show();
    filePosition=filePosition+1;
    lcd.setCursor(0,2);
    lcd.print(filePosition);
  */
  
    for (boucle=0; boucle<90; boucle++) {
     watchdogValue=analogRead(analogInPin);
       // print the results to the serial monitor:

    if (watchdogValue < 800) {
     lastest_line_bytes[0]=reader_universe.GetLinesRead();
     lastest_line_bytes[1]=reader_universe.GetBytesRead();  
 
      long address = 0;
      EEPROMWritelong(address, lastest_line_bytes[0]);
      address+=4;
      EEPROMWritelong(address,lastest_line_bytes[1]);
      lcd.setCursor(0,0);
      lcd.print("Lines :");
      lcd.println(lastest_line_bytes[0]);
      lcd.setCursor(0,1);
      lcd.print("Bytes :");
      lcd.println(lastest_line_bytes[1]);
      lcd.setCursor(5,5);
      lcd.println("saved");
      
    }

    // delay(10);
    }
 
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}



