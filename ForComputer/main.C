/*
 * O. Dadoun & N. Darrot October 2016
 * odadoun@gmail.com 
 * Use GLUT to display pixel color
 */
/* Use this function glutIdleFunc instead of glutTimerFunc
 see https://mycodelog.com/2010/04/16/fps    */
#include <iostream>
#include <fstream> 
#include "TheReaderUniverse/TheReaderUniverse.h"
#include <csignal>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>
/* For OpenGL Utility Toolkit (GLUT) */
float colorR = 0.0f;
float colorG = 0.0f;
float colorB = 0.0f;
int WindowHeight = 400;
int WindowWidth = 400;
/* ************************* */
void signalHandler(int signum);
void GetRGBUniverse();
void ReadLastLine();
void idle(void);
void calculateTime();
float *complementary_color(int r,int g,int b);
void printtext(int x, int y, string String,bool legend);
void renderSceneLabels();
void renderScene();
/* ************************* */
/* Force the rand to be egal according to linux, Arduino, mac os what ever platform */
/* see Pseudo-random sequence generation functions */
/* in http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf for explanation */
static unsigned long int my_next = 1;
int my_rand(void);
void my_srand(unsigned int seed);
unsigned int max_rand_mseconds = 900;
int currentTime = 0;
int previousTime = 0;
int micro_chronos = 0;
/* ************************* */
TheReaderUniverse reader_universe("onepixel.txt");
char xy_RGB[5][64];
fstream last_line_read;
int window_labels, window;  
/* ************************* */
struct timeval te_beginning;
struct timeval te_current;
struct timeval deltatime;
unsigned int rand_milliseconds;
unsigned add_milli=0;
/* ************************* */
void Dodo();
/* ************************* */
int main(int argc, char **argv)
{
	//start to read the last line in llr
	ReadLastLine();
        gettimeofday(&te_beginning, NULL); // get current time
	// long long microseconds_beginning = 
	// te_beginning.tv_sec*1000LL + te_beginning.tv_usec/1000+te_beginning.tv_usec;
	my_srand(12);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutInitWindowPosition(WindowWidth/2, WindowHeight/2);
	window_labels=glutCreateWindow("One Pixel Universe labels");
	glutDisplayFunc(renderSceneLabels);

	glutInitWindowSize(WindowWidth,WindowHeight);
	glutInitWindowPosition(WindowWidth, WindowHeight);
	window=glutCreateWindow("One Pixel Universe");
	glutDisplayFunc(renderScene);
	
	glutIdleFunc(idle);
	//  Start GLUT event processing loop
	glutMainLoop();
	
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
	// cleanup and close up stuff here, terminate program  
	exit(signum);  
}
/* ************************* */
/* Read and/or Save last line read */
void ReadLastLine()
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

	cout << "Nb Lines read : " << n << endl;
	cout << "Tot bytes : "     << tot_bytes << endl;
	unsigned long int px=strtoul(xy_RGB[0],NULL,0);
	unsigned long int py=strtoul(xy_RGB[1],NULL,0);
	string name_const = reader_universe.return_constellation(px,py);
	cout << "Constellation name  : " << name_const << endl;
	cout << "Galactic coordinates "  <<  reader_universe.GetLongitude(px) << " "
	     << reader_universe.GetLatitude(py) << endl;
	cout << " **** ****\n" << endl;
}
/* ************************* */
void Dodo()
{
	gettimeofday(&te_current, NULL);
        rand_milliseconds=(unsigned int)(my_rand()*max_rand_mseconds)/32768;
	deltatime.tv_sec = 1;//rand_microseconds;
	add_milli = rand_milliseconds;//need ms and not mus
	deltatime.tv_usec = add_milli*1000;//rand_milliseconds*1000;
	
	struct timeval endtime;
	timeradd(&te_current, &deltatime, &endtime);
       
	while (timercmp(&te_current, &endtime, <)) {
		    gettimeofday(&te_current, NULL);
	}
	te_current=te_beginning;	
	long long milliseconds_current = te_current.tv_sec*1000LL + te_current.tv_usec/1000;
	cout << "TIME " << milliseconds_current << endl;
	calculateTime();
	cerr << rand_milliseconds  << endl;//" and " << micro_chronos << endl;
}
/* ************************* */
void idle(void)
{
	GetRGBUniverse();
	Dodo();
	colorR = (atoi(xy_RGB[2])/255.);
	colorG = (atoi(xy_RGB[3])/255.);
	colorB = (atoi(xy_RGB[4])/255.);
	if (colorR > 1.0) colorR = 0;
	if (colorG > 1.0) colorG = 0;
	if (colorB > 1.0) colorB = 0;
	
	glutPostRedisplay();
        
	reader_universe.load_constellations_abacus();
	signal(SIGINT, signalHandler);
 	
	glutSetWindow(window_labels);
	glutPostRedisplay();  // Update screen label
		 
	glutSetWindow(window);
	glutPostRedisplay();  // Update screen 1
	
}
void calculateTime()
{
    //  Get the number of milliseconds since glutInit called 
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;
    micro_chronos = timeInterval;
    previousTime = currentTime;    

  //  if(timeInterval > 1000)  previousTime = currentTime;
  //  else previousTime=1000;
    
 if(timeInterval > 1000)
    {
        micro_chronos = timeInterval;
        //  Set time
        previousTime = currentTime;
    }
}
float *complementary_color(int r,int g,int b)
{
	float *array= new float[3];
	array[0]=(255.-(float)r)/255.;
	array[1]=(255-(float)g)/255.;
	array[2]=(255-(float)b)/255.;
	return array;
}
/* ************************* */   
void printtext(int x, int y, string String,bool legend)
{
	//(x,y) is from the bottom left of the window
 	float *comp=complementary_color(atoi(xy_RGB[2]),atoi(xy_RGB[3]),atoi(xy_RGB[4]));
	glColor3f(comp[0],comp[1],comp[2]);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//glLoadIdentity();
	glOrtho(0, WindowWidth, 0, WindowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//glPushAttrib(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
	glRasterPos2i(x,y);
	for (int i=0; i<String.size(); i++)
	{
		//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
		if(legend==true)
		    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, String[i]);   	
		else
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, String[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
/* ************************* */   
void renderSceneLabels()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(colorR, colorG, colorB);
	glRectf(-1.f,1.f, 1.f, -1.f);
	unsigned long int px=strtoul(xy_RGB[0],NULL,0);
	unsigned long int py=strtoul(xy_RGB[1],NULL,0);
	string name_const = reader_universe.return_constellation(px,py);
	char text0[256];
	char text1[256];
	char text1legend[256];
	char text2[256];
	char text2legend[256];
	char text3[256];
	char text3legend[256];
	//empty or there is a constellation name ?
	
	//sprintf(text0,"%d\n",rand_milliseconds);
	//printtext(5,WindowHeight/2,string(text0));
	int position0=WindowHeight/3;
	sprintf(text1legend,"Pixel #");
	printtext(5,position0,string(text1legend),true);
	sprintf(text1,"%d\n",reader_universe.GetLinesRead());
	printtext(5,position0-20,string(text1),false);
	
	sprintf(text2legend,"Galactic coordinates");
	printtext(5,position0-40,string(text2legend),true);
	sprintf(text2,"%f , %f\n", reader_universe.GetLongitude(px),reader_universe.GetLatitude(py));
	printtext(5,position0-60,string(text2),false);
	if(name_const != "")
	{
	sprintf(text3,"%s",name_const.c_str());
	printtext(5,position0-80,string(text3),true);
	}
	glutSwapBuffers();
}
/* ************************* */   
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(colorR, colorG, colorB);
	glRectf(-1.f,1.f, 1.f, -1.f);
	glutSwapBuffers();
}
/* ************************* */
int my_rand(void) // RAND_MAX assumed to be 32767
{
 my_next = my_next * 1103515245 + 12345;
 return (unsigned int)(my_next/65536) % 32768;
}
void my_srand(unsigned int seed) 
{
 my_next=seed;
}
/* ************************* */

