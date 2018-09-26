//Sebastiaan Alvarez Rodriguez - s1810979
//Brent Maas                   - s1826247
#include <stdlib.h>
#include <iostream>
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#define MAXPARTICLES 1000

float g_posX = 0.0, g_posY = 25.0, g_posZ = 50.0;
float g_orientation = 15.0; // y axis

//lighting parameters
int mflag = 0;
GLfloat dlr = 1.0, dlg = 1.0, dlb = 1.0;
GLfloat alr = 1.0, alg = 1.0, alb = 1.0;
GLfloat lx = 0.0, ly = 50.0, lz = 10.0, lw = 0.0;

//commonly used material values
const GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat default_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
const GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
const GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
const GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
const GLfloat default_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
const GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat no_shininess[] = { 0.0 };
const GLfloat low_shininess[] = { 5.0 };
const GLfloat high_shininess[] = { 100.0 };
const GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 };

int spinflag = 1;

//windmill material values
GLfloat diffWoodDarkgreen[] = {0.0, 0.35, 0.0, 1.0};
GLfloat diffWoodLightgreen[] = {0.0, 0.5, 0.0, 1.0};
GLfloat diffWoodBlack[] = {0.1, 0.1, 0.1, 1.0};
GLfloat diffWoodWhite[] = {1.0, 1.0, 1.0, 1.0};
GLfloat diffRooftiles[] = {1.0, 0.5, 0.0, 1.0};
GLfloat diffFabricWhite[] = {0.9, 0.9, 0.9, 1.0};
GLfloat diffFabricOrange[] = {0.9, 0.5, 0.1, 1.0};

const static float orig_posX = g_posX, orig_posY = g_posY, orig_posZ = g_posZ,
		orig_orientation = g_orientation, g = 9.81, G = 50;

static bool gravity_toggle = false, well_toggle = false;
struct pinfo {
	float width;
	float x, y, z;
	float v_x, v_y, v_z;
	float r, g, b;
} particles[MAXPARTICLES];

void fireCannon() {
	for (int i = 0; i < MAXPARTICLES; i++) {
		particles[i].width = 3.0 * (rand() / (float) RAND_MAX) + 1.0;
		particles[i].x = 0.0;
		particles[i].y = 0.0;
		particles[i].z = 0.0;
		particles[i].v_x = 5.0 * (rand() / (float) RAND_MAX) - 2.5;
		particles[i].v_y = 15.0 * (rand() / (float) RAND_MAX) + 10.0; // always upwards
		particles[i].v_z = 5.0 * (rand() / (float) RAND_MAX) - 2.5;
		particles[i].r = 1.0 * (rand() / (float) RAND_MAX);
		particles[i].g = 1.0 * (rand() / (float) RAND_MAX);
		particles[i].b = 1.0 * (rand() / (float) RAND_MAX);
	}
	glutGet(GLUT_ELAPSED_TIME);
}

void doExplosion() {
	const float height = 50.0 * (rand() / (float) RAND_MAX) + 10.0, r = 1.0
			* (rand() / (float) RAND_MAX), g = 1.0
			* (rand() / (float) RAND_MAX), b = 1.0
			* (rand() / (float) RAND_MAX);

	unsigned int i;
	for (i = 0; i < MAXPARTICLES; i = i + 1) {
		particles[i].width = 3.0 * (rand() / (float) RAND_MAX) + 1.0;
		particles[i].x = 0.0;
		particles[i].y = height;
		particles[i].z = 0.0;
		particles[i].v_x = 20.0 * (rand() / (float) RAND_MAX) - 10;
		particles[i].v_y = 20.0 * (rand() / (float) RAND_MAX) - 10;
		particles[i].v_z = 20.0 * (rand() / (float) RAND_MAX) - 10;
		particles[i].r = r;
		particles[i].g = g;
		particles[i].b = b;
	}
	glutGet(GLUT_ELAPSED_TIME);
}

void drawOneParticle() {
	glutSolidSphere(0.5, 30, 30);
}

void drawParticles() {
	for (int i = 0;i < MAXPARTICLES;i++) {
		glTranslatef(particles[i].x, particles[i].y, particles[i].z);
		glScalef(particles[i].width, particles[i].width, particles[i].width);
		glColor3f(particles[i].r, particles[i].g, particles[i].b);
		GLfloat diff[] = {particles[i].r, particles[i].g, particles[i].b};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
		drawOneParticle();
		glScalef(1.0 / particles[i].width, 1.0 / particles[i].width, 1.0 / particles[i].width);
		glTranslatef(-particles[i].x, -particles[i].y, -particles[i].z);
	}
	if (well_toggle) {
		glPushMatrix();
		glTranslatef(0, 50, 0);
		glColor3f(1, 1, 1);
		drawOneParticle();
		glPopMatrix();
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '4': //move the light up
		ly++;
		break;
	case 'a': //toggle ambient lighting
		if(glIsEnabled(GL_LIGHT1)) glDisable(GL_LIGHT1);
		else glEnable(GL_LIGHT1);
		break;
	case 'b': //toggle windmill blades spinning
		spinflag = !spinflag;
		break;
	case 'd': //toggle diffuse lighting
		if(glIsEnabled(GL_LIGHT0)) glDisable(GL_LIGHT0);
		else glEnable(GL_LIGHT0);
		break;
	case 'e': //move the light left
		lx--;
		break;
	case 'f': // fire
		fireCannon();
		break;
	case 'g': //gravity
		if (!well_toggle)
			gravity_toggle = !gravity_toggle;
		break;
	case 'h': // reset 
		g_posX = orig_posX, g_posY = orig_posY, g_posZ = orig_posZ, g_orientation =
				orig_orientation;
		break;
	case 'i': // move viewer forwards
		g_posX = g_posX + sin(g_orientation / 180.0 * M_PI);
		g_posZ = g_posZ - cos(g_orientation / 180.0 * M_PI);
		break;
	case 'j': // move viewer left
		g_orientation = g_orientation - 15.0;
		break;
	case 'k': // move viewer backwards
		g_posX = g_posX - sin(g_orientation / 180.0 * M_PI);
		g_posZ = g_posZ + cos(g_orientation / 180.0 * M_PI);
		break;
	case 'l': // move viewer right
		g_orientation = g_orientation + 15.0;
		break;
	case 'm': //toggle material properties
		mflag = !mflag;
		break;
	case 'q': // exit
		exit(0);
		break;
	case 'r': //move the light down
		ly--;
		break;
	/*case 'r': // explosion
		doExplosion();
		break;*/
	case 's': // move viewer up
		g_posY++;
		break;
	case 't': //move the light right
		lx++;
		break;
	case 'w': //gravity well
		well_toggle = !well_toggle;
		gravity_toggle = false;
		break;
	case 'x': //move viewer down
		g_posY--;
		break;
	}
	glutPostRedisplay();
}

void drawBox(float x, float y, float z, float width, float height, float depth){
	glBegin(GL_QUADS);
	//bottom
	glNormal3f(0, -1, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y, z);
	//top
	glNormal3f(0, 1, 0);
	glVertex3f(x, y + height, z);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y + height, z);
	//left side
	glNormal3f(-1, 0, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y, z + depth);
	//right side
	glNormal3f(1, 0, 0);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	//front
	glNormal3f(0, 0, 1);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	//back
	glNormal3f(0, 0, -1);
	glVertex3f(x, y, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y, z);
	glEnd();
}

//https://molens.hippoextranet.nl/07071954/noordholland/752DeHuisman-Zaandam-1.jpg
void drawWindmill(){
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	//Base house
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
	drawBox(-5, 0, -5, 10, 10, 10);
	//roof bits
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, 1);
	glVertex3f(5, 10, 5);
	glVertex3f(2.5, 10, 5);
	glVertex3f(2.5, 12.5, 5);
	glVertex3f(-2.5, 10, 5);
	glVertex3f(-2.5, 12.5, 5);
	glVertex3f(-5, 10, 5);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, -1);
	glVertex3f(5, 10, -5);
	glVertex3f(2.5, 10, -5);
	glVertex3f(2.5, 12.5, -5);
	glVertex3f(-2.5, 10, -5);
	glVertex3f(-2.5, 12.5, -5);
	glVertex3f(-5, 10, -5);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 0);
	glVertex3f(5, 10, 5);
	glVertex3f(5, 10, -5);
	glVertex3f(2.5, 12.5, -5);
	glVertex3f(2.5, 12.5, 5);
	glNormal3f(-1, 1, 0);
	glVertex3f(-5, 10, 5);
	glVertex3f(-5, 10, -5);
	glVertex3f(-2.5, 12.5, -5);
	glVertex3f(-2.5, 12.5, 5);
	glEnd();
	
	//Shed
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
	drawBox(-5, 0, -25, 10, 5, 20);
	//roof bits
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, -1);
	glVertex3f(-5, 5, -25);
	glVertex3f(0, 10, -25);
	glVertex3f(5, 5, -25);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 0);
	glVertex3f(5, 5, -25);
	glVertex3f(5, 5, -5);
	glVertex3f(0, 10, -5);
	glVertex3f(0, 10, -25);
	glNormal3f(-1, 1, 0);
	glVertex3f(-5, 5, -25);
	glVertex3f(-5, 5, -5);
	glVertex3f(0, 10, -5);
	glVertex3f(0, 10, -25);
	glEnd();
	
	//Walkway
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-2.5, 12.5, 5);
	glVertex3f(-2.5, 12.5, -5);
	glVertex3f(2.5, 12.5, -5);
	glVertex3f(2.5, 12.5, 5);
	glVertex3f(-5, 12.5, 2.5);
	glVertex3f(-5, 12.5, -2.5);
	glVertex3f(5, 12.5, -2.5);
	glVertex3f(5, 12.5, 2.5);
	glVertex3f(5, 12.5, 2.5);
	glVertex3f(2.5, 12.5, 5);
	glVertex3f(-5, 12.5, -2.5);
	glVertex3f(-2.5, 12.5, -5);
	glVertex3f(-5, 12.5, 2.5);
	glVertex3f(-2.5, 12.5, 5);
	glVertex3f(5, 12.5, -2.5);
	glVertex3f(2.5, 12.5, -5);
	glEnd();
	//struts
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodWhite);
	drawBox(-4.1, 10, -2.6, 0.2, 2.49, 0.2);
	drawBox(-4.1, 10, -0.1, 0.2, 2.49, 0.2);
	drawBox(-4.1, 10, 2.4, 0.2, 2.49, 0.2);
	drawBox(3.9, 10, -2.6, 0.2, 2.49, 0.2);
	drawBox(3.9, 10, -0.1, 0.2, 2.49, 0.2);
	drawBox(3.9, 10, 2.4, 0.2, 2.49, 0.2);
	//fences
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
	drawBox(2.4, 12.5, 4.9, 0.2, 1, 0.2);
	drawBox(-2.6, 12.5, 4.9, 0.2, 1, 0.2);
	drawBox(-5.1, 12.5, 2.4, 0.2, 1, 0.2);
	drawBox(-5.1, 12.5, -2.6, 0.2, 1, 0.2);
	drawBox(-2.6, 12.5, -5.1, 0.2, 1, 0.2);
	drawBox(2.4, 12.5, -5.1, 0.2, 1, 0.2);
	drawBox(4.9, 12.5, -2.6, 0.2, 1, 0.2);
	drawBox(4.9, 12.5, 2.4, 0.2, 1, 0.2);
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glVertex3f(5, 12.8, 2.5);
	glVertex3f(5, 12.8, -2.5);
	glVertex3f(5, 13, -2.5);
	glVertex3f(5, 13, 2.5);
	glNormal3f(1, 0, 1);
	glVertex3f(5, 12.8, 2.5);
	glVertex3f(2.5, 12.8, 5);
	glVertex3f(2.5, 13, 5);
	glVertex3f(5, 13, 2.5);
	glNormal3f(0, 0, 1);
	glVertex3f(2.5, 12.8, 5);
	glVertex3f(-2.5, 12.8, 5);
	glVertex3f(-2.5, 13, 5);
	glVertex3f(2.5, 13, 5);
	glNormal3f(-1, 0, 1);
	glVertex3f(-2.5, 12.8, 5);
	glVertex3f(-5, 12.8, 2.5);
	glVertex3f(-5, 13, 2.5);
	glVertex3f(-2.5, 13, 5);
	glNormal3f(-1, 0, 0);
	glVertex3f(-5, 12.8, 2.5);
	glVertex3f(-5, 12.8, -2.5);
	glVertex3f(-5, 13, -2.5);
	glVertex3f(-5, 13, 2.5);
	glNormal3f(-1, 0, -1);
	glVertex3f(-5, 12.8, -2.5);
	glVertex3f(-2.5, 12.8, -5);
	glVertex3f(-2.5, 13, -5);
	glVertex3f(-5, 13, -2.5);
	glNormal3f(0, 0, -1);
	glVertex3f(-2.5, 12.8, -5);
	glVertex3f(2.5, 12.8, -5);
	glVertex3f(2.5, 13, -5);
	glVertex3f(-2.5, 13, -5);
	glNormal3f(1, 0, -1);
	glVertex3f(2.5, 12.8, -5);
	glVertex3f(5, 12.8, -2.5);
	glVertex3f(5, 13, -2.5);
	glVertex3f(2.5, 13, -5);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodWhite);
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glVertex3f(5, 13.3, 2.5);
	glVertex3f(5, 13.3, -2.5);
	glVertex3f(5, 13.5, -2.5);
	glVertex3f(5, 13.5, 2.5);
	glNormal3f(1, 0, 1);
	glVertex3f(5, 13.3, 2.5);
	glVertex3f(2.5, 13.3, 5);
	glVertex3f(2.5, 13.5, 5);
	glVertex3f(5, 13.5, 2.5);
	glNormal3f(0, 0, 1);
	glVertex3f(2.5, 13.3, 5);
	glVertex3f(-2.5, 13.3, 5);
	glVertex3f(-2.5, 13.5, 5);
	glVertex3f(2.5, 13.5, 5);
	glNormal3f(-1, 0, 1);
	glVertex3f(-2.5, 13.3, 5);
	glVertex3f(-5, 13.3, 2.5);
	glVertex3f(-5, 13.5, 2.5);
	glVertex3f(-2.5, 13.5, 5);
	glNormal3f(-1, 0, 0);
	glVertex3f(-5, 13.3, 2.5);
	glVertex3f(-5, 13.3, -2.5);
	glVertex3f(-5, 13.5, -2.5);
	glVertex3f(-5, 13.5, 2.5);
	glNormal3f(-1, 0, -1);
	glVertex3f(-5, 13.3, -2.5);
	glVertex3f(-2.5, 13.3, -5);
	glVertex3f(-2.5, 13.5, -5);
	glVertex3f(-5, 13.5, -2.5);
	glNormal3f(0, 0, -1);
	glVertex3f(-2.5, 13.3, -5);
	glVertex3f(2.5, 13.3, -5);
	glVertex3f(2.5, 13.5, -5);
	glVertex3f(-2.5, 13.5, -5);
	glNormal3f(1, 0, -1);
	glVertex3f(2.5, 13.3, -5);
	glVertex3f(5, 13.3, -2.5);
	glVertex3f(5, 13.5, -2.5);
	glVertex3f(2.5, 13.5, -5);
	glEnd();
	
	//Windmill base
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glVertex3f(2.5, 12.5, -1.25);
	glVertex3f(2.5, 12.5, 1.25);
	glVertex3f(2.5, 15, 1.25);
	glVertex3f(2.5, 15, -1.25);
	glNormal3f(1, 0, 1);
	glVertex3f(2.5, 12.5, 1.25);
	glVertex3f(1.25, 12.5, 2.5);
	glVertex3f(1.25, 15, 2.5);
	glVertex3f(2.5, 15, 1.25);
	glNormal3f(0, 0, 1);
	glVertex3f(1.25, 12.5, 2.5);
	glVertex3f(-1.25, 12.5, 2.5);
	glVertex3f(-1.25, 15, 2.5);
	glVertex3f(1.25, 15, 2.5);
	glNormal3f(-1, 0, 1);
	glVertex3f(-2.5, 12.5, 1.25);
	glVertex3f(-1.25, 12.5, 2.5);
	glVertex3f(-1.25, 15, 2.5);
	glVertex3f(-2.5, 15, 1.25);
	glNormal3f(-1, 0, 0);
	glVertex3f(-2.5, 12.5, -1.25);
	glVertex3f(-2.5, 12.5, 1.25);
	glVertex3f(-2.5, 15, 1.25);
	glVertex3f(-2.5, 15, -1.25);
	glNormal3f(-1, 0, -1);
	glVertex3f(-1.25, 12.5, -2.5);
	glVertex3f(-2.5, 12.5, -1.25);
	glVertex3f(-2.5, 15, -1.25);
	glVertex3f(-1.25, 15, -2.5);
	glNormal3f(0, 0, -1);
	glVertex3f(1.25, 12.5, -2.5);
	glVertex3f(-1.25, 12.5, -2.5);
	glVertex3f(-1.25, 15, -2.5);
	glVertex3f(1.25, 15, -2.5);
	glNormal3f(1, 0, -1);
	glVertex3f(2.5, 12.5, -1.25);
	glVertex3f(1.25, 12.5, -2.5);
	glVertex3f(1.25, 15, -2.5);
	glVertex3f(2.5, 15, -1.25);
	glEnd();
	
	//Windmill body
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodLightgreen);
	glBegin(GL_QUADS);
	//bottom
	glNormal3f(0, -1, 0);
	glVertex3f(3, 15, 1.5);
	glVertex3f(-3, 15, 1.5);
	glVertex3f(-3, 15, -1.5);
	glVertex3f(3, 15, -1.5);
	glVertex3f(1.5, 15, 3);
	glVertex3f(1.5, 15, -3);
	glVertex3f(-1.5, 15, -3);
	glVertex3f(-1.5, 15, 3);
	glVertex3f(3, 15, 1.5);
	glVertex3f(1.5, 15, 3);
	glVertex3f(-3, 15, -1.5);
	glVertex3f(-1.5, 15, -3);
	glVertex3f(3, 15, -1.5);
	glVertex3f(1.5, 15, -3);
	glVertex3f(-3, 15, 1.5);
	glVertex3f(-1.5, 15, 3);
	
	//lower body
	glNormal3f(1, 0.5, 0);
	glVertex3f(3, 15, 1.5);
	glVertex3f(3, 15, -1.5);
	glVertex3f(2.5, 16, -1.25);
	glVertex3f(2.5, 16, 1.25);
	glNormal3f(1, 1, 1);
	glVertex3f(3, 15, 1.5);
	glVertex3f(1.5, 15, 3);
	glVertex3f(1.25, 16, 2.5);
	glVertex3f(2.5, 16, 1.25);
	glNormal3f(0, 0.5, 1);
	glVertex3f(1.5, 15, 3);
	glVertex3f(-1.5, 15, 3);
	glVertex3f(-1.25, 16, 2.5);
	glVertex3f(1.25, 16, 2.5);
	glNormal3f(-1, 1, 1);
	glVertex3f(-1.5, 15, 3);
	glVertex3f(-3, 15, 1.5);
	glVertex3f(-2.5, 16, 1.25);
	glVertex3f(-1.25, 16, 2.5);
	glNormal3f(-1, 0.5, 0);
	glVertex3f(-3, 15, 1.5);
	glVertex3f(-3, 15, -1.5);
	glVertex3f(-2.5, 16, -1.25);
	glVertex3f(-2.5, 16, 1.25);
	glNormal3f(-1, 1, -1);
	glVertex3f(-3, 15, -1.5);
	glVertex3f(-1.5, 15, -3);
	glVertex3f(-1.25, 16, -2.5);
	glVertex3f(-2.5, 16, -1.25);
	glNormal3f(0, 0.5, -1);
	glVertex3f(-1.5, 15, -3);
	glVertex3f(1.5, 15, -3);
	glVertex3f(1.25, 16, -2.5);
	glVertex3f(-1.25, 16, -2.5);
	glNormal3f(1, 1, -1);
	glVertex3f(1.5, 15, -3);
	glVertex3f(3, 15, -1.5);
	glVertex3f(2.5, 16, -1.25);
	glVertex3f(1.25, 16, -2.5);
	//middle body
	glNormal3f(1, 0.25, 0);
	glVertex3f(2.5, 16, -1.25);
	glVertex3f(2.5, 16, 1.25);
	glVertex3f(2, 18, 1);
	glVertex3f(2, 18, -1);
	glNormal3f(1, 0.5, 1);
	glVertex3f(2.5, 16, 1.25);
	glVertex3f(1.25, 16, 2.5);
	glVertex3f(1, 18, 2);
	glVertex3f(2, 18, 1);
	glNormal3f(0, 0.25, 1);
	glVertex3f(1.25, 16, 2.5);
	glVertex3f(-1.25, 16, 2.5);
	glVertex3f(-1, 18, 2);
	glVertex3f(1, 18, 2);
	glNormal3f(-1, 0.5, 1);
	glVertex3f(-1.25, 16, 2.5);
	glVertex3f(-2.5, 16, 1.25);
	glVertex3f(-2, 18, 1);
	glVertex3f(-1, 18, 2);
	glNormal3f(-1, 0.25, 0);
	glVertex3f(-2.5, 16, 1.25);
	glVertex3f(-2.5, 16, -1.25);
	glVertex3f(-2, 18, -1);
	glVertex3f(-2, 18, 1);
	glNormal3f(-1, 0.5, -1);
	glVertex3f(-2.5, 16, -1.25);
	glVertex3f(-1.25, 16, -2.5);
	glVertex3f(-1, 18, -2);
	glVertex3f(-2, 18, -1);
	glNormal3f(0, 0.25, -1);
	glVertex3f(-1.25, 16, -2.5);
	glVertex3f(1.25, 16, -2.5);
	glVertex3f(1, 18, -2);
	glVertex3f(-1, 18, -2);
	glNormal3f(1, 0.5, -1);
	glVertex3f(1.25, 16, -2.5);
	glVertex3f(2.5, 16, -1.25);
	glVertex3f(2, 18, -1);
	glVertex3f(1, 18, -2);
	//upper body
	glNormal3f(1, 0, 0);
	glVertex3f(2, 18, -1);
	glVertex3f(2, 18, 1);
	glVertex3f(2, 22, 1);
	glVertex3f(2, 22, -1);
	glNormal3f(1, 0, 1);
	glVertex3f(2, 18, 1);
	glVertex3f(1, 18, 2);
	glVertex3f(1, 22, 2);
	glVertex3f(2, 22, 1);
	glNormal3f(0, 0, 1);
	glVertex3f(1, 18, 2);
	glVertex3f(-1, 18, 2);
	glVertex3f(-1, 22, 2);
	glVertex3f(1, 22, 2);
	glNormal3f(-1, 0, 1);
	glVertex3f(-1, 18, 2);
	glVertex3f(-2, 18, 1);
	glVertex3f(-2, 22, 1);
	glVertex3f(-1, 22, 2);
	glNormal3f(-1, 0, 0);
	glVertex3f(-2, 18, 1);
	glVertex3f(-2, 18, -1);
	glVertex3f(-2, 22, -1);
	glVertex3f(-2, 22, 1);
	glNormal3f(-1, 0, -1);
	glVertex3f(-2, 18, -1);
	glVertex3f(-1, 18, -2);
	glVertex3f(-1, 22, -2);
	glVertex3f(-2, 22, -1);
	glNormal3f(0, 0, -1);
	glVertex3f(-1, 18, -2);
	glVertex3f(1, 18, -2);
	glVertex3f(1, 22, -2);
	glVertex3f(-1, 22, -2);
	glNormal3f(1, 0, -1);
	glVertex3f(1, 18, -2);
	glVertex3f(2, 18, -1);
	glVertex3f(2, 22, -1);
	glVertex3f(1, 22, -2);
	
	//Windmill cap
	//cap bottom
	glNormal3f(0, -1, 0);
	glVertex3f(2.5, 22, -1.25);
	glVertex3f(2.5, 22, 1.25);
	glVertex3f(-2.5, 22, 1.25);
	glVertex3f(-2.5, 22, -1.25);
	glVertex3f(1.25, 22, 2.5);
	glVertex3f(-1.25, 22, 2.5);
	glVertex3f(-1.25, 22, -2.5);
	glVertex3f(1.25, 22, -2.5);
	glVertex3f(2.5, 22, 1.25);
	glVertex3f(1.25, 22, 2.5);
	glVertex3f(-2.5, 22, -1.25);
	glVertex3f(-1.25, 22, -2.5);
	glVertex3f(2.5, 22, -1.25);
	glVertex3f(1.25, 22, -2.5);
	glVertex3f(-2.5, 22, 1.25);
	glVertex3f(-1.25, 22, 2.5);
	glEnd();
	//cap top
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0, 0, 1);
	glVertex3f(1.25, 22, 2.5);
	glVertex3f(-1.25, 22, 2.5);
	glNormal3f(0, 1, 0);
	glVertex3f(0, 23.5, 2);
	glVertex3f(-2.5, 22, 1.25);
	glVertex3f(0, 23, -1);
	glVertex3f(-2.5, 22, -1.25);
	glVertex3f(-1.25, 22, -2.5);
	glVertex3f(0, 23, -1);
	glVertex3f(1.25, 22, -2.5);
	glVertex3f(2.5, 22, -1.25);
	glVertex3f(0, 23, -1);
	glVertex3f(2.5, 22, 1.25);
	glVertex3f(0, 23.5, 2);
	glVertex3f(1.25, 22, 2.5);
	glEnd();
}

void update() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis
	glTranslatef(-g_posX, -g_posY, -g_posZ);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat diffuseLight[] = {dlr, dlg, dlb};
	GLfloat ambientLight[] = {alr, alg, alb};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	GLfloat lightPos[] = {lx, ly, lz, lw};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	glColor3f(1.0, 1.0, 1.0);
	// cannon base
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-5.0, 0.0, -5.0);
	glVertex3f(-5.0, 0.0, 5.0);
	glVertex3f(5.0, 0.0, 5.0);
	glVertex3f(5.0, 0.0, -5.0);
	glEnd();
	// ground plane
	glBegin(GL_LINE_STRIP);
	glVertex3f(-40.0, 0.0, -40.0);
	glVertex3f(-40.0, 0.0, 40.0);
	glVertex3f(40.0, 0.0, 40.0);
	glVertex3f(40.0, 0.0, -40.0);
	glVertex3f(-40.0, 0.0, -40.0);
	glEnd();
	
	glTranslatef(20, 0, 0);
	drawWindmill();
	glTranslatef(-20, 0, 0);
	drawParticles();
	
	glPushMatrix();
	glTranslatef(lx, ly, lz);
	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	if(mflag){
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	}else{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	}
	
	glutSwapBuffers();
}

void timer(int value) {
	unsigned int i;
	static int lastTime;
	int thisTime;
	float time;
	thisTime = glutGet(GLUT_ELAPSED_TIME);
	time = (thisTime - lastTime) / 500.0;
	lastTime = thisTime;
	for (i = 0; i < MAXPARTICLES; i = i + 1) {
		if (gravity_toggle)
			particles[i].v_y -= g * time;
		if (well_toggle) {
			float dx = -particles[i].x;
			float dy = 50 - particles[i].y;
			float dz = -particles[i].z;
			float d = sqrtf(dx * dx + dy * dy + dz * dz);
			dx /= d;
			dy /= d;
			dz /= d;
			particles[i].v_x += dx * G / (d * d);
			particles[i].v_y += dy * G / (d * d);
			particles[i].v_z += dz * G / (d * d);
		}

		particles[i].y = particles[i].y + particles[i].v_y * time;

		if (particles[i].y <= 0 && gravity_toggle) {
			particles[i].y = 0;
			particles[i].v_x = 0;
			particles[i].v_y = 0;
			particles[i].v_z = 0;
			continue;
		}
		particles[i].x = particles[i].x + particles[i].v_x * time;
		particles[i].z = particles[i].z + particles[i].v_z * time;
	}
	glutPostRedisplay();
	glutTimerFunc(50, &timer, 0);
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); //set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat) w / (GLfloat) h, 1.0, 100.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Particle Cannon");
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(120.0, 1.0, 0.001, 1000.0);
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(&update);
	glutIdleFunc(&update);
	glutReshapeFunc(&reshape);
	glutKeyboardFunc(&keyboard);
	glutTimerFunc(50, &timer, 0);
	glutMainLoop();
	return 0;
}
