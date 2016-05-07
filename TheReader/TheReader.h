/* The Reader class 
 * O. Dadoun & N. Darrot May 2016 
 * odadoun@gmail.com 
 * Loads an ascii file (located on arduino SD card) onto SDRAM 
 * file (with nb_lines) format is
 * 01010101 ... 64 times corresponding to ON/OFF leads status
 * (64 leds bar)
 * the file is loaded into memory char[8*nb_lines] is using 
 * i.e 8*8 bytes = 64 bits (xnb_lines)
*/
#ifndef _TheReader_
#define _TheReader_H_

#include <SPI.h>
#include <SD.h>

class TheReader {

  public:
    TheReader(String file_name);
    ~TheReader(void);
    
    void fill_sequence_mem();
    
    bool fill_sequence_online(long int increm,String &led_list);

    void SetBitsLineSequence(long int i,char val);
    char GetBitsLineSequence(long int i); 
    
    long int GetNbLines() {return nb_lines;}
    long int GetTotalBytes() {return total_bytes;}

    File GetFile() {return myFile;}
    String GetFileName() {return itsFileName; }
    String eight_bits_sequence(long int nb_bytes); 
  	 
  
  private:
    File myFile;
    String itsFileName; 
    char* itsLines;
    long int total_bytes;
    long int nb_lines;
};

#endif

