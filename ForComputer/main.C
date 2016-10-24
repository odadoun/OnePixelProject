#include <iostream>
#include "TheReaderUniverse/TheReaderUniverse.h"
TheReaderUniverse reader_universe;
int main(int argc, char **argv)
{
	reader_universe.myFile.open("../test1.txt", std::ifstream::in);
	
	while(!reader_universe.myFile.eof())
	{
	char xy_RGB[5][64];
	reader_universe.fill_sequence_online(xy_RGB);
	
         for(int i=0;i<=4;i++) 
	   {
	      cout << xy_RGB[i] << endl; 
	   }       
	
	int n=reader_universe.GetLinesRead();
	int tot_bytes=reader_universe.GetBytesRead();

	cout << "Nb Lines:" << n+1 << endl;
	cout << "Tot bytes:" << tot_bytes << endl;
	}
	return 0;
}
