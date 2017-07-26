/* The Reader for Universe scan
   O. Dadoun & N. Darrot May 2016
   odadoun@gmail.com
   Read an ascii file (located on arduino SD card) onto SDRAM
   file (with nb_lines) format is
   pixel_i,pixel_j,Red,Green,Blue ... RGB between 0 up to 255
   180 Mega lines !!!
   Load from a file located on arduino SD card the constellation
   name and positions into memory 75 lignes

|  _ \ ___  __ _  __| | ___ _ __| | | |_ __ (_)_   _____ _ __ ___
| |_) / _ \/ _` |/ _` |/ _ \ '__| | | | '_ \| \ \ / / _ \ '__/ __|
|  _ <  __/ (_| | (_| |  __/ |  | |_| | | | | |\ V /  __/ |  \__ \
|_| \_\___|\__,_|\__,_|\___|_|   \___/|_| |_|_| \_/ \___|_|  |___/
*/
#include "TheReaderUniverse.h"

TheReaderUniverse::TheReaderUniverse(): itsFileName("ONEPIXEL.TXT"), nb_lines_read(0), nb_bytes_read(0)
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
        line_extracter(char_temp, abac);

        sprintf(abacus[nb_lines].name_abac, "%s", abac[0]);
        abacus[nb_lines].begin_x = strtoul(abac[1], NULL, 0);
        abacus[nb_lines].begin_y = strtoul(abac[2], NULL, 0);
        abacus[nb_lines].end_x   = strtoul(abac[3], NULL, 0);
        abacus[nb_lines].end_y   = strtoul(abac[4], NULL, 0);

        temp = "";
        nb_lines++;
      }
    }
    myFile.close();
    Serial.println("File closed");
  }
  else Serial.print("Error opening : ");
}

String TheReaderUniverse::return_constellation(unsigned long int  pix_x, unsigned long int  pix_y)
{
  unsigned long int x0 = 19000;
  unsigned long int y0 = 9500;

  String name_value;
  char temp[64];
  sprintf(temp, "%s", "Nothing interesting here ...");

  String tmp = "";
  String name = "";
  for (int i = 0; i < nb_lines_abaccus; i++)
  {
    tmp = String(abacus[i].name_abac);
    tmp.replace('-', ' ');
    if ( pix_x <= x0 - abacus[i].begin_x && pix_x >= x0 - abacus[i].end_x
         &&  pix_y <= y0 - abacus[i].begin_y && pix_y >= y0 - abacus[i].end_y )
    {
      name = tmp;
      //cout << name <<  " LIne " << i << endl;
      break;
    }
  }
  return name;
}


float TheReaderUniverse::GetLongitude(unsigned long int pix_x)
{
  float longitude = 0.;
  if (pix_x <= 9500) longitude = -(180. / 9500.) * (pix_x - 9500.);
  else longitude = (180. / 9499) * (pix_x - 9500.);
  return longitude;
}

float TheReaderUniverse::GetLatitude(unsigned long int pix_y)
{
  float latitude = 0.;
  if (pix_y >= 4749) latitude = (90. / 4750) * (pix_y - 4749.);
  else latitude = (90. / 4749) * (pix_y - 4749.);
  return latitude;
}

void TheReaderUniverse::line_extracter(char* input_string, char (&name_value)[5][64])
{
  char *token;
  token = strtok(input_string, ",");
  int i = 1;
  while (token != NULL ) {
    sprintf(name_value[i - 1], "%s", token);
    token = strtok(NULL, ",");
    i++;
  }
}

unsigned long int TheReaderUniverse::injection(unsigned long int which_line)
{
        File fp = SD.open("ONEPIXEL.TXT", FILE_READ);  
        unsigned long int nb_bytes_read=0;
        char c;
        unsigned long int counter=1;
        while (fp.available()) {
        char c = fp.read();
        if(counter<=which_line)
        {
          if (c != '\n' )  nb_bytes_read+=1;
          else counter+=1;
        }
        else break;
        }
        fp.close();
        return nb_bytes_read+counter-1; // \n need to be count
}
