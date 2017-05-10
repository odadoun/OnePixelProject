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

void setup() {
  Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
    if (!SD.begin(4)) 
  {
    Serial.println("Initialization failed!");
   } 
    Serial.println("Initializing brains ...");
    
    hecate = new Hecate("starmap.txt");
   
   for(int i=0;i<NB_CONST;i++)
    {   
      int Nc=i;
      int Ns=hecate->nb_stars[Nc]; 
      Serial.println(hecate->const_name[Nc]); 
      Serial.println(Ns);
      float xy[2*NB_STARS_MAX];
      hecate->Pioche(xy,Nc);  

      for(int k=0 ; k < (hecate->nb_stars[Nc])*2 ;k++) 
      {
        if(k %2 == 0) {
          Serial.print(xy[k]); 
          Serial.print(" " ); 
        }
        else Serial.println(xy[k]);
      }
  }
}

void loop() {

#if 0 
    for(int j=0;j<=n;j++) 
    {
      Serial.print(x[j]);
      Serial.print("  ");
      Serial.println(y[j]);
    }
#endif    
 #if 0 
    for(pos = 50; pos <= 130; pos += 1)
    {
  
      for(int i=0;i<NUMBER_BRAINS;i++)   hecate->GetVoCer()[i].write(pos);
      delay(35);
    }

  for(pos = 130; pos >= 50; pos -= 1)
  {
      for(int i=0;i<NUMBER_BRAINS;i++) hecate->GetVoCer()[i].write(pos);
      delay(35);
  }
#endif  
}

