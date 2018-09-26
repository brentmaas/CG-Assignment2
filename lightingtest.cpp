// lightingtest.cpp
//
// LIACS Computer Graphics
//
// This sample code is for using and exploring OpenGL Lighting and Materials
//

//#include "stdafx.h"
#include <stdlib.h>
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

//angle of rotation
GLfloat angle = 0.0;

//diffuse light color variables
GLfloat dlr = 1.0;
GLfloat dlg = 1.0;
GLfloat dlb = 1.0;

//ambient light color variables
GLfloat alr = 1.0;
GLfloat alg = 1.0;
GLfloat alb = 1.0;

//light position variables
GLfloat lx = 0.0;
GLfloat ly = 0.0;
GLfloat lz = 1.0;
GLfloat lw = 0.0;

//interface lighting flags
int aflag = 0;
int dflag = 1;
int mflag = 0;

//viewer position variables
GLfloat vx = 0.0;
GLfloat vy = 0.0;
GLfloat vz = 5.0;

//spinning flag
int spinflag = 0;

//commonly used material values
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat default_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat default_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 };

//draw the object
void myobject(void) {

//    glRotatef(angle, 1.0, 0.0, 0.0); //rotate on the x axis
//    glRotatef(angle, 0.0, 1.0, 0.0); //rotate on the y axis
//    glRotatef(angle, 0.0, 0.0, 1.0); //rotate on the z axis

//glutSolidSphere(1,60,60); //draw the object

	glTranslatef(lx, ly, lz);
	glutSolidSphere(0.1f, 10, 10);
	glTranslatef(-lx, -ly, -lz);

	const double hsqrt3 = 0.25 * sqrt(3);

	const GLfloat diff_bricks[] = { 0.5, 0.1, 0.0, 1.0 };
	const GLfloat diff_wood[] = { 0.5, 0.2, 0.0, 1.0 };
	const GLfloat diff_hay[] = { 0.6, 0.6, 0.0, 1.0 };
	const GLfloat diff_fabric[] = { 1.0, 1.0, 1.0, 1.0 };
	const GLfloat diff_greenwood[] = { 0.0, 0.3, 0.0, 1.0 };
	const GLfloat diff_whitewood[] = { 0.9, 0.9, 0.9, 1.0 };
	
	//Base floor
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff_bricks);
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0, 0, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0, 0);
	glEnd();

	//Base bottom walls
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3d(1, 0, 0);
	glVertex3d(0.5, 0, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(0.5, 0.5, 0);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.25, 0, hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.25, 0.5, hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.25, 0, hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.25, 0.5, hsqrt3);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.5, 0, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.5, 0.5, 0);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.25, 0, -hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.25, 0.5, -hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.25, 0, -hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.25, 0.5, -hsqrt3);
	glNormal3d(1, 0, 0);
	glVertex3d(0.5, 0, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(0.5, 0.5, 0);
	glEnd();

	//Base top walls
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3d(1, 0, 0);
	glVertex3d(0.5, 0.5, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(0.5, 0.75, 0);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.25, 0.5, hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.25, 0.75, hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.25, 0.5, hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.25, 0.75, hsqrt3);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.5, 0.5, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.5, 0.75, 0);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.25, 0.5, -hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.25, 0.75, -hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.25, 0.5, -hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.25, 0.75, -hsqrt3);
	glNormal3d(1, 0, 0);
	glVertex3d(0.5, 0.5, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(0.5, 0.75, 0);
	glEnd();

	//Deck floor bottom
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff_greenwood);
	glBegin(GL_QUADS);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0.49, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(1, 0.49, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0.49, 2 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0.49, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0.49, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0.49, 2 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0.49, 2 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0.49, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0.49, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0.49, 2 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-1, 0.49, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0.49, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0.49, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-1, 0.49, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0.49, -2 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0.49, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0.49, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0.49, -2 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0.49, -2 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0.49, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0.49, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0.49, -2 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(1, 0.49, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0.49, 0);
	glEnd();

	//Deck floor side
	glBegin(GL_QUADS);
	glNormal3d(1, 0, 0);
	glVertex3d(1, 0.49, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(1, 0.51, 0);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.5, 0.51, 2 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.5, 0.49, 2 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.5, 0.49, 2 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.5, 0.51, 2 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.5, 0.51, 2 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.5, 0.49, 2 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.5, 0.49, 2 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.5, 0.51, 2 * hsqrt3);
	glNormal3d(-1, 0, 0);
	glVertex3d(-1, 0.51, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-1, 0.49, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-1, 0.49, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-1, 0.51, 0);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.5, 0.51, -2 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.5, 0.49, -2 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.5, 0.49, -2 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.5, 0.51, -2 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.5, 0.51, -2 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.5, 0.49, -2 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.5, 0.49, -2 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.5, 0.51, -2 * hsqrt3);
	glNormal3d(1, 0, 0);
	glVertex3d(1, 0.51, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(1, 0.49, 0);
	glEnd();

	//Deck floor top
	glBegin(GL_QUADS);
	glNormal3d(0, 1, 0);
	glVertex3d(0.5, 0.51, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(1, 0.51, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(0.5, 0.51, 2 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0.25, 0.51, hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0.25, 0.51, hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0.5, 0.51, 2 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.5, 0.51, 2 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.25, 0.51, hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.25, 0.51, hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.5, 0.51, 2 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(-1, 0.51, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.5, 0.51, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.5, 0.51, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(-1, 0.51, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.5, 0.51, -2 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.25, 0.51, -hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.25, 0.51, -hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.5, 0.51, -2 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0.5, 0.51, -2 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0.25, 0.51, -hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0.25, 0.51, -hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0.5, 0.51, -2 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(1, 0.51, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(0.5, 0.51, 0);
	glEnd();

	//Body bottom
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff_hay);
	glBegin(GL_QUADS);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.55, 0.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.275, 0.75, 1.1 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0.75, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0.75, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.275, 0.75, 1.1 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.275, 0.75, 1.1 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0.75, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0.75, hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.275, 0.75, 1.1 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.55, 0.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.5, 0.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.55, 0.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.275, 0.75, -1.1 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0.75, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.25, 0.75, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.275, 0.75, -1.1 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.275, 0.75, -1.1 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0.75, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.25, 0.75, -hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.275, 0.75, -1.1 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.55, 0.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.5, 0.75, 0);
	glEnd();

	//Body bottom walls
	glBegin(GL_QUADS);
	glNormal3d(1, 0, 0);
	glVertex3d(0.45, 1, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(0.55, 0.75, 0);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.275, 0.75, 1.1 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.225, 1, 0.9 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.225, 1, 0.9 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.275, 0.75, 1.1 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.275, 0.75, 1.1 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.225, 1, 0.9 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.225, 1, 0.9 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.275, 0.75, 1.1 * hsqrt3);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.55, 0.75, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.45, 1, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.45, 1, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.55, 0.75, 0);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.275, 0.75, -1.1 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.225, 1, -0.9 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.225, 1, -0.9 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.275, 0.75, -1.1 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.275, 0.75, -1.1 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.225, 1, -0.9 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.225, 1, -0.9 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.275, 0.75, -1.1 * hsqrt3);
	glNormal3d(1, 0, 0);
	glVertex3d(0.55, 0.75, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(0.45, 1, 0);
	glEnd();

	//Body top walls
	glBegin(GL_QUADS);
	glNormal3d(1, 0, 0);
	glVertex3d(0.3, 1.75, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(0.45, 1, 0);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.225, 1, 0.9 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.15, 1.75, 0.6 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.15, 1.75, 0.6 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.225, 1, 0.9 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.225, 1, 0.9 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.15, 1.75, 0.6 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.15, 1.75, 0.6 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.225, 1, 0.9 * hsqrt3);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.45, 1, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.3, 1.75, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.3, 1.75, 0);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.45, 1, 0);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.225, 1, -0.9 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.15, 1.75, -0.6 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.15, 1.75, -0.6 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.225, 1, -0.9 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.225, 1, -0.9 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.15, 1.75, -0.6 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.15, 1.75, -0.6 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.225, 1, -0.9 * hsqrt3);
	glNormal3d(1, 0, 0);
	glVertex3d(0.45, 1, 0);
	glNormal3d(1, 0, 0);
	glVertex3d(0.3, 1.75, 0);
	glEnd();

	//Cap bottom
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff_greenwood);
	glBegin(GL_QUADS);
	glNormal3d(0, -1, 0);
	glVertex3d(0.3, 1.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.45, 1.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.225, 1.75, 0.9 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.15, 1.75, 0.6 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.15, 1.75, 0.6 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.225, 1.75, 0.9 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.225, 1.75, 0.9 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.15, 1.75, 0.6 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.15, 1.75, 0.6 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.225, 1.75, 0.9 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.45, 1.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.3, 1.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.3, 1.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.45, 1.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.225, 1.75, -0.9 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.15, 1.75, -0.6 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.15, 1.75, -0.6 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.225, 1.75, -0.9 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.225, 1.75, -0.9 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.15, 1.75, -0.6 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.15, 1.75, -0.6 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.225, 1.75, -0.9 * hsqrt3);
	glNormal3d(0, -1, 0);
	glVertex3d(0.45, 1.75, 0);
	glNormal3d(0, -1, 0);
	glVertex3d(0.3, 1.75, 0);
	glEnd();

	//Cap top
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.225, 1.75, 0.9 * hsqrt3);
	glNormal3d(0.25, 0, hsqrt3);
	glVertex3d(0.225, 1.75, 0.9 * hsqrt3);
	glNormal3d(0, 0, 1);
	glVertex3d(0, 2, 0.8 * hsqrt3);
	glNormal3d(1, 0, 0);
	glVertex3d(0.45, 1.75, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(0, 2, 0.8 * hsqrt3);
	glNormal3d(0.25, 0, -hsqrt3);
	glVertex3d(0.225, 1.75, -0.9 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0, 2, 0.8 * hsqrt3);
	glNormal3d(-0.25, 0, -hsqrt3);
	glVertex3d(-0.225, 1.75, -0.9 * hsqrt3);
	glNormal3d(0, 1, 0);
	glVertex3d(0, 2, 0.8 * hsqrt3);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.45, 1.75, 0);
	glNormal3d(0, 1, 0);
	glVertex3d(0, 2, 0.8 * hsqrt3);
	glNormal3d(-0.25, 0, hsqrt3);
	glVertex3d(-0.225, 1.75, 0.9 * hsqrt3);
	glEnd();

	glTranslatef(0, 1.9, 0);
	glRotatef(angle, 0, 0, 1);

	//Axis
	glBegin(GL_QUADS);
	glNormal3d(0, 1, 0.2);
	glVertex3d(0.025, 0.025, 0.2);
	glNormal3d(0, 1, 0.2);
	glVertex3d(-0.025, 0.025, 0.2);
	glNormal3d(0, 1, 0.2);
	glVertex3d(-0.025, 0.025, 0.6);
	glNormal3d(0, 1, 0.2);
	glVertex3d(0.025, 0.025, 0.6);
	glNormal3d(1, 0, 0.2);
	glVertex3d(0.025, 0.025, 0.2);
	glNormal3d(1, 0, 0.2);
	glVertex3d(0.025, -0.025, 0.2);
	glNormal3d(1, 0, 0.2);
	glVertex3d(0.025, -0.025, 0.6);
	glNormal3d(1, 0, 0.2);
	glVertex3d(0.025, 0.025, 0.6);
	glNormal3d(0, -1, 0.2);
	glVertex3d(0.025, -0.025, 0.2);
	glNormal3d(0, -1, 0.2);
	glVertex3d(-0.025, -0.025, 0.2);
	glNormal3d(0, -1, 0.2);
	glVertex3d(-0.025, -0.025, 0.6);
	glNormal3d(0, -1, 0.2);
	glVertex3d(0.025, -0.025, 0.6);
	glNormal3d(-1, 0, 0.2);
	glVertex3d(-0.025, 0.025, 0.2);
	glNormal3d(-1, 0, 0.2);
	glVertex3d(-0.025, -0.025, 0.2);
	glNormal3d(-1, 0, 0.2);
	glVertex3d(-0.025, -0.025, 0.6);
	glNormal3d(-1, 0, 0.2);
	glVertex3d(-0.025, 0.025, 0.6);
	glNormal3d(0, 0, 1);
	glVertex3d(0.025, 0.025, 0.6);
	glNormal3d(0, 0, 1);
	glVertex3d(0.025, -0.025, 0.6);
	glNormal3d(0, 0, 1);
	glVertex3d(-0.025, -0.025, 0.6);
	glNormal3d(0, 0, 1);
	glVertex3d(-0.025, 0.025, 0.6);
	glEnd();

	//Blade axes
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glVertex3d(0.025, -1, 0.55);
	glNormal3d(0, 0, 1);
	glVertex3d(0.025, 1, 0.55);
	glNormal3d(0, 0, 1);
	glVertex3d(-0.025, 1, 0.55);
	glNormal3d(0, 0, 1);
	glVertex3d(-0.025, -1, 0.55);
	glNormal3d(1, 0, 0);
	glVertex3d(0.025, 1, 0.55);
	glNormal3d(1, 0, 0);
	glVertex3d(0.025, 1, 0.5);
	glNormal3d(1, 0, 0);
	glVertex3d(0.025, -1, 0.5);
	glNormal3d(1, 0, 0);
	glVertex3d(0.025, -1, 0.55);
	glNormal3d(0, 0, -1);
	glVertex3d(0.025, -1, 0.5);
	glNormal3d(0, 0, -1);
	glVertex3d(0.025, 1, 0.5);
	glNormal3d(0, 0, -1);
	glVertex3d(-0.025, 1, 0.5);
	glNormal3d(0, 0, -1);
	glVertex3d(-0.025, -1, 0.5);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.025, 1, 0.55);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.025, 1, 0.5);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.025, -1, 0.5);
	glNormal3d(-1, 0, 0);
	glVertex3d(-0.025, -1, 0.55);
	glNormal3d(0, 1, 0);
	glVertex3d(0.025, 1, 0.5);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.025, 1, 0.5);
	glNormal3d(0, 1, 0);
	glVertex3d(-0.025, 1, 0.55);
	glNormal3d(0, 1, 0);
	glVertex3d(0.025, 1, 0.55);
	glNormal3d(0, -1, 0);
	glVertex3d(0.025, -1, 0.5);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.025, -1, 0.5);
	glNormal3d(0, -1, 0);
	glVertex3d(-0.025, -1, 0.55);
	glNormal3d(0, -1, 0);
	glVertex3d(0.025, -1, 0.55);

	glNormal3d(0, 0, 1);
	glVertex3d(-1, 0.025, 0.55);
	glNormal3d(0, 0, 1);
	glVertex3d(1, 0.025, 0.55);
	glNormal3d(0, 0, 1);
	glVertex3d(1, -0.025, 0.55);
	glNormal3d(0, 0, 1);
	glVertex3d(-1, -0.025, 0.55);
	glNormal3d(0, 1, 0);
	glVertex3d(1, 0.025, 0.5);
	glNormal3d(0, 1, 0);
	glVertex3d(-1, 0.025, 0.5);
	glNormal3d(0, 1, 0);
	glVertex3d(-1, 0.025, 0.55);
	glNormal3d(0, 1, 0);
	glVertex3d(1, 0.025, 0.55);
	glNormal3d(0, 0, -1);
	glVertex3d(-1, 0.025, 0.5);
	glNormal3d(0, 0, -1);
	glVertex3d(1, 0.025, 0.5);
	glNormal3d(0, 0, -1);
	glVertex3d(1, -0.025, 0.5);
	glNormal3d(0, 0, -1);
	glVertex3d(-1, -0.025, 0.5);
	glNormal3d(0, -1, 0);
	glVertex3d(1, -0.025, 0.5);
	glNormal3d(0, -1, 0);
	glVertex3d(-1, -0.025, 0.5);
	glNormal3d(0, -1, 0);
	glVertex3d(-1, -0.025, 0.55);
	glNormal3d(0, -1, 0);
	glVertex3d(1, -0.025, 0.55);
	glEnd();

	//Blades sheets

	glRotatef(-angle, 0, 0, 1);
	glTranslatef(0, -1.9, 0);

	glPushMatrix();
	glTranslatef(lx, ly, lz);
	if (mflag == 1) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	} else {
		glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	}

	glPopMatrix();
}

void init(void) {
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST); //enable the depth testing
	glEnable(GL_LIGHTING); //enable the lighting
	glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light
	glEnable(GL_LIGHT1); //enable LIGHT1, our Ambient Light
	glShadeModel(GL_SMOOTH); //set the shader to smooth shader

}

void update(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0); //clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
	glLoadIdentity();
	GLfloat DiffuseLight[] = { dlr, dlg, dlb }; //set DiffuseLight[] to the specified values
	GLfloat AmbientLight[] = { alr, alg, alb }; //set AmbientLight[] to the specified values
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight); //change the light accordingly
	GLfloat LightPosition[] = { lx, ly, lz, lw }; //set the LightPosition to the specified values
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
	gluLookAt(vx, vy, vz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //camera position, x,y,z, looking at x,y,z, Up Positions of the camera
	myobject(); //call the object drawing function
	glutSwapBuffers(); //swap the buffers
	angle += 0.05f; //increase the angle
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); //set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat) w / (GLfloat) h, 1.0, 100.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}

void keyboard(unsigned char key, int x, int y) {
	/*
	 if (key=='i') {
	 ly += 0.2; //move the light up
	 }
	 if (key=='k') {
	 ly -= 0.2; //move the light down
	 }
	 if (key=='j') {
	 lx -= 0.2; //move the light left
	 }
	 if (key=='l') {
	 lx += 0.2; //move the light right
	 }
	 if (key=='d') {
	 if(dflag==1){dflag=0;glDisable (GL_LIGHT0);}  //toggle diffuse lighting
	 else {dflag=1;glEnable (GL_LIGHT0);} 
	 }
	 if (key=='a') {
	 if(aflag==1){aflag=0;glDisable (GL_LIGHT1);}  //toggle ambient lighting
	 else {aflag=1;glEnable (GL_LIGHT1);} 
	 }
	 if (key=='m') {
	 if(mflag==1){mflag=0;}  //toggle material properties
	 else {mflag=1;} 
	 }*/
	switch (key) {
	case '4': //move the light up
		ly += 0.2;
		break;
	case 'a': //toggle ambient lighting
		if (glIsEnabled(GL_LIGHT1))
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		break;
	case 'b': //toggle windmill blades spinning
		if (spinflag)
			spinflag = 0;
		else
			spinflag = 1;
		break;
	case 'd': //toggle diffuse lighting
		if (glIsEnabled(GL_LIGHT0))
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0);
		break;
	case 'e': //move the light left
		lx -= 0.2;
		break;
	case 'f': //fire particle cannon
		//TODO
		break;
	case 'g': //toggle gravity
		//TODO
		break;
	case 'i': //move viewer forwards
		vz -= 0.2;
		break;
	case 'j': //move viewer left
		vx -= 0.2;
		break;
	case 'k': //move viewer backwards
		vz += 0.2;
		break;
	case 'l': //move viewer right
		vx += 0.2;
		break;
	case 'm': //toggle material properties
		if (mflag)
			mflag = 0;
		else
			mflag = 1;
		break;
	case 'r': //move the light down
		ly -= 0.2;
		break;
	case 's': //move viewer up
		vy += 0.2;
		break;
	case 't': //move the light right
		lx += 0.2;
		break;
	case 'x': //move viewer down
		vy -= 0.2;
		break;
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); //set the display to Double buffer, with depth
	glutInitWindowSize(1200, 800); //set the window size
	glutInitWindowPosition(100, 100); //set the position of the window
	glutCreateWindow("OpenGL Lighting Test");
	init(); //call the init function
	glutDisplayFunc(&update); //use the display function to draw everything
	glutIdleFunc(&update); //update any variables in display, display can be changed to anyhing, as long as you move the variables to be updated, in this case, angle++;
	glutReshapeFunc(&reshape); //reshape the window accordingly
	glutKeyboardFunc(&keyboard); //check the keyboard
	glutMainLoop(); //call the main loop
	return 0;
}
