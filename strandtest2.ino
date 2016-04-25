/* April 2016 odadoun@gmail.com */
/*  Dadoun for One Pixel project  */
/* Ibis project  in the following */
#include <Adafruit_DotStar.h>
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

uint32_t color = 0xFF0000;      // 'On' color (starts red)
//unsigned int *led_nb = (unsigned int*)malloc(NUMPIXELS * sizeof(unsigned int));
//bool *seq = (bool*)malloc(NUMPIXELS * sizeof(bool));
unsigned int pos;
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
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
      //pos=K2000(buttonPushCounter);
       ReadIt();
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
unsigned int current_position;
unsigned int K2000(unsigned int index)
{
  unsigned int index_modulo = index % NUMPIXELS;
  if(increment_k2000%2==0)
  {
    strip.setPixelColor(index_modulo, color);
    current_position=index_modulo;
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
    current_position=NUMPIXELS-index_modulo;
  if(index_modulo!=0)
      strip.setPixelColor(NUMPIXELS-index_modulo + 1, 0);
    else
    {
      strip.setPixelColor(1, 0);
      strip.setPixelColor(0, color);
      current_position=0;
      increment_k2000+=1;
    }
  }
  strip.show(); // Refresh strip
  return current_position;
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
/**********************************/
/* READ FILE                     */
void ReadIt()
{
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  File myFile = SD.open("test.txt");
  String line;
  int n=0;
  if (myFile) {
    Serial.println("test.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      line = myFile.readStringUntil('\n');
      Serial.println(line);
      for(unsigned int i=0;i<line.length()-1;i++)
      {
        strip.setPixelColor(i,line[i]);
        Serial.println(i);
        Serial.println(line[i]);
      }
      
      delay(50);
      n++;
    }
    // close the file:
    myFile.close();
 }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

