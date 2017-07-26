/*
   _1PXUniVRS main program
      _ ______  ___   _       ___     ______  ____
     / |  _ \ \/ / | | |_ __ (_) \   / /  _ \/ ___|
     | | |_) \  /| | | | '_ \| |\ \ / /| |_) \___ \
     | |  __//  \| |_| | | | | | \ V / |  _ < ___) |
 ____|_|_|  /_/\_\\___/|_| |_|_|  \_/  |_| \_\____/
|_____|
   The Reader for Universe scan
   O. Dadoun & N. Darrot May 2016
   UPDATED : November 2016
   UPATED : July 2017 conenction to faune.xyz
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
/* */

/**/
#include "TheReaderUniverse.h"
#include <SPI.h>
#include <SD.h>


#define NUMPIXELS 10 // Number of Pixies in the strip
#define PIXIEPIN  6 // Pin number for SoftwareSerial output
Adafruit_LiquidCrystal lcd(0);

SoftwareSerial pixieSerial(-1, PIXIEPIN);

Adafruit_Pixie strip = Adafruit_Pixie(NUMPIXELS, &pixieSerial);
int resetPin = 7;
int resetValue = 0;
const int analogInPin = A2;  // Analog input pin used as a watchdog
int watchdogValue = 0;        // value read from the pot
long randnumber;  // Value for randoming the delay between two pixels
int address = 0;
// This function will return a 4 byte (32bit) long from the eeprom
// at the specified address to address + 3.
/* ******************************** */
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

/* Reaching Faune Stuff */
#include "ReachingFaune.h"

ReachingFaune faune_xyz;
byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x04, 0x37 };
// Set the static IP address to use if the DHCP fails to assign
IPAddress IP(192, 168, 100, 100);
const unsigned long requestInterval = 100;
boolean test_connection = false;
void setup() {

  randomSeed (analogRead(A0));
  pinMode ( resetPin, INPUT);
 
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
  String name_file = "ONEPIXEL.TXT";
  if (SD.exists(name_file))
  {
    reader_universe.myFile = SD.open(name_file, FILE_READ);
    //  Serial.print(name_file);
    //  Serial.println(" : file opened");
  }
  else Serial.println("File does not exist ... '(");
  /*  End of testing and opening SD file   */

  // set up the LCD's number of rows and columns:
  lcd.begin(20, 4);

  pixieSerial.begin(115200); // Pixie REQUIRES this baud rate
  // Serial1.begin(115200);  // <- Alt. if using hardware serial port
  strip.setBrightness(255);  // Adjust as necessary to avoid blinding

  // Serial.println(EEPROMReadlong(0));
  //  Serial.println(EEPROMReadlong(4));

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

/********************/
  // attempt a DHCP connection:
  //SD + ethernet can not coexist at the same time
  // disable SD card
    pinMode(4, OUTPUT);  //disable SD card slot on ethernet shield, due to SPI bus
    digitalWrite(4, HIGH);
 
   Serial.println("Attempting to get an IP address using DHCP:");
   if (!Ethernet.begin(mac)) {
    Ethernet.begin(mac, IP);
    }
   Serial.print("My adress is :");
   Serial.println(Ethernet.localIP());
      
   test_connection=faune_xyz.connectToServer();
/****************************/ 
}

void loop() {
    unsigned long int pixel_number;
    unsigned long int px=0;
    unsigned long int py=0;
    int color_r=0;
    int color_g=0;
    int color_b=0;
    String name_const;

    char input_string[256];
    char line_bytes_xy_rgb[7][64];
    

     /* Faune online stuff */ 
    if(faune_xyz.GetMessage().length() > 0 && faune_xyz.GetLecture()==false)
    {
     // Serial.print("I have received this from the server ");
     // Serial.println(faune_xyz.GetMessage());
      sprintf(input_string,"%s",(faune_xyz.GetMessage()).c_str());
      faune_xyz.parse_web(input_string,line_bytes_xy_rgb);
      /*Serial.println(line_xy_rgb[0]);
      Serial.println(line_xy_rgb[1]);
      Serial.println(line_xy_rgb[2]);
      Serial.println(line_xy_rgb[3]);
      Serial.println(line_xy_rgb[4]);
      Serial.println(line_xy_rgb[5]);*/
      pixel_number=strtoul(line_bytes_xy_rgb[0], NULL, 0);
      px=strtoul(line_bytes_xy_rgb[2], NULL, 0);
      py=strtoul(line_bytes_xy_rgb[3], NULL, 0);
      faune_xyz.SetMessage("");
      
      color_r=atoi(line_bytes_xy_rgb[4]);
      color_g=atoi(line_bytes_xy_rgb[5]);
      color_b=atoi(line_bytes_xy_rgb[6]);
      for (int i = 0; i < NUMPIXELS; i++)
        strip.setPixelColor(i, color_r,color_g,color_b);
      strip.show();
      
      reader_universe.SetLinesRead(pixel_number);
      reader_universe.SetBytesRead(strtoul(line_bytes_xy_rgb[1], NULL, 0));
      delay(300);
    } 
    if (faune_xyz.GetClient().connected()) 
    {
      while(faune_xyz.GetClient().available()) faune_xyz.check_page();
    }
    else if (millis() - faune_xyz.GetLastAttemptTime() > requestInterval) 
    {
   // if you're not connected, and two minutes have passed since
   // your last connection, then attempt to connect again:
     test_connection=faune_xyz.connectToServer();
    }
    /* END FAUNE ONLINE */
   
    //if(faune_xyz.GetMessage().length() == 0 && faune_xyz.GetLecture()==true
    if(test_connection==false)
   {
      char xy_RGB[5][64];
      reader_universe.fill_sequence_online(xy_RGB);

      px = strtoul(xy_RGB[0], NULL, 0);
      py = strtoul(xy_RGB[1], NULL, 0);
      // Serial.println("It is the constellation named ");
      
      // strip.setBrightness(30);
  
      color_r=atoi(xy_RGB[2]);
      color_g=atoi(xy_RGB[3]);
      color_b=atoi(xy_RGB[4]);
  
      for (int i = 0; i < NUMPIXELS; i++)
        strip.setPixelColor(i, color_r,color_g,color_b);
      strip.show();

      lastest_line_bytes[0] = reader_universe.GetLinesRead();
      lastest_line_bytes[1] = reader_universe.GetBytesRead();

      Serial.println(reader_universe.GetBytesRead());
   
      for (int boucle = 0; boucle < 60; boucle++) 
      {
        watchdogValue = analogRead(analogInPin);
        resetValue = digitalRead (resetPin);

    
        if (resetValue > 0 ) {
          lastest_line_bytes[0] = 0;
          lastest_line_bytes[1] = 0;
          reader_universe.SetLinesRead(lastest_line_bytes[0]);
          reader_universe.SetBytesRead(lastest_line_bytes[1]);
        }
        // print the results to the serial monitor:
        if (watchdogValue < 800) {
          long address = 0;
          EEPROMWritelong(address, lastest_line_bytes[0]);
          address += 4;
          EEPROMWritelong(address, lastest_line_bytes[1]);
        }
      }
      pixel_number=reader_universe.GetLinesRead();
   }

      
    if(px != 0 && py !=0) 
    {
      name_const = reader_universe.return_constellation(px, py);
      lcd.setCursor(0, 1);
      lcd.print("Galactic coordinate");
      lcd.setCursor(0, 2);
      lcd.print("x=");
      lcd.print(reader_universe.GetLongitude(px));
      lcd.print((char)223);
      lcd.setCursor(10, 2);
      lcd.print("y=");
      lcd.print(reader_universe.GetLatitude(py));
      lcd.print((char)223);
      lcd.setCursor(0, 0);
      lcd.print ("Pixel #");
      lcd.print (pixel_number);
      lcd.print ("  ");
      lcd.setCursor(0, 3);
      lcd.print(name_const);
      lcd.print(" ");
    }
    delay(3);
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

