/* The Reader for Universe scan  
 * O. Dadoun & N. Darrot May 2016 
 * odadoun@gmail.com 
 * Loads an ascii file (located on arduino SD card) onto SDRAM 
 * file (with nb_lines) format is
 * pixel_i,pixel_j,Red,Green,Blue ... RGB between 0 up to 255
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
  long int begin_x;
  long int begin_y;
  long int end_x;
  long int end_y;
  } abacus[nb_lines_abaccus];

  public:
    TheReaderUniverse();
    ~TheReaderUniverse();
    
    File GetFile() {return myFile;}
    String GetFileName() {return itsFileName; }

    void fill_sequence_online(char (&pix_rgb)[5][64]);
    
    long int GetLinesRead() {return nb_lines_read;}
    void SetLinesRead(long int lines) {nb_lines_read=lines;}
    
    long int GetBytesRead() {return nb_bytes_read;}
    void SetBytesRead(long int bytes) {nb_bytes_read=bytes;}
    
    void load_constellations_abacus();
    void line_extracter(char* input_string, char (&name_value)[5][64]);
    char *return_constellation(long int pix_x,long int pix_y);
      
  private:
    File myFile;
    String itsFileName; 
    long int nb_lines_read;
    long int nb_bytes_read; 
};

#endif

