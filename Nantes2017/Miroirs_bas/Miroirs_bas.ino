/* O. Dadoun & N. Darrot may 2017
 * odadoun@gmail.com
 _   _                _
| | | | ___  ___ __ _| |_ ___
| |_| |/ _ \/ __/ _` | __/ _ \
|  _  |  __/ (_| (_| | ||  __/
|_| |_|\___|\___\__,_|\__\___|

*/
#include "Hecate.h"
Hecate *hecate = NULL ;
   
int pos=0;
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
 for(int Nc=0;Nc<NB_CONST;Nc++)
 { 
      float xy[2*NB_STARS_MAX];
      hecate->ExtractInfo(xy,Nc);     
      int Ns=hecate->nb_stars[Nc]; 
      Serial.print("Constellation Name :" );
      Serial.println(hecate->const_name[Nc]); 
      Serial.print("Number of stars :" );
      Serial.println(Ns);
   
      float x;
      float y;
      float phi;
      float theta;
 
      hecate->MotorAtRest();

      for(int k=0 ; k < Ns ;k++) 
      {
        int init_theta = 50;
        int init_phi   = 30;
        hecate->initMotorAngle(k,init_theta,init_phi);
      }
      
      for(int k=0 ; k < Ns ;k++) 
      {
        Serial.print("Star n: ");
        Serial.println(k);
        x=xy[2*k];
        y=xy[2*k+1];

        theta = atan(sqrt(x*x+y*y)/DISTANCE_WALL);
        //from angle to motor angle
        theta = theta + 90;
        
        phi   = atan(x/y); 
        //from angle to motor angle
        phi   = 0.6*phi + 90;
              
        Serial.print(theta);
        Serial.print("  :  ");
        Serial.println(phi);

        hecate->GetVoCer()[2*k].write(int(theta));
        hecate->GetVoCer()[2*k+1].write(int(phi));
      // Where all the motors are goin in the same diraction
      // Hardware debug test    
     // hecate->TestSynchronisation();     
      }
      delay(5000);
      Serial.println("Next constellation");
 }     
 
}     

