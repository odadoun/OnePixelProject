/* April 2016 odadoun@gmail.com */
/*  Dadoun for One Pixel project  */
/* Ibis project  in the following */
#include <Adafruit_DotStar.h>
#include "TheReader.h"
#include <SPI.h>       
#include <SD.h>
#define NUMPIXELS 64 // Number of LEDs in strip
#define DATAPIN    5
#define CLOCKPIN   6

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
const int  buttonPinB = 8;
const int  buttonPinA = 9;
const int ethPin = 10;
const int sdPin = 4;
int buttonPushCounter = 0;   
int buttonState = 0;         
int lastButtonState = 0;    


TheReader reader("test1.txt");//""PHEROM~1.TXT");
int n=reader.GetNbLines();
int tot_bytes=reader.GetTotalBytes();
bool ended_boolean="false";

void setup() {
  
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  // initialize the encoder pin as a input:
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
  //   initialize the LED as an output:
  pinMode(ethPin, OUTPUT);
  pinMode(sdPin, OUTPUT);
  digitalWrite(ethPin, HIGH); //turn ethernet line off to prevent SD bug
  digitalWrite(sdPin, HIGH);
  //initialize serial communication:
  Serial.begin (9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  /* A K2000 STYLE FOR INIT */
  Serial.println("Initialisation stage ...");

  
}

uint32_t color = 0xFF0000;    
void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPinB);
  strip.setBrightness(10);
  String list=""; 

  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      if(ended_boolean == true) 
	{
	File fi=reader.GetFile(); 
  String fi_name=  reader.GetFileName();
	Serial.println("END File, close it and reopen it ...");
  	fi.close();
	fi=SD.open(fi_name,FILE_READ);
	}
      // if the current state is HIGH then the button
      // wend from off to on:
      //increment();
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
 
       ended_boolean = reader.fill_sequence_online(buttonPushCounter,list);
        Serial.println(ended_boolean);
       Serial.println(list);
       for(int i=0;i<NUMPIXELS;i++) 
       {
        if(list[i] == '1')
        strip.setPixelColor(i, color); // 'On' pixel at head
        else strip.setPixelColor(i, 0);  
       }
       strip.show();
       buttonPushCounter++;
    }
    else {
       Serial.println("off");   
    }
  }

  lastButtonState = buttonState;
}


