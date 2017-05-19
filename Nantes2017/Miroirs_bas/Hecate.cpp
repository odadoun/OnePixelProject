/* The Hecate class
   O. Dadoun  May 2017
   odadoun@gmail.com
   read an ascii file (located on arduino SD card)
   SD should be formated in FAT16(32) format
*/
#include "Hecate.h"
#include <Stream.h>
#define Max(A,B) ((A>B)? A:B)

Hecate::Hecate()
{;}
Hecate::Hecate(String file_name):itsFileName(file_name)
{
  Serial.println("Hecate Constructor, init Brains ...");
  Serial.println("Brains  attached : ");
 
  InitAttach();   

}

Hecate::~Hecate(void)
{
  Serial.println("In the Reader Destructor");
  myFile.close();
}

void Hecate::InitAttach()
{
  for (int i = 0; i < NUMBER_BRAINS; i++) VoCer[i].attach(init_pin[i]);
}

void Hecate::TestSynchronisation()
{
   for(int pos = 50; pos <= 130; pos += 1)
    {
      for(int i=0;i<NUMBER_BRAINS;i++)  GetVoCer()[i].write(pos);
      delay(55);
     }
   for(int pos = 130; pos >= 50; pos -= 1)
    {
      for(int i=0;i<NUMBER_BRAINS;i++) GetVoCer()[i].write(pos);
      delay(55);
    }
 }
void Hecate::MotorAtRest()
{
  for(int i=0;i<NUMBER_BRAINS;i++) 
  {
        if(i%2==0)  GetVoCer()[i].write(90+ANGLE_MIROR);
        else GetVoCer()[i].write(90);
  }
}
void Hecate::initMotorAngle(int brain,int theta,int phi)
{
  GetVoCer()[2*brain].write(theta);
  GetVoCer()[2*brain+1].write(phi);
}

void Hecate::column_extracter(char string_line[256], char (&_xy)[4][64])
{
  char *token;
  char temp[256];
  strcpy(temp, string_line); // this is needed if not string_line is changed
  token = strtok(temp, "\t");
  int i = 1;
  while (token != NULL ) {
    sprintf(_xy[i - 1], "%s", token);
    token = strtok(NULL, "\t");
    i++;
  }
  delete token;
}

void Hecate::SetNameConst(int _i1, String temp_name)
{
  const_name[_i1] = temp_name;
}

void Hecate::SetStarsInConst(int _i2,int temp_i)
{
  nb_stars[_i2] = temp_i;
}

void Hecate::ExtractInfo(float (&xy)[2*NB_STARS_MAX], int nb)
{
  String temp="";
  int nb_const=0;
  
  int ii=0;
  char cool_chaine[64];
  char char_xy[4][64];
  myFile = SD.open(itsFileName, FILE_READ);
  
  if (!myFile)
  {
    Serial.println("**** ERROR MESSAGE ***");
    Serial.println(itsFileName);
    Serial.println("doesn't exit ... check, exit now!");
  }
  else
  {   
    Serial.print("File "); 
    Serial.print(itsFileName);
    Serial.println(" opened ...");

    while (myFile.available())
    {
      temp = myFile.readStringUntil('\n');
      if (!temp.startsWith("#"))
      {
        if (isAlpha(temp[0]))
        {
        SetNameConst(nb_const,temp);
        
         nb_const++;
         ii=0;
         if(nb_const-1 > nb  ) break; 
        }
        else
        {            
         SetStarsInConst(nb_const-1,ii+1); 
         strcpy(cool_chaine, temp.c_str());
         column_extracter(cool_chaine,char_xy);
         if( nb_const-1  == nb )
         {
           //Fill same array odd and even
           xy[2*ii]=atof(char_xy[2]); 
           xy[2*ii+1]=atof(char_xy[3]);
         }
          ii++ ;           
        }
      }
    }
  } 
  Serial.println("ExtractInfo done !");
  myFile.close();
}

