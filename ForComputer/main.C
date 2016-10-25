#include <iostream>
#include "TheReaderUniverse/TheReaderUniverse.h"

#include <stdio.h>
#include <GL/glut.h>
#include <unistd.h>

TheReaderUniverse reader_universe;
char xy_RGB[5][64];

/* For OpenGL Utility Toolkit (GLUT) */
float colorR = 0.0f;
float colorG = 0.0f;
float colorB = 0.0f;
bool position_defined = false;
void GetRGBUniverse()
{
	unsigned long int bytes_read=reader_universe.injection(30000);	
	if(position_defined == false) {reader_universe.SetBytesRead(bytes_read);position_defined=true;}
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
void timer( int value )
{
	//changeColor?
	GetRGBUniverse();
	colorR = atof(xy_RGB[0])/255.;
	colorG = atof(xy_RGB[1])/255.;
	colorB = atof(xy_RGB[2])/255.;
	if (colorR > 1.0)
		colorR = 0;
	if (colorG > 1.0)
		colorG = 0;
	if (colorB > 1.0)
		colorB = 0;

	glutPostRedisplay();
	glutTimerFunc( 10, timer, 0 );
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(colorR, colorG, colorB);
	glRectf(-1.f,1.f, 1.f, -1.f);
	glutSwapBuffers();
}

/* ************************* */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(320, 320);
	glutCreateWindow("tutorial example");
	glutDisplayFunc(renderScene);
	glutTimerFunc( 0, timer, 0 );
	glutMainLoop();
	glutMainLoop();

	return 0;
}
