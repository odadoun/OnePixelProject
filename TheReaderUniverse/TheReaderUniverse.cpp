/* The Reader for Universe scan
   O. Dadoun & N. Darrot May 2016
   odadoun@gmail.com
   Read an ascii file (located on arduino SD card) onto SDRAM
   file (with nb_lines) format is
   pixel_i,pixel_j,Red,Green,Blue ... RGB between 0 up to 255 
   180 Mega lines !!!
   Load from a file located on arduino SD card the constellation 
   name and positions into memory 75 lignes 
*/
#include "TheReaderUniverse.h"

TheReaderUniverse::TheReaderUniverse():itsFileName("ONEPIXEL.TXT"), nb_lines_read(0), nb_bytes_read(0)
{
 ;
}

TheReaderUniverse::~TheReaderUniverse()
{
  Serial.println("In the Reader Destructor");
}

void TheReaderUniverse::fill_sequence_online(char (&pix_rgb)[5][64])
{
  String temp = "";
  bool new_line = false;
  int line_length;

  if (myFile)
  {
    Serial.println("In fill_sequence_online");
    Serial.println(nb_bytes_read);
    Serial.println(nb_lines_read);
    myFile.seek(nb_bytes_read);
    String led_list = "";
    while (myFile.available() && new_line == false)
    {
      char c = myFile.read();
      if ( c != '\n'  )
      {
        temp += c;
      }
      else new_line = true;
    }
    line_length = temp.length() + 1;
    nb_bytes_read += line_length;
    nb_lines_read++;
    led_list = temp;

    /* convert the line string into pixel and rgb */
    char cool_chaine[25];
    
    strcpy(cool_chaine, led_list.c_str());
    line_extracter(cool_chaine, pix_rgb);
    Serial.println(cool_chaine);
    new_line = false;
    temp = "";
    if (nb_bytes_read >= myFile.size())
    {
      Serial.println("Amazing !! End of file was reached ...");
      nb_lines_read = 0;
      nb_bytes_read = 0;
      myFile.close();
      Serial.print(itsFileName);
      Serial.print(" file closed.");
    }
  }
  else
  {
    Serial.print("Error opening : ");
    Serial.println(itsFileName);
  }
}

void TheReaderUniverse::load_constellations_abacus()
{
  String temp = "";
  File myFile = SD.open("CONST.TXT", FILE_READ);
  
  /* NAME_CONSTELLATION MIN_X MIN_Y MAX_X MAX_Y */
  int nb_lines = 0;
  if (myFile) {
    Serial.println("Constellation abacus opened");

    while (myFile.available()) {
      char c = myFile.read();
      if (c != '\n' )
      {
        temp += c;
      }
      else
      {
        //char *char_temp;
        //char_temp = (char*)malloc(temp.length() * sizeof(char));
        char char_temp[128];
        sprintf(char_temp, "%s", temp.c_str());
        
        char abac[5][64];  
        line_extracter(char_temp,abac);
          
        sprintf(abacus[nb_lines].name_abac, "%s", abac[0]);
        abacus[nb_lines].begin_x = atol(abac[1]);
        abacus[nb_lines].begin_y = atol(abac[2]);
        abacus[nb_lines].end_x = atol(abac[3]);
        abacus[nb_lines].end_y = atol(abac[4]);
        
        temp = "";
        nb_lines++;
      }
    }
    myFile.close();
    Serial.println("File closed");
  }
  else Serial.print("Error opening : ");
}

String TheReaderUniverse::return_constellation(long unsigned int pix_x,long unsigned int pix_y)
{
  String name_value;
  char temp[64];
  sprintf(temp,"%s","Nothing interesting here ...");
  
  for(int i=0;i<nb_lines_abaccus;i++)  
  {
    if( pix_x>=abacus[i].begin_x && pix_x<=abacus[i].end_x 
    &&  pix_y>=abacus[i].begin_y && pix_y<=abacus[i].end_y)
    {
      name_value=String(abacus[i].name_abac);
      break;
    }
   else  name_value=String(temp);
  }
  return name_value;
}

 
float TheReaderUniverse::GetLongitude(long unsigned int pix_x)
{
    float longitude=0.; 
    if(pix_x<=9500) longitude=-(180./9500.)*(pix_x-9500.);
    else longitude=(180./9499)*(pix_x-9500.);
    return longitude;
}

float TheReaderUniverse::GetLatitude(long unsigned int pix_y)
{
    float latitude=0.;
    if(pix_y>=4749) latitude=(90./4750)*(pix_y-4749.);
    else latitude=(90./4749)*(pix_y-4749.);
    return latitude;  
}

void TheReaderUniverse::line_extracter(char* input_string, char (&name_value)[5][64])
{
  char *token;
  token = strtok(input_string, ",");
  int i = 1;
  while (token != NULL ) {
    sprintf(name_value[i-1], "%s", token);
    token = strtok(NULL, ",");
    i++;
  }
}
