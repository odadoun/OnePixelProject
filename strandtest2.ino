// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!
#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#include <SD.h>
#define NUMPIXELS 64 // Number of LEDs in strip
// Here's how to control the LEDs from any two pins:
#define DATAPIN    5
#define CLOCKPIN   6
Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);
const int  buttonPinB = 8;
const int  buttonPinA = 9;// the pin that the encoder is attached to
//const int ledPin = 13;       // the pin that the LED is attached to
const int ethPin = 10;
const int sdPin = 4;
// Variables will change:
int buttonPushCounter = 0;   // counter for the number of encoder switch
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

int increment_k2000 = 0;

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
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.
int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)
//unsigned int *led_nb = (unsigned int*)malloc(NUMPIXELS * sizeof(unsigned int));
//bool *seq = (bool*)malloc(NUMPIXELS * sizeof(bool));

void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPinB);
  strip.setBrightness(8);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;

      //increment();
      Serial.println(buttonState);
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
      K2000(buttonPushCounter);

   
      //all_pixels_off();
      //onestep(buttonPushCounter);
      /* From 1 to 63 */  
      //led_nb = fill_led_number();
      /*LED sequence*/    
      //seq=fill_led_sequence();
      
     /* 
      for(int i=0;i<NUMPIXELS;i++)
      {
        //Serial.println(led_nb[i]);
        if(seq[i] != 0)
        {
          //Serial.println(seq[i]);
          strip.setPixelColor(led_nb[i], color);
        }  
        else
       {
          //Serial.println("0");
          strip.setPixelColor(led_nb[i], 0);
        }
      }
     strip.show();
     */
    }
    else {
      //if (lastButtonState != HIGH) 
      Serial.println(lastButtonState);
      // if the current state is LOW then the button
      // wend from on to off:
       Serial.println("off");
      //increment();
    }
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
}
/**********************************/
/* All pixels switch OFF          */
void all_pixels_off()
{
  for(int i=0;i<NUMPIXELS;i++)
    strip.setPixelColor(i, 0);
  strip.show(); 
}
/**********************************/
/*      All pixels switch ON      */
void all_pixels_on()
{
  for(int i=0;i<NUMPIXELS;i++)
    strip.setPixelColor(i, color);
  strip.show();
}
/**********************************/
/* One pixel deplacement, K2000 style */
void K2000(int index)
{
  int index_modulo = index % NUMPIXELS;
  Serial.println(index_modulo);
  if(increment_k2000%2==0)
  {
    strip.setPixelColor(index_modulo, color);
    if(index_modulo!=0)
      strip.setPixelColor(index_modulo - 1, 0);
    else
    {
      strip.setPixelColor(0, 0); // boudary problem
      increment_k2000+=1;
    }  
  }
  else
  {
    strip.setPixelColor(NUMPIXELS-index_modulo, color);
  if(index_modulo!=0)
      strip.setPixelColor(NUMPIXELS-index_modulo + 1, 0);
    else
    {
      strip.setPixelColor(1, 0);
      strip.setPixelColor(0, color);
      increment_k2000+=1;
    }
  }
  strip.show(); // Refresh strip
}
/**********************************/
/* One pixel deplacement          */
/* (always in the same direction) */ 
void onestep(int index)
{
  int index_modulo = index % NUMPIXELS;
  strip.setPixelColor(index_modulo, color);
  if(index_modulo!=0)
    strip.setPixelColor(index_modulo - 1, 0);
  else
    strip.setPixelColor(NUMPIXELS-1, 0);
  strip.show(); // Refresh strip
  Serial.print("index modulo :");
  Serial.println(index_modulo);
}
/**********************************/
/*  LED number from 0 up to   63 */
unsigned int* fill_led_number()
{
  //unsigned int *all_led = NULL;
  //all_led = (unsigned int*)malloc((NUMPIXELS-1) * sizeof(unsigned int));
  unsigned int all_led[NUMPIXELS];
  for(int i=0;i<NUMPIXELS;i++)
  {
    all_led[i]=i;
  }
  return all_led;
} 
/**********************************/
/* FILL LED sequence on-off (1/0) */
/* from 0 up to 63                */
bool* fill_led_sequence()
{
  bool on_off[NUMPIXELS];
  for(int i=0;i<NUMPIXELS;i++)
  {
    if(i<30)
      on_off[i]=0;
    else
      on_off[i]=1;
  }
  return on_off;
} 
