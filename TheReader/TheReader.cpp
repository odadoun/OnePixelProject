/* The Reader class
 * O. Dadoun May 2016
 * odadoun@gmail.com
 * Loads an ascii file (located on arduino SD card) onto SDRAM
 * file (with nb_lines) format is
 * 01010101 ... 64 times corresponding to ON/OFF leads status
 * (64 leds bar)
 * the file is loaded into memory char[8*nb_lines] is using
 * i.e 8*8 bytes = 64 bits (xnb_lines)
*/
#include "TheReader.h"
#define NUMPIXELS 64
#define BASE_8 8 

TheReader::TheReader(String file_name):itsFileName(file_name),total_bytes(0),nb_lines(0)
{   
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("In the Reader Constructor"); 
  if (!SD.begin(4)) 
  {
    Serial.println("Initialization failed!");
   } 
    fill_sequence_mem();
}
 
TheReader::~TheReader(void)
{ 
  Serial.println("In the Reader Destructor"); 
  myFile.close();
  Serial.println("File closed");
  delete[] itsLines;
}


void TheReader::SetBitsLineSequence(int i,char val)
{
  sprintf(itsLines+i,"%c",val);
 }
 
char TheReader::GetBitsLineSequence(int i)
{
  return itsLines[i];
}

void TheReader::fill_sequence_mem()
{             
        int i=0;
        String temp="";   
       
	      myFile=SD.open(itsFileName,FILE_READ);
        /* Init first byte reallocation will be needed ...*/   
        itsLines = (char*)malloc(8*sizeof(char));

         if (myFile) {
               Serial.println(itsFileName);
               Serial.println("File opened");
                
              while (myFile.available()) {
                char c = myFile.read();
                char cc = '0';
                if(c != '\n' )
                 {            
                  temp+=c;
                  i++; 
                  if(i%BASE_8==0) 
                  { // 8 char of bits correspond to 1 byte (8x1 bits)
                    cc = strtol(temp.c_str(), 0, 2);
                    SetBitsLineSequence(total_bytes,cc);
                    temp="";
                    total_bytes++;
                   }            
                 }
                else
                 {  
                    temp="";
                    i=0;  
		                nb_lines++; 
                    itsLines = (char*)realloc(itsLines,nb_lines*8*(sizeof(char)));
                  }
              }
              myFile.close();
            }         
      else 
      {
      Serial.print("Error opening : ");
      Serial.println(itsFileName);
      } 

     if(total_bytes    != nb_lines * BASE_8 ) 
	  Serial.println("Oups !!! this should not happen ... total_bytes != nb_lines * BASE_8 something very very wrong !!!");	
}

String TheReader::eight_bits_sequence(int nb_bytes)
{ 
  unsigned char c = GetBitsLineSequence(nb_bytes);
  String temp="";
  for(int j = 7; j >= 0; --j)
  {
        temp+=((c >> j) & 1);
   }
   return temp;
}
        

