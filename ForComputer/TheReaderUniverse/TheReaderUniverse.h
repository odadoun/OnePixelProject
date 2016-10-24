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

#ifndef _TheReaderUniverse_
#define _TheReaderUniverse_H_
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#define nb_lines_abaccus 88

class TheReaderUniverse {
  
  struct abaccus_struct
  {
  char name_abac[32];
  unsigned long int  begin_x;
  unsigned long int  begin_y;
  unsigned long int  end_x;
  unsigned long int  end_y;
  } abacus[nb_lines_abaccus];

  public:
    TheReaderUniverse();
    ~TheReaderUniverse();
    
    ifstream myFile;

    void fill_sequence_online(char (&pix_rgb)[5][64]);

    void SetLinesRead(unsigned long int  lines) {nb_lines_read=lines;}
    unsigned long int GetLinesRead() {return nb_lines_read;}
    
    unsigned long int  GetBytesRead() {return nb_bytes_read;}
    void SetBytesRead(unsigned long int  bytes) {nb_bytes_read=bytes;}
    
    void load_constellations_abacus();
    void line_extracter(char* input_string, char (&name_value)[5][64]);
    string return_constellation(unsigned long int  pix_x,unsigned long int  pix_y);

    float GetLongitude(unsigned long int  pix_x);
    float GetLatitude(unsigned long int  pix_y);
    unsigned long injection(unsigned long int which_line);
    
  private:
    
    string itsFileName;
    unsigned long int nb_lines_read;
    unsigned long int  nb_bytes_read; 
};

#endif

