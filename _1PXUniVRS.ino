/*
 * _1PXUniVRS main program
 * 
   The Reader for Universe scan
   O. Dadoun & N. Darrot May 2016
   odadoun@gmail.com
   Read an ascii file (located on arduino SD card) onto SDRAM
   file (with nb_lines) format is
   pixel_i,pixel_j,Red,Green,Blue ... RGB between 0 up to 255
   180 Mega lines !!!
   Load from a file located on arduino SD card the constellation
   name and positions into memory 75 lignes
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

const int analogInPin = A2;  // Analog input pin used as a watchdog
int watchdogValue = 0;        // value read from the pot

int address = 0;
// This function will return a 4 byte (32bit) long from the eeprom
// at the specified address to address + 3.
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four  = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two   = EEPROM.read(address + 2);
  long one   = EEPROM.read(address + 3);
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
long filePosition = EEPROMReadlong(0);

TheReaderUniverse reader_universe;
long int lastest_line_bytes[2];

void setup() {
  /*  Begin of testing and opening SD file   */
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (!SD.begin(4))
  {
    Serial.println("Initialization failed!");
    return;
  }
  String name_file="ONEPIXEL.TXT";
  if(SD.exists(name_file))
  {
    reader_universe.myFile = SD.open(name_file, FILE_READ);
    Serial.print(name_file);
    Serial.println(" : file opened");
  }
  else Serial.println("File does not exist ... '(");
  /*  End of testing and opening SD file   */

  // set up the LCD's number of rows and columns:
  lcd.begin(20, 4);

  pixieSerial.begin(115200); // Pixie REQUIRES this baud rate
  // Serial1.begin(115200);  // <- Alt. if using hardware serial port
  strip.setBrightness(255);  // Adjust as necessary to avoid blinding
  
  Serial.println(EEPROMReadlong(0));
  Serial.println(EEPROMReadlong(4));

  lastest_line_bytes[0] = EEPROMReadlong(0);
  lastest_line_bytes[1] = EEPROMReadlong(4);
  reader_universe.SetLinesRead(lastest_line_bytes[0]);
  reader_universe.SetBytesRead(lastest_line_bytes[1]);
 /* loaded constellations names and position */
  reader_universe.load_constellations_abacus();

/* Injection line test only for test */
/*  unsigned long int lines_read=500;
  unsigned long int bytes_read=reader_universe.injection(lines_read);
  Serial.println(lines_read);
  Serial.println(10166912);//bytes_read);
  reader_universe.SetLinesRead(lines_read);
  reader_universe.SetBytesRead(bytes_read);*/
  
}

void loop() {

  char xy_RGB[5][64];
  reader_universe.fill_sequence_online(xy_RGB);
  
  for(int i=0;i<=4;i++) 
  {
    Serial.print(xy_RGB[i]); 
    Serial.print(" ");
  }
  Serial.println();
  
  unsigned long int px=strtoul(xy_RGB[0],NULL,0);
  unsigned long int py=strtoul(xy_RGB[1],NULL,0);
  Serial.println("It is the constellation named ");
  String name_const=reader_universe.return_constellation(px,py);
  Serial.println(name_const);

  strip.setBrightness(30);
  for (int i = 0; i < NUMPIXELS; i++)
    strip.setPixelColor(i, atoi(xy_RGB[2]), atoi(xy_RGB[3]), atoi(xy_RGB[4]));
  strip.show();

  lastest_line_bytes[0] = reader_universe.GetLinesRead();
  lastest_line_bytes[1] = reader_universe.GetBytesRead();

  for (int boucle = 0; boucle < 90; boucle++) {
    watchdogValue = analogRead(analogInPin);
    // print the results to the serial monitor:
    if (watchdogValue < 800) {
      long address = 0;
      EEPROMWritelong(address, lastest_line_bytes[0]);
      address += 4;
      EEPROMWritelong(address, lastest_line_bytes[1]);
    }
  }

  lcd.setCursor(0, 0);
  lcd.print(" Galactic coordinate");
  lcd.setCursor(0, 1);
  lcd.print(reader_universe.GetLongitude(px));
  lcd.print((char)223);
  lcd.setCursor(0, 2);
  lcd.print(reader_universe.GetLatitude(py));
  lcd.print((char)223);  
  lcd.setCursor(0, 3); 
  lcd.print(name_const);
  delay(1);
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





