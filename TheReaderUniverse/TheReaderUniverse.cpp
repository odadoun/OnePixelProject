/* The Reader for Universe scan
   O. Dadoun & N. Darrot May 2016
   odadoun@gmail.com
   Loads an ascii file (located on arduino SD card) onto SDRAM
   file (with nb_lines) format is
   pixel_i,pixel_j,Red,Green,Blue ... RGB between 0 up to 255
*/
#include "TheReaderUniverse.h"
TheReaderUniverse::TheReaderUniverse()
{;}

TheReaderUniverse::TheReaderUniverse(String file_name): itsFileName(file_name), nb_lines_read(0), nb_bytes_read(0)
{
 
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!SD.begin(4))
  {
    Serial.println("Initialization failed!");
    return;
  }

  Serial.println("In the Reader Constructor");
  if (SD.exists(itsFileName))
  {
    Serial.print(itsFileName);
    Serial.println(" file open");
    myFile = SD.open(itsFileName, FILE_READ);
    Serial.println(myFile);
  }
  else {
    Serial.println("File does not exist ... '(");
  }
}

TheReaderUniverse::~TheReaderUniverse(void)
{
  Serial.println("In the Reader Destructor");
}

bool TheReaderUniverse::fill_sequence_online(long int (&pix_rgb)[5])
{
  String temp = "";
  bool value = false;
  bool new_line = false;
  int line_length;

  if (myFile)
  {
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

    /* convert the line string into pixel and rgb integer thanks to subStr method */
    char cool_chaine[25];
    strcpy(cool_chaine, led_list.c_str());
    pix_rgb[0] = atoi(subStr(cool_chaine, ",", 1));
    pix_rgb[1] = atoi(subStr(cool_chaine, ",", 2));
    pix_rgb[2] = atoi(subStr(cool_chaine, ",", 3));
    pix_rgb[3] = atoi(subStr(cool_chaine, ",", 4));
    pix_rgb[4] = atoi(subStr(cool_chaine, ",", 5));
    
    new_line=false;
    temp="";
    if(nb_bytes_read>= myFile.size())
    {
      Serial.println("Amazing !! End of file was reached ...");
      nb_lines_read=0;
      nb_bytes_read=0;
      value=true;
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
  return value;
}


/* Very very cool function took from this url
   http://www.esologic.com/?p=1157
*/
char* TheReaderUniverse::subStr(char* input_string, char *separator, int segment_number) {
  char *act, *sub, *ptr;
  static char copy[25];
  int i;
  strcpy(copy, input_string);
  for (i = 1, act = copy; i <= segment_number; i++, act = NULL) {
    sub = strtok_r(act, separator, &ptr);
    if (sub == NULL) break;
  }
  return sub;
}
