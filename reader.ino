#include "TheReader.h"

void setup() {
 
 TheReader reader("test1.txt");
 int n=reader.GetNbLines();
 int tot_bytes=reader.GetTotalBytes();
 
 Serial.print("Nb Lines:");
 Serial.println(n);
 Serial.print("Tot bytes:");
 Serial.println(tot_bytes);


for(int i=8;i<16;i++) {Serial.print(reader.eight_bits_sequence(i)); if((i+1)%8==0)Serial.println(); }
 
}
void loop() {

}



