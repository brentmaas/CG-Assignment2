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

//commonly used material values
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat default_ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat default_diffuse[] = {0.8, 0.8, 0.8, 1.0};
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

//draw the object
void myobject (void) {
	
//    glRotatef(angle, 1.0, 0.0, 0.0); //rotate on the x axis
//    glRotatef(angle, 0.0, 1.0, 0.0); //rotate on the y axis
//    glRotatef(angle, 0.0, 0.0, 1.0); //rotate on the z axis
	
    //glutSolidSphere(1,60,60); //draw the object
	
	const double hsqrt3 = 0.5 * sqrt(3);
	
	glTranslatef(0, -1, 0);
	const GLfloat diff_bricks[] = {0.5, 0.1, 0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff_bricks);
	
	//Base floor
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0.5, 0, hsqrt3);
	glVertex3d(0, 0, 0);
	glVertex3d(-0.5, 0, hsqrt3);
	glVertex3d(0, 0, 0);
	glVertex3d(-1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(-0.5, 0, -hsqrt3);
	glVertex3d(0, 0, 0);
	glVertex3d(0.5, 0, -hsqrt3);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glEnd();
	
	//Base walls
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(1, 0, 0);
	glVertex3d(1, 1, 0);
	glVertex3d(0.5, 0, hsqrt3);
	glVertex3d(0.5, 1, hsqrt3);
	glVertex3d(-0.5, 0, hsqrt3);
	glVertex3d(-0.5, 1, hsqrt3);
	glVertex3d(-1, 0, 0);
	glVertex3d(-1, 1, 0);
	glVertex3d(-0.5, 0, -hsqrt3);
	glVertex3d(-0.5, 1, -hsqrt3);
	glVertex3d(0.5, 0, -hsqrt3);
	glVertex3d(0.5, 1, -hsqrt3);
	glVertex3d(1, 0, 0);
	glVertex3d(1, 1, 0);
	glEnd();
	glTranslatef(0, 1, 0);
	
	glPushMatrix();
	glTranslatef(lx,ly,lz);
	if(mflag==1)
	{
	   glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
       glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
       glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
       glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
       glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	}
	else
	{
       glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
       glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
       glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
       glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
       glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	}

	glPopMatrix();
}

void init (void) {
    glEnable (GL_DEPTH_TEST); //enable the depth testing
    glEnable (GL_LIGHTING); //enable the lighting
    glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable (GL_LIGHT1); //enable LIGHT1, our Ambient Light
    glShadeModel (GL_SMOOTH); //set the shader to smooth shader

}

void update (void) {
    glClearColor (0.0,0.0,0.0,1.0); //clear the screen to black
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glLoadIdentity();  
    GLfloat DiffuseLight[] = {dlr, dlg, dlb}; //set DiffuseLight[] to the specified values
    GLfloat AmbientLight[] = {alr, alg, alb}; //set AmbientLight[] to the specified values
    glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
    glLightfv (GL_LIGHT1, GL_AMBIENT, AmbientLight); //change the light accordingly
    GLfloat LightPosition[] = {lx, ly, lz, lw}; //set the LightPosition to the specified values
    glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //camera position, x,y,z, looking at x,y,z, Up Positions of the camera
    myobject(); //call the object drawing function
    glutSwapBuffers(); //swap the buffers
    angle++; //increase the angle
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION); //set the matrix to projection

    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0); //set the perspective (angle of sight, width, height, , depth)
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model

}

void keyboard (unsigned char key, int x, int y) {
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
    }
}

int main (int argc, char *argv[]) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); //set the display to Double buffer, with depth
    glutInitWindowSize (500, 500); //set the window size
    glutInitWindowPosition (100, 100); //set the position of the window
    glutCreateWindow ("OpenGL Lighting Test"); 
    init (); //call the init function
    glutDisplayFunc (&update); //use the display function to draw everything
    glutIdleFunc (&update); //update any variables in display, display can be changed to anyhing, as long as you move the variables to be updated, in this case, angle++;
    glutReshapeFunc (&reshape); //reshape the window accordingly
    glutKeyboardFunc (&keyboard); //check the keyboard
    glutMainLoop (); //call the main loop
    return 0;
}
