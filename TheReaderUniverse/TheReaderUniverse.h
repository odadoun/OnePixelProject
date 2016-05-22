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

#include <SPI.h>
#include <SD.h>
#define nb_lines_abaccus 75

class TheReaderUniverse {
  
  struct abaccus_struct
  {
  char name_abac[32];
  long unsigned int begin_x;
  long unsigned int begin_y;
  long unsigned int end_x;
  long unsigned int end_y;
  } abacus[nb_lines_abaccus];

  public:
    TheReaderUniverse();
    ~TheReaderUniverse();
    
    File myFile;

    void fill_sequence_online(char (&pix_rgb)[5][64]);

    void SetLinesRead(long unsigned int lines) {nb_lines_read=lines;}
    long unsigned int GetLinesRead() {return nb_lines_read;}
    
    long unsigned int GetBytesRead() {return nb_bytes_read;}
    void SetBytesRead(long unsigned int bytes) {nb_bytes_read=bytes;}
    
    void load_constellations_abacus();
    void line_extracter(char* input_string, char (&name_value)[5][64]);
    String return_constellation(long unsigned int pix_x,long unsigned int pix_y);
      
  private:
    
    String itsFileName; 
    long unsigned int nb_lines_read;
    long unsigned int nb_bytes_read; 
};

#endif

