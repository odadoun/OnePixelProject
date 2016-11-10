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
#include <GL/glut.h>
#include <GL/freeglut.h>
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
void timer(int value);
void printtext(int x, int y, string String);
void renderSceneLabels();
void renderScene();
/* ************************* */
/* Force the rand to be egal according to linux, Arduino, mac os what ever platform */
/* see Pseudo-random sequence generation functions */
/* in http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf for explanation */
static unsigned long int my_next = 1;
int my_rand(void);
void my_srand(unsigned int seed);
unsigned int init_start = 2000000;
unsigned int my_microseconds;
/* ************************* */
TheReaderUniverse reader_universe("onepixel.txt");
char xy_RGB[5][64];
fstream last_line_read;
bool position_defined = false;
int window_labels, window1, window2;  
/* ************************* */
int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
        my_srand(65);	
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutInitWindowPosition(WindowWidth/2, WindowHeight/2);
	//glutTimerFunc( 0, timer, 0 );
	window_labels=glutCreateWindow("One Pixel Universe labels");
	glutDisplayFunc(renderSceneLabels);

	glutInitWindowSize(WindowWidth,WindowHeight);
	glutInitWindowPosition(WindowWidth, WindowHeight);
	window1=glutCreateWindow("One Pixel Universe (1)");
	glutDisplayFunc(renderScene);
	
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutInitWindowPosition(WindowWidth*2, WindowHeight*2);
	window2=glutCreateWindow("One Pixel Universe (2)");
	glutDisplayFunc(renderScene);

	glutTimerFunc( 0, timer, 0 );
	
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

	cout << "Nb Lines read : " << n << endl;
	cout << "Tot bytes : "     << tot_bytes << endl;
	unsigned long int px=strtoul(xy_RGB[0],NULL,0);
	unsigned long int py=strtoul(xy_RGB[1],NULL,0);
	string name_const = reader_universe.return_constellation(px,py);
	cout << "Constellation name  : " << name_const << endl;
	cout << "Galactic coordinates "   <<  reader_universe.GetLongitude(px) << " " << reader_universe.GetLatitude(py) << endl;
	cout << " **** **** " << endl;

}
/* ************************* */
void timer(int value)
{

	//changeColor?
	GetRGBUniverse();
	colorR = (atoi(xy_RGB[2])/255.);
	colorG = (atoi(xy_RGB[3])/255.);
	colorB = (atoi(xy_RGB[4])/255.);
	if (colorR > 1.0)
		colorR = 0;
	if (colorG > 1.0)
		colorG = 0;
	if (colorB > 1.0)
		colorB = 0;

	glutPostRedisplay();
	int time_random;
	time_random = 10 + rand()%2000;
        
	cout << " Time random " << time_random << endl;
	reader_universe.load_constellations_abacus();
	signal(SIGINT, signalHandler);

	my_microseconds=init_start+(unsigned int)(my_rand()*init_start)/32768;	
	time_random=int(my_microseconds/1000);
	cerr << "Dodo time  " << time_random << endl;  
	glutSetWindow(window_labels);
	glutPostRedisplay();  // Update screen label
		 
	glutSetWindow(window1);
	glutPostRedisplay();  // Update screen 1
	
	glutSetWindow(window2);
	glutPostRedisplay();  // Update screen 2
	
	// Not enough precision
	glutTimerFunc( time_random, timer, 0 );
}
/* ************************* */   
void printtext(int x, int y, string String)
{
	//(x,y) is from the bottom left of the window
	glColor3f(127./255.,127./255.,127./255.);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WindowWidth, 0, WindowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2i(x,y);
	for (int i=0; i<String.size(); i++)
	{
		//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, String[i]);
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
	cout << " renderSceneLabels" << colorR << " " << colorG <<  " " << colorB << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(colorR, colorG, colorB);
	glRectf(-1.f,1.f, 1.f, -1.f);
	unsigned long int px=strtoul(xy_RGB[0],NULL,0);
	unsigned long int py=strtoul(xy_RGB[1],NULL,0);
	string name_const = reader_universe.return_constellation(px,py);
	//cout << name_const.size() << "Galactic coordinates "   <<  reader_universe.GetLongitude(px) << " " << reader_universe.GetLatitude(py) << endl;
	char temp[256];
	//empty or there is a constellation name ?
	if(name_const != "")
		sprintf(temp,"Pixel %d / Galactic coordinates \t %f° , %f°  / Constellation name : %s ",
				reader_universe.GetLinesRead(),
				reader_universe.GetLongitude(px),reader_universe.GetLongitude(py),name_const.c_str());
	else
		sprintf(temp,"Pixel %d / Galactic coordinates \t %f , %f",
				reader_universe.GetLinesRead(),
				reader_universe.GetLongitude(px),reader_universe.GetLongitude(py));
	string sentence = string(temp);
	printtext(5,10,sentence);
	glutSwapBuffers();
}
/* ************************* */   
void renderScene()
{
	cout << " renderScene" << colorR << " " << colorG <<  " " << colorB << endl;
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
void my_srand(unsigned int seed) {
	                my_next=seed;
}
/* ************************* */
