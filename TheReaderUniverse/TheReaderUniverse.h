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

class TheReaderUniverse {

  public:
    TheReaderUniverse();
    TheReaderUniverse(String file_name);
    ~TheReaderUniverse(void);
    
    bool fill_sequence_online(long int (&pix_rgb)[5]);
    
    long int GetLinesRead() {return nb_lines_read;}
    void SetLinesRead(long int lines) {nb_lines_read=lines;}
    
    long int GetBytesRead() {return nb_bytes_read;}
    void SetBytesRead(long int bytes) {nb_bytes_read=bytes;}
    

    File GetFile() {return myFile;}
    String GetFileName() {return itsFileName; }

    char *subStr(char* input_string, char *separator, int segment_number);
    
  private:
    File myFile;
    String itsFileName; 
    long int nb_lines_read;
    long int nb_bytes_read; 
};

#endif

