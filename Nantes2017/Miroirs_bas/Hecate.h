/* The ReaderPlanetarium class
 * O. Dadoun May 2017
 * odadoun@gmail.com
 * read an ascii file (located on arduino SD card)
 * SD should be formated in FAT16(32) format
*/
#ifndef _Hecate_
#define _Hecate_H_

#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#define NUMBER_BRAINS 40
#define NB_STARS_MAX  25
#define NB_CONST  45
#define MAX_LINES NB_STARS_MAX*NB_CONST


class Hecate {

  public:
     Hecate();
     Hecate(String file_name);
     ~Hecate(void);
      
    String GetFileName() {return itsFileName;};
   
  private:
    File myFile;
    String itsFileName;
    void column_extracter(char string_line[256], char (&_xy)[4][64]);
 
  private:
    Servo VoCer[NUMBER_BRAINS];
    
  public:
      void InitAttach();
      int init_pin[40]={3,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,23,25,27,29,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
      Servo *GetVoCer() {return VoCer;}  

 private:
    int _i1;
    int _i2;
    void SetNameConst(int _i1, String temp_name); 
    void SetStarsInConst(int _i2,int temp_i); 

    
public:
    void ExtractInfo(float (&xy)[2*NB_STARS_MAX], int nb);
    String const_name[NB_CONST];
    int nb_stars[NB_CONST];

    
};

#endif

