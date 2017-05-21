/* O. Dadoun & N. Darrot may 2017
   odadoun@gmail.com
  _   _                _
  | | | | ___  ___ __ _| |_ ___
  | |_| |/ _ \/ __/ _` | __/ _ \
  |  _  |  __/ (_| (_| | ||  __/
  |_| |_|\___|\___\__,_|\__\___|

*/
#include "Hecate.h"
Hecate *hecate = NULL ;

int pos = 0;
#define darrot_angle_constant 50
#define shift_angle 60

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (!SD.begin(4))  Serial.println("Initialization failed!");
  else Serial.println("Initializing brains ...");

  hecate = new Hecate("starmap.txt");

  Serial.println("SetUp over, entering in the loop ... ");
}

void loop() {
  //   int Nc=12;
  for (int Nc = 0; Nc < NB_CONST; Nc++)
  {
    float xy[2 * NB_STARS_MAX];
    hecate->ExtractInfo(xy, Nc);
    int Ns = hecate->nb_stars[Nc];
    Serial.print("Constellation Name :" );
    Serial.println(hecate->const_name[Nc]);
    Serial.print("Number of stars :" );
    Serial.println(Ns);
    float x;
    float y;
    float phi;
    float theta;
    
    delay(500);
   
    hecate->MotorAtRest();
    delay(5000);
    
    float alpha_min_vertical = 30;
    float alpha_max_vertical = 70;
    float alpha_min_horizontal = amplitude_min_mirror_horizontal;
    float alpha_max_horizontal = amplitude_max_mirror_horizontal;
    
    float old_theta=(90+ANGLE_MIROR);
    float old_phi=90; 
    
    int imax=300;
    
  for(int stepper=0;stepper<=imax;stepper++)
 {

   if(Ns>20) Ns=20;  
    for (int k = 0 ; k < Ns ; k++)
    {
   // Serial.print("Star n: ");
   // Serial.println(k)
      x = xy[2 * k];
      y = xy[2 * k + 1];

      
      theta = atan(sqrt(x * x + y * y) / DISTANCE_WALL);
      theta = theta*180/M_PI;
      //from angle to motor angle
      theta = theta * (alpha_max_vertical - alpha_min_vertical) / 40  + alpha_min_vertical;
      
      phi   = atan(x / y);
      phi  = 180*phi/M_PI;
      //from angle to motor angle
     //  phi   = phi * (alpha_max_horizontal - alpha_min_horizontal) / 180  + alpha_min_horizontal;  
      phi   = phi * (alpha_max_horizontal - (alpha_max_horizontal+alpha_min_horizontal)/2 ) / 180 
              + (alpha_max_horizontal+alpha_min_horizontal)/2;    
     /* Serial.print(int(stepper*(theta-old_theta)/imax)  + int(old_theta));
      Serial.print("  :  ");
      Serial.println(int(stepper*(phi-old_phi)/imax)  + int(old_phi));*/         
      hecate->GetVoCer()[2 * k].write(int(stepper*(theta-old_theta)/imax)  + int(old_theta));
      hecate->GetVoCer()[2 * k + 1].write(int(stepper*(phi-old_phi)/imax)  + int(old_phi));   
     }     
    // Where all the motors are goin in the same diraction
    // Hardware debug test
    //  hecate->TestSynchronisation();
    delay(1);    
    }
    delay(5000);
    Serial.println("Next constellation");
  }
    Serial.println("Constellations finished .... ");
}

