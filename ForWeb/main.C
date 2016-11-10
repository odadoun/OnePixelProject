/*
 * O. Dadoun & N. Darrot October 2016
 * odadoun@gmail.com 
 * Use GLUT to display pixel color
 */
#include <iostream>
#include <fstream> 
#include "TheReaderUniverse/TheReaderUniverse.h"
#include <csignal>
#include <stdio.h>
#include <unistd.h>

/* For OpenGL Utility Toolkit (GLUT) */
float colorR = 0.0f;
float colorG = 0.0f;
float colorB = 0.0f;
int WindowHeight = 400;
int WindowWidth = 400;
/* ************************* */
void signalHandler(int signum);
void GetRGBUniverse();
/* ************************* */
/* ************************* */
/* Force the rand to be egal according to linux, Arduino, mac os what ever platform
 * see http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf for explanation */
unsigned long my_rand(void);
void my_srand(unsigned long seed);
/* ************************* */
TheReaderUniverse reader_universe("onepixel.txt");
char xy_RGB[5][64];
fstream last_line_read;
bool position_defined = false;
//unsigned int microseconds;
useconds_t microseconds;
/* ************************* */
int main(int argc, char **argv)
{
	reader_universe.load_constellations_abacus();
	signal(SIGINT, signalHandler);
	my_srand(65);
	while(1) {
		microseconds = (1000+rand()%2000)*1000;
		cout << microseconds << endl;
		unsigned long  init_start = 2000000;
		unsigned long my_microseconds=init_start+(unsigned int)(my_rand()*2*init_start)/32768;
		microseconds=my_microseconds;
		cout << " my " << my_microseconds<< endl;
		usleep(my_microseconds);
		GetRGBUniverse();
	}	
		return 0;
}
/* ************************* */
/* ************************* */
/* To catch CTRL C signal*/
void signalHandler(int signum) 
{
	cout << "Interrupt signal (" << signum << ") received.\n";
	cout << "Write last line number read into llr file ..." << reader_universe.GetLinesRead() << endl;
	// Catch last line read
	last_line_read.open("llr",ios::out);
	last_line_read << reader_universe.GetLinesRead();
	last_line_read.close();
	// cleanup and close up stuff here  
	// terminate program  
	exit(signum);  
}
/* ************************* */
void GetRGBUniverse()
{
	if(position_defined == false) 
	{
		unsigned long int line_position;
		last_line_read.open("llr",ios::in);
		if (last_line_read.is_open())
		{
			last_line_read >> line_position;
			last_line_read.close();
		}
		else 
			line_position=0;
		unsigned long int bytes_read=reader_universe.injection(line_position);	
		reader_universe.SetLinesRead(line_position);
		reader_universe.SetBytesRead(bytes_read);
		cout << " From the last line read, start @ line " << line_position 
			<< " bytes already readed  " <<  reader_universe.GetBytesRead() << endl;
		position_defined=true;
	}
	reader_universe.fill_sequence_online(xy_RGB);

	for(int i=0;i<=4;i++)
	{
		cout << " xy RGB values : " << xy_RGB[i] << endl;
	}

	unsigned long int n=reader_universe.GetLinesRead();
	unsigned long int tot_bytes=reader_universe.GetBytesRead();
	unsigned long int px=strtoul(xy_RGB[0],NULL,0);
	unsigned long int py=strtoul(xy_RGB[1],NULL,0);
	string name_const = reader_universe.return_constellation(px,py);

	fstream html_template;
	ofstream html;
	html_template.open("faune.template", ios::in | ios::out );
	if (!html_template)
	{
		cerr << "Could not open " << " faune template" << endl;
		exit(1);
	}
	char temp[256];

	sprintf(temp,"(%s,%s,%s)",xy_RGB[2],xy_RGB[3],xy_RGB[4]);

	string line;
	char char_microseconds[256];
	sprintf(char_microseconds,"%d",microseconds/1000);
	string replace_by[6];	
	replace_by[0]=string(char_microseconds);
	cout << " replace " << replace_by[1] << endl;
	replace_by[1]="rgb("+string(xy_RGB[2])+","+string(xy_RGB[3])+","+string(xy_RGB[4])+")";
	char nb_read[256];
	sprintf(nb_read,"%lu",n);
	replace_by[2]=to_string(n);
	replace_by[3]=to_string(reader_universe.GetLongitude(px)) + " , " + to_string(reader_universe.GetLatitude(py));
        		
	if(name_const != "")
	{
	 replace_by[4]="Constellation name :";
	 replace_by[5]=name_const;
	}
	else
	{
	replace_by[4]="";
	replace_by[5]="";	
	}	
	string to_replace[]={"TEMPLATE_TIME","TEMPLATE_RGB","TEMPLATE_PIXEL","TEMPLATE_COORD","APPEAR_OR_NOT","TEMPLATE_LABEL"};
	html.open("faune.html");
	while (getline(html_template, line))
	{
		for(int i=0;i<6;i++)
		{
	 	size_t len = to_replace[i].length();
		size_t pos = line.find(to_replace[i]);
		if (pos != string::npos) line.replace(pos, len, replace_by[i]); 
		}
		html << line << endl;
	}
	html.close();
	cout << "Nb Lines read : " << n << endl;
	cout << "Tot bytes : "     << tot_bytes << endl;
	cout << "Constellation name  : " << name_const << endl;
	cout << "Galactic coordinates "   <<  reader_universe.GetLongitude(px) << " " << reader_universe.GetLatitude(py) << endl;

// to provide an output easely ./reader 2>toto
#if 0
replace_by[4]="Constellation name :";
cerr << "Pixel \t " << n << endl;
cerr << "Galactic coordinates: \t"<< reader_universe.GetLongitude(px) << " " << reader_universe.GetLatitude(py) << endl;
cerr << "Constellation name : \t "<< name_const << "\n" << endl;
#endif
}
/* ************************* */
/* ************************* */
static unsigned long int my_next = 1;
unsigned long my_rand(void) // RAND_MAX assumed to be 32767
{
	 my_next = my_next * 1103515245 + 12345;
	  return (unsigned long)(my_next/65536) % 32768;
}
void my_srand(unsigned long seed) {
	        my_next=seed;
}
/* ************************* */   
