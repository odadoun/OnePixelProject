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

TheReaderUniverse reader_universe;
char xy_RGB[5][64];
fstream last_line_read;
/* For OpenGL Utility Toolkit (GLUT) */
float colorR = 0.0f;
float colorG = 0.0f;
float colorB = 0.0f;
int WindowHeight = 300;
int WindowWidth = 300;
bool position_defined = false;
void printtext(int x, int y, string String)
{
	//(x,y) is from the bottom left of the window
	glColor3f(0,0,0);
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

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	char string[64];
	sprintf(string, "something");
	printtext(10,10,string);

	glutSwapBuffers();
}

/* ************************* */
void signalHandler(int signum);
void GetRGBUniverse();
void renderScene();
void timer(int value);
/* ************************* */
int main(int argc, char **argv)
{
	reader_universe.myFile.open("test1.txt", std::ifstream::in);
	reader_universe.load_constellations_abacus();
	signal(SIGINT, signalHandler);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WindowWidth,WindowHeight);
	glutCreateWindow("One Pixel Universe");
	glutDisplayFunc(renderScene);
	glutTimerFunc( 0, timer, 0 );
	glutMainLoop();
	return 0;
}
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
		cout << " From the last line read, start @ line " << line_position <<endl;
		unsigned long int bytes_read=reader_universe.injection(line_position);	
		reader_universe.SetLinesRead(line_position);
		reader_universe.SetBytesRead(bytes_read);
		position_defined=true;
	}
	reader_universe.fill_sequence_online(xy_RGB);

	for(int i=0;i<=4;i++)
	{
		cout << " xy RGB values : " << xy_RGB[i] << endl;
	}

	int n=reader_universe.GetLinesRead();
	int tot_bytes=reader_universe.GetBytesRead();

	cout << "Nb Lines read : " << n << endl;
	cout << "Tot bytes : "     << tot_bytes << endl;
	unsigned long int px=strtoul(xy_RGB[0],NULL,0);
	unsigned long int py=strtoul(xy_RGB[1],NULL,0);
	string name_const = reader_universe.return_constellation(px,py);
	cout << "Constellation name  : " << name_const << endl;
	cout << "Galactic coordinates "   <<  reader_universe.GetLongitude(px) << " " << reader_universe.GetLatitude(py) << endl;
}
/* ************************* */
void timer(int value)
{
	//changeColor?
	GetRGBUniverse();
	colorR = atof(xy_RGB[2])/255.;
	colorG = atof(xy_RGB[3])/255.;
	colorB = atof(xy_RGB[4])/255.;
	if (colorR > 1.0)
		colorR = 0;
	if (colorG > 1.0)
		colorG = 0;
	if (colorB > 1.0)
		colorB = 0;

	glutPostRedisplay();
	int time_random;
	time_random = 10 + rand()%1000;
	cout << " Time random " << time_random << endl;
	glutTimerFunc( time_random, timer, 0 );
}
/* ************************* */   
/* ************************* */   
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(colorR, colorG, colorB);
	glRectf(-1.f,1.f, 1.f, -1.f);
	unsigned long int px=strtoul(xy_RGB[0],NULL,0);
	unsigned long int py=strtoul(xy_RGB[1],NULL,0);
	string name_const = reader_universe.return_constellation(px,py);
	cout << "Galactic coordinates "   <<  reader_universe.GetLongitude(px) << " " << reader_universe.GetLatitude(py) << endl;
        char temp[256];
	sprintf(temp,"        %f   %f",reader_universe.GetLongitude(px),reader_universe.GetLongitude(py));
	string sentence = name_const + "       "  + string(temp);
	printtext(5,10,sentence);
	glutSwapBuffers();
}
/* ************************* */
