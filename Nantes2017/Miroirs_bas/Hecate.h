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
#define DISTANCE_WALL 3300
#define ANGLE_MIROR 30

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
      void InitAttach();

#if 0
//default order
      int init_pin[40]={3,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,23,25,27,29,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
      String init_motors[40]={"A0T","A0P","B0T","B0P","B1T","B1P","B2T","B2P","B3T","B3P","C0T","C0P","C1T","C1P","C2T","C2P","C3T","C3P","C4T","C4P",
                              "D0T","D0P", "D1T","D1P","D2T","D2P","D3T","D3P","D4T","D4P","E0T","E0P","E1T","E1P","E2T","E2P","E3T","E3P","F0T","F0P"}; //center C2
#endif
// spiral order 
       int init_pin[40]={19,20,21,23,36,37,34,35,32,33,17,18,8,9,11,12,13,14,25,27,38,39,46,47,44,45,42,43,40,41,29,31,15,16,6,7,3,5,48,49};
       String final_motors_order[40]={"C2T","C2P","C3T","C3P","D3T","D3P","D2T","D2P","D1T","D1P","C1T","C1P","B1T","B1P","B2T","B2P",
                                "B3T","B3P","C4T","C4P","D4T","D4P","E3T","E3P","E2T","E2P","E1T","E1P","E0T","E0P","D0T","D0P","C0T","C0P","B0T","B0P","A0T","A0P","F0T","F0P"};                       

  public:
      Servo *GetVoCer() {return VoCer;}  

 private:
    void SetNameConst(int _i1, String temp_name); 
    void SetStarsInConst(int _i2,int temp_i); 
    
public:
    void ExtractInfo(float (&xy)[2*NB_STARS_MAX], int nb);
    String const_name[NB_CONST];
    int nb_stars[NB_CONST];
    void TestSynchronisation();
    void MotorAtRest();
    void initMotorAngle(int brain,int theta,int phi);  
};

#endif

