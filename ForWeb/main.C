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
#include <math.h>
#include <sys/time.h>
/* For OpenGL Utility Toolkit (GLUT) */
float colorR = 0.0f;
float colorG = 0.0f;
float colorB = 0.0f;
int WindowHeight = 400;
int WindowWidth = 400;
/* ************************* */
void signalHandler(int signum);
void ReadLastLine();
void GetRGBUniverse();
/* ************************* */
/* ************************* */
/* Force the rand to be egal according to linux, Arduino, mac os what ever platform */
/* see Pseudo-random sequence generation functions */
/* in http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf for explanation */
static unsigned long int my_next = 1;
int my_rand(void);
void my_srand(unsigned int seed);
unsigned int init_start_us = 2000000;
unsigned int my_microseconds;
/* ************************* */
void Dodo();
unsigned long max_rand_mseconds = 900;
struct timeval te_beginning;
struct timeval te_current;
struct timeval deltatime;
unsigned long rand_milliseconds;
unsigned add_milli=0;
/* ************************* */
TheReaderUniverse reader_universe("onepixel.txt");
char xy_RGB[5][64];
fstream last_line_read;
bool position_defined = false;
/* ************************* */
int main(int argc, char **argv)
{
	reader_universe.load_constellations_abacus();
	ReadLastLine();
	gettimeofday(&te_beginning, NULL); // get current time
	signal(SIGINT, signalHandler);
	my_srand(12);
	while(1) {
		//Due to ms time pause in opengl this is a good trick
		Dodo();
		//usleep(my_microseconds);
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
void ReadLastLine()
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
}
/* ************************* */
void GetRGBUniverse()
{
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
	sprintf(char_microseconds,"%d",int(1000));//+rand_milliseconds));
	string replace_by[6];
		
	replace_by[0]=string(char_microseconds);
	cout << " ---->replace " << replace_by[0] << endl;
	replace_by[1]="rgb("+string(xy_RGB[2])+","+string(xy_RGB[3])+","+string(xy_RGB[4])+")";
	char nb_read[256];
	sprintf(nb_read,"%lu",n);
	replace_by[2]=to_string(n);
	//replace_by[3]=to_string(reader_universe.GetLongitude(px)) + " , " + to_string(reader_universe.GetLatitude(py));
	replace_by[3]=to_string(rand_milliseconds) + " , " + to_string(reader_universe.GetLatitude(py));
        		
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
	cout << " **** **** " << endl;
// to provide an output easely ./reader 2>toto
#if 0
replace_by[4]="Constellation name :";
cerr << "Pixel \t " << n << endl;
cerr << "Galactic coordinates: \t"<< reader_universe.GetLongitude(px) << " " << reader_universe.GetLatitude(py) << endl;
cerr << "Constellation name : \t "<< name_const << "\n" << endl;
#endif
}
/* ************************* */
void Dodo()
{
        
	gettimeofday(&te_current, NULL);

        rand_milliseconds=(unsigned int)(my_rand()*max_rand_mseconds)/32768;
        deltatime.tv_sec = 1;//rand_microseconds;
        add_milli = 500;//rand_milliseconds;//need ms and not mus
	deltatime.tv_usec = add_milli*1000;//rand_milliseconds*1000;
	//long long microseconds_beginning
	//= te_beginning.tv_sec*1000LL + te_beginning.tv_usec/1000+te_beginning.tv_usec;

        struct timeval endtime;
        timeradd(&te_current, &deltatime, &endtime);

        while (timercmp(&te_current, &endtime, <)) {
                    gettimeofday(&te_current, NULL);
        }
        te_current=te_beginning;
	cerr << rand_milliseconds  << endl;
}
/* ************************* */
/* ************************* */
int my_rand(void) // RAND_MAX assumed to be 32767
{
 my_next = my_next * 1103515245 + 12345;
 return (unsigned int)(my_next/65536) % 32768;
}
/* ************************* */
void my_srand(unsigned int seed) 
{
 my_next=seed;
}
/* ************************* */   
