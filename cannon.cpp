//Workshop 1:
//	Sebastiaan Alvarez Rodriguez - s1810979
//	Brent Maas                   - s1826247
//Assignment 2:
//	Brent Maas					 - s1826247
#include <stdlib.h>
#include <iostream>
#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <functional>

const int MAXPARTICLES = 25;
const int particlesPerExplosion = 8;
int explosionCounter = 0;

//8 base house parts, 4 base house roof parts, 8 shed parts, 8 shed roof parts,
//1 walkway, 1 windmill base, 1 windmill body, 1 cap, 1 blades = 33 parts
const int numMillParts = 33;

const float targetFPS = 60.0;

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

const GLfloat white[] = {1.0, 1.0, 1.0, 1.0};

int spinflag = 1;
float angle = 0; //The rotation angle of the windmill's blades
const float dangle = 15.0; //Change in angle per second

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
	int exploded;
} particles[MAXPARTICLES + numMillParts * particlesPerExplosion];

//Class to handle drawing and falling apart of parts of the windmill
class MillPart {
public:
	MillPart(){
		x = 0;
		y = 0;
		z = 0;
		vx = 0;
		vy = 0;
		vz = 0;
		bx = 0;
		by = 0;
		bz = 0;
		bw = 0;
		bh = 0;
		bd = 0;
		exploded = false;
		explodedBy = -1;
	}
	
	MillPart(float x, float y, float z, std::function<void()> onDraw, std::function<void()> onTimer, std::function<void()> onHit,
			float bx, float by, float bz, float bw, float bh, float bd){
		this->x = x;
		this->y = y;
		this->z = z;
		vx = 0;
		vy = 0;
		vz = 0;
		this->bx = bx;
		this->by = by;
		this->bz = bz;
		this->bw = bw;
		this->bh = bh;
		this->bd = bd;
		this->onDraw = onDraw;
		this->onTimer = onTimer;
		this->onHit = onHit;
		exploded = false;
		explodedBy = -1;
	}
	
	void draw(){
		glTranslatef(x, y, z);
		onDraw();
		glTranslatef(-x, -y, -z);
	}
	
	void update(float dt){
		if(exploded){
			if(gravity_toggle) vy -= g * dt;
			if(well_toggle){
				float dx = -x;
				float dy = 50 - y;
				float dz = -z;
				float d = sqrt(dx*dx + dy*dy + dz*dz);
				dx /= d;
				dy /= d;
				dz /= d;
				vx += dx * G / (d*d);
				vy += dy * G / (d*d);
				vz += dz * G / (d*d);
			}
			onTimer();
			x += vx * dt;
			y += vy * dt;
			z += vz * dt;
			if(gravity_toggle && y <= 0){
				y = 0;
				vx = 0;
				vy = 0;
				vz = 0;
			}
		}
	}
	
	void explode(int explodedBy){
		if(!exploded){
			this->explodedBy = explodedBy;
			onHit();
			if(explodedBy < 0 || explodedBy >= MAXPARTICLES){
				vx = 2 - 4 * (rand() / (float) RAND_MAX);
				vy = 4 + 4 * (rand() / (float) RAND_MAX);
				vz = 2 - 4 * (rand() / (float) RAND_MAX);
			}else{
				vx = particles[explodedBy].v_x;
				vy = particles[explodedBy].v_y;
				vz = particles[explodedBy].v_z;
			}
		}
		exploded = true;
	}
	
	bool testBounding(float x, float y, float z){
		return x >= bx && x <= bx + bw && y >= by && y <= by + bh && z >= bz && z <= bz + bd;
	}
	
	float x, y, z, vx, vy, vz, bx, by, bz, bw, bh, bd; //Position, velocity, bounding box position, bounding box width/height/depth
	int explodedBy;
	//Event functions for drawing, timer updates, and collision events
	std::function<void()> onDraw, onTimer, onHit;
	bool exploded;
};

MillPart parts[numMillParts];

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
		particles[i].exploded = 0;
	}
}

void doExplosion() {
	const float height = 50.0 * (rand() / (float) RAND_MAX) + 10.0, r = 1.0
			* (rand() / (float) RAND_MAX), g = 1.0
			* (rand() / (float) RAND_MAX), b = 1.0
			* (rand() / (float) RAND_MAX);
	
	for (int i = 0;i < MAXPARTICLES;i++) {
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
		particles[i].exploded = 0;
	}
}

void drawOneParticle() {
	glutSolidSphere(0.5, 30, 30);
}

void drawParticles() {
	if (well_toggle) {
		glTranslatef(0, 50, 0);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
		drawOneParticle();
		glTranslatef(0, -50, 0);
	}
	//Particles that have exploded may not be rendered
	for (int i = 0;i < MAXPARTICLES + explosionCounter * particlesPerExplosion;i++) if(!particles[i].exploded){
		glTranslatef(particles[i].x, particles[i].y, particles[i].z);
		glScalef(particles[i].width, particles[i].width, particles[i].width);
		glColor3f(particles[i].r, particles[i].g, particles[i].b);
		GLfloat diff[] = {particles[i].r, particles[i].g, particles[i].b};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
		drawOneParticle();
		glScalef(1.0 / particles[i].width, 1.0 / particles[i].width, 1.0 / particles[i].width);
		glTranslatef(-particles[i].x, -particles[i].y, -particles[i].z);
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1': //test windmill explosion
		for(int i = 0;i < numMillParts;i++) parts[i].explode(-1);
		//parts[rand() % numMillParts].explode(-1);
		break;
	case '4': //move the light up
		ly++;
		break;
	case '6': //move light towards -z
		lz--;
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
	case 's': // move viewer up
		g_posY++;
		break;
	case 't': //move the light right
		lx++;
		break;
	case 'u': // explosion
		doExplosion();
		break;
	case 'w': //gravity well
		well_toggle = !well_toggle;
		gravity_toggle = false;
		break;
	case 'x': //move viewer down
		g_posY--;
		break;
	case 'y': //move light towards +z
		lz++;
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
void initWindmill(){
	//Base house parts
	parts[0] = MillPart(17.5, 0, -2.5,[](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[4].explode(parts[0].explodedBy); //Part above also explodes
	}, 15, 0, -5, 5, 5, 5);
	parts[1] = MillPart(17.5, 0, 2.5,[](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[5].explode(parts[1].explodedBy); //Part above also explodes
	}, 15, 0, 0, 5, 5, 5);
	parts[2] = MillPart(22.5, 0, 2.5,[](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[6].explode(parts[2].explodedBy); //Part above also explodes
	}, 20, 0, 0, 5, 5, 5);
	parts[3] = MillPart(22.5, 0, -2.5,[](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[7].explode(parts[3].explodedBy); //Part above also explodes
	}, 20, 0, -5, 5, 5, 5);
	parts[4] = MillPart(17.5, 5, -2.5,[](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[8].explode(parts[4].explodedBy); //Part above also explodes
	}, 15, 5, -5, 5, 5, 5);
	parts[5] = MillPart(17.5, 5, 2.5,[](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[9].explode(parts[5].explodedBy); //Part above also explodes
	}, 15, 5, 0, 5, 5, 5);
	parts[6] = MillPart(22.5, 5, 2.5,[](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[10].explode(parts[6].explodedBy); //Part above also explodes
	}, 20, 5, 0, 5, 5, 5);
	parts[7] = MillPart(22.5, 5, -2.5,[](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[11].explode(parts[7].explodedBy); //Part above also explodes
	}, 20, 5, -5, 5, 5, 5);
	
	//Base house roof
	parts[8] = MillPart(17.5, 10, -2.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(0, 0, -2.5);
		glVertex3f(0, 2.5, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 2.5, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(0, 0, 2.5);
		glVertex3f(0, 2.5, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 2.5, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(1, 0, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 2.5, 2.5);
		glVertex3f(2.5, 2.5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(-1, 1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(0, 2.5, 2.5);
		glVertex3f(0, 2.5, -2.5);
		glNormal3f(0, 1, 0);
		glVertex3f(0, 2.5, 2.5);
		glVertex3f(0, 2.5, -2.5);
		glVertex3f(2.5, 2.5, -2.5);
		glVertex3f(2.5, 2.5, 2.5);
		glEnd();
	}, [](){}, [](){
		parts[12].explode(parts[8].explodedBy); //Part above also explodes
	}, 15, 10, -5, 5, 2.5, 5);
	parts[9] = MillPart(17.5, 10, 2.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(0, 0, -2.5);
		glVertex3f(0, 2.5, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 2.5, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(0, 0, 2.5);
		glVertex3f(0, 2.5, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 2.5, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(1, 0, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 2.5, 2.5);
		glVertex3f(2.5, 2.5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(-1, 1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(0, 2.5, 2.5);
		glVertex3f(0, 2.5, -2.5);
		glNormal3f(0, 1, 0);
		glVertex3f(0, 2.5, 2.5);
		glVertex3f(0, 2.5, -2.5);
		glVertex3f(2.5, 2.5, -2.5);
		glVertex3f(2.5, 2.5, 2.5);
		glEnd();
	}, [](){}, [](){
		parts[12].explode(parts[9].explodedBy); //Part above also explodes
	}, 15, 10, 0, 5, 2.5, 5);
	parts[10] = MillPart(22.5, 10, -2.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 2.5, -2.5);
		glVertex3f(0, 0, -2.5);
		glVertex3f(0, 2.5, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 2.5, 2.5);
		glVertex3f(0, 0, 2.5);
		glVertex3f(0, 2.5, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(-1, 0, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 2.5, 2.5);
		glVertex3f(-2.5, 2.5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(0, 1, 0);
		glVertex3f(-2.5, 2.5, 2.5);
		glVertex3f(-2.5, 2.5, -2.5);
		glVertex3f(0, 2.5, -2.5);
		glVertex3f(0, 2.5, 2.5);
		glEnd();
	}, [](){}, [](){
		parts[12].explode(parts[10].explodedBy); //Part above also explodes
	}, 20, 10, -5, 5, 2.5, 5);
	parts[11] = MillPart(22.5, 10, 2.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 2.5, -2.5);
		glVertex3f(0, 0, -2.5);
		glVertex3f(0, 2.5, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 2.5, 2.5);
		glVertex3f(0, 0, 2.5);
		glVertex3f(0, 2.5, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(-1, 0, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 2.5, 2.5);
		glVertex3f(-2.5, 2.5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(0, 1, 0);
		glVertex3f(-2.5, 2.5, 2.5);
		glVertex3f(-2.5, 2.5, -2.5);
		glVertex3f(0, 2.5, -2.5);
		glVertex3f(0, 2.5, 2.5);
		glEnd();
	}, [](){}, [](){
		parts[12].explode(parts[11].explodedBy); //Part above also explodes
	}, 20, 10, 0, 5, 2.5, 5);
	
	//Walkway
	parts[12] = MillPart(20, 10.0, 0, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(-2.5, 2.51, 5);
		glVertex3f(-2.5, 2.51, -5);
		glVertex3f(2.5, 2.51, -5);
		glVertex3f(2.5, 2.51, 5);
		glVertex3f(-5, 2.51, 2.5);
		glVertex3f(-5, 2.51, -2.5);
		glVertex3f(5, 2.51, -2.5);
		glVertex3f(5, 2.51, 2.5);
		glVertex3f(5, 2.51, 2.5);
		glVertex3f(2.5, 2.51, 5);
		glVertex3f(-5, 2.51, -2.5);
		glVertex3f(-2.5, 2.51, -5);
		glVertex3f(-5, 2.51, 2.5);
		glVertex3f(-2.5, 2.51, 5);
		glVertex3f(5, 2.51, -2.5);
		glVertex3f(2.5, 2.51, -5);
		glEnd();
		//struts
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodWhite);
		drawBox(-4.1, 0, -2.6, 0.2, 2.49, 0.2);
		drawBox(-4.1, 0, -0.1, 0.2, 2.49, 0.2);
		drawBox(-4.1, 0, 2.4, 0.2, 2.49, 0.2);
		drawBox(3.9, 0, -2.6, 0.2, 2.49, 0.2);
		drawBox(3.9, 0, -0.1, 0.2, 2.49, 0.2);
		drawBox(3.9, 0, 2.4, 0.2, 2.49, 0.2);
		//fences
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		drawBox(2.4, 2.5, 4.9, 0.2, 1, 0.2);
		drawBox(-2.6, 2.5, 4.9, 0.2, 1, 0.2);
		drawBox(-5.1, 2.5, 2.4, 0.2, 1, 0.2);
		drawBox(-5.1, 2.5, -2.6, 0.2, 1, 0.2);
		drawBox(-2.6, 2.5, -5.1, 0.2, 1, 0.2);
		drawBox(2.4, 2.5, -5.1, 0.2, 1, 0.2);
		drawBox(4.9, 2.5, -2.6, 0.2, 1, 0.2);
		drawBox(4.9, 2.5, 2.4, 0.2, 1, 0.2);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(5, 2.8, 2.5);
		glVertex3f(5, 2.8, -2.5);
		glVertex3f(5, 3, -2.5);
		glVertex3f(5, 3, 2.5);
		glNormal3f(1, 0, 1);
		glVertex3f(5, 2.8, 2.5);
		glVertex3f(2.5, 2.8, 5);
		glVertex3f(2.5, 3, 5);
		glVertex3f(5, 3, 2.5);
		glNormal3f(0, 0, 1);
		glVertex3f(2.5, 2.8, 5);
		glVertex3f(-2.5, 2.8, 5);
		glVertex3f(-2.5, 3, 5);
		glVertex3f(2.5, 3, 5);
		glNormal3f(-1, 0, 1);
		glVertex3f(-2.5, 2.8, 5);
		glVertex3f(-5, 2.8, 2.5);
		glVertex3f(-5, 3, 2.5);
		glVertex3f(-2.5, 3, 5);
		glNormal3f(-1, 0, 0);
		glVertex3f(-5, 2.8, 2.5);
		glVertex3f(-5, 2.8, -2.5);
		glVertex3f(-5, 3, -2.5);
		glVertex3f(-5, 3, 2.5);
		glNormal3f(-1, 0, -1);
		glVertex3f(-5, 2.8, -2.5);
		glVertex3f(-2.5, 2.8, -5);
		glVertex3f(-2.5, 3, -5);
		glVertex3f(-5, 3, -2.5);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 2.8, -5);
		glVertex3f(2.5, 2.8, -5);
		glVertex3f(2.5, 3, -5);
		glVertex3f(-2.5, 3, -5);
		glNormal3f(1, 0, -1);
		glVertex3f(2.5, 2.8, -5);
		glVertex3f(5, 2.8, -2.5);
		glVertex3f(5, 3, -2.5);
		glVertex3f(2.5, 3, -5);
		glEnd();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodWhite);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(5, 3.3, 2.5);
		glVertex3f(5, 3.3, -2.5);
		glVertex3f(5, 3.5, -2.5);
		glVertex3f(5, 3.5, 2.5);
		glNormal3f(1, 0, 1);
		glVertex3f(5, 3.3, 2.5);
		glVertex3f(2.5, 3.3, 5);
		glVertex3f(2.5, 3.5, 5);
		glVertex3f(5, 3.5, 2.5);
		glNormal3f(0, 0, 1);
		glVertex3f(2.5, 3.3, 5);
		glVertex3f(-2.5, 3.3, 5);
		glVertex3f(-2.5, 3.5, 5);
		glVertex3f(2.5, 3.5, 5);
		glNormal3f(-1, 0, 1);
		glVertex3f(-2.5, 3.3, 5);
		glVertex3f(-5, 3.3, 2.5);
		glVertex3f(-5, 3.5, 2.5);
		glVertex3f(-2.5, 3.5, 5);
		glNormal3f(-1, 0, 0);
		glVertex3f(-5, 3.3, 2.5);
		glVertex3f(-5, 3.3, -2.5);
		glVertex3f(-5, 3.5, -2.5);
		glVertex3f(-5, 3.5, 2.5);
		glNormal3f(-1, 0, -1);
		glVertex3f(-5, 3.3, -2.5);
		glVertex3f(-2.5, 3.3, -5);
		glVertex3f(-2.5, 3.5, -5);
		glVertex3f(-5, 3.5, -2.5);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 3.3, -5);
		glVertex3f(2.5, 3.3, -5);
		glVertex3f(2.5, 3.5, -5);
		glVertex3f(-2.5, 3.5, -5);
		glNormal3f(1, 0, -1);
		glVertex3f(2.5, 3.3, -5);
		glVertex3f(5, 3.3, -2.5);
		glVertex3f(5, 3.5, -2.5);
		glVertex3f(2.5, 3.5, -5);
		glEnd();
	}, [](){}, [](){
		parts[13].explode(parts[12].explodedBy); //Part above also explodes
	}, 15, 10, -5, 10, 3.5, 10);
	
	//Windmill base
	parts[13] = MillPart(20, 12.5, 0, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodWhite);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(2.5, 0, -1.25);
		glVertex3f(2.5, 0, 1.25);
		glVertex3f(2.5, 2.5, 1.25);
		glVertex3f(2.5, 2.5, -1.25);
		glNormal3f(1, 0, 1);
		glVertex3f(2.5, 0, 1.25);
		glVertex3f(1.25, 0, 2.5);
		glVertex3f(1.25, 2.5, 2.5);
		glVertex3f(2.5, 2.5, 1.25);
		glNormal3f(0, 0, 1);
		glVertex3f(1.25, 0, 2.5);
		glVertex3f(-1.25, 0, 2.5);
		glVertex3f(-1.25, 2.5, 2.5);
		glVertex3f(1.25, 2.5, 2.5);
		glNormal3f(-1, 0, 1);
		glVertex3f(-2.5, 0, 1.25);
		glVertex3f(-1.25, 0, 2.5);
		glVertex3f(-1.25, 2.5, 2.5);
		glVertex3f(-2.5, 2.5, 1.25);
		glNormal3f(-1, 0, 0);
		glVertex3f(-2.5, 0, -1.25);
		glVertex3f(-2.5, 0, 1.25);
		glVertex3f(-2.5, 2.5, 1.25);
		glVertex3f(-2.5, 2.5, -1.25);
		glNormal3f(-1, 0, -1);
		glVertex3f(-1.25, 0, -2.5);
		glVertex3f(-2.5, 0, -1.25);
		glVertex3f(-2.5, 2.5, -1.25);
		glVertex3f(-1.25, 2.5, -2.5);
		glNormal3f(0, 0, -1);
		glVertex3f(1.25, 0, -2.5);
		glVertex3f(-1.25, 0, -2.5);
		glVertex3f(-1.25, 2.5, -2.5);
		glVertex3f(1.25, 2.5, -2.5);
		glNormal3f(1, 0, -1);
		glVertex3f(2.5, 0, -1.25);
		glVertex3f(1.25, 0, -2.5);
		glVertex3f(1.25, 2.5, -2.5);
		glVertex3f(2.5, 2.5, -1.25);
		glNormal3f(0, -1, 0);
		glVertex3f(2.5, 0, 1.25);
		glVertex3f(1.25, 0, 2.5);
		glVertex3f(-2.5, 0, -1.25);
		glVertex3f(-1.25, 0, -2.5);
		glVertex3f(2.5, 0, -1.25);
		glVertex3f(1.25, 0, -2.5);
		glVertex3f(-2.5, 0, 1.25);
		glVertex3f(-1.25, 0, 2.5);
		glVertex3f(2.5, 0, 1.25);
		glVertex3f(2.5, 0, -1.25);
		glVertex3f(-2.5, 0, -1.25);
		glVertex3f(-2.5, 0, 1.25);
		glVertex3f(1.25, 0, 2.5);
		glVertex3f(1.25, 0, -2.5);
		glVertex3f(-1.25, 0, -2.5);
		glVertex3f(-1.25, 0, 2.5);
		glNormal3f(0, 1, 0);
		glVertex3f(2.5, 2.5, 1.25);
		glVertex3f(1.25, 2.5, 2.5);
		glVertex3f(-2.5, 2.5, -1.25);
		glVertex3f(-1.25, 2.5, -2.5);
		glVertex3f(2.5, 2.5, -1.25);
		glVertex3f(1.25, 2.5, -2.5);
		glVertex3f(-2.5, 2.5, 1.25);
		glVertex3f(-1.25, 2.5, 2.5);
		glVertex3f(2.5, 2.5, 1.25);
		glVertex3f(2.5, 2.5, -1.25);
		glVertex3f(-2.5, 2.5, -1.25);
		glVertex3f(-2.5, 2.5, 1.25);
		glVertex3f(1.25, 2.5, 2.5);
		glVertex3f(1.25, 2.5, -2.5);
		glVertex3f(-1.25, 2.5, -2.5);
		glVertex3f(-1.25, 2.5, 2.5);
		glEnd();
	}, [](){}, [](){
		parts[14].explode(parts[13].explodedBy); //Part above also explodes
	}, 17.5, 12.5, -2.5, 5, 2.5, 5);
	
	//Windmill body
	parts[14] = MillPart(20, 15, 0, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodLightgreen);
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(3, 0, 1.5);
		glVertex3f(-3, 0, 1.5);
		glVertex3f(-3, 0, -1.5);
		glVertex3f(3, 0, -1.5);
		glVertex3f(1.5, 0, 3);
		glVertex3f(1.5, 0, -3);
		glVertex3f(-1.5, 0, -3);
		glVertex3f(-1.5, 0, 3);
		glVertex3f(3, 0, 1.5);
		glVertex3f(1.5, 0, 3);
		glVertex3f(-3, 0, -1.5);
		glVertex3f(-1.5, 0, -3);
		glVertex3f(3, 0, -1.5);
		glVertex3f(1.5, 0, -3);
		glVertex3f(-3, 0, 1.5);
		glVertex3f(-1.5, 0, 3);
		glNormal3f(1, 0.5, 0);
		glVertex3f(3, 0, 1.5);
		glVertex3f(3, 0, -1.5);
		glVertex3f(2.5, 1, -1.25);
		glVertex3f(2.5, 1, 1.25);
		glNormal3f(1, 1, 1);
		glVertex3f(3, 0, 1.5);
		glVertex3f(1.5, 0, 3);
		glVertex3f(1.25, 1, 2.5);
		glVertex3f(2.5, 1, 1.25);
		glNormal3f(0, 0.5, 1);
		glVertex3f(1.5, 0, 3);
		glVertex3f(-1.5, 0, 3);
		glVertex3f(-1.25, 1, 2.5);
		glVertex3f(1.25, 1, 2.5);
		glNormal3f(-1, 1, 1);
		glVertex3f(-1.5, 0, 3);
		glVertex3f(-3, 0, 1.5);
		glVertex3f(-2.5, 1, 1.25);
		glVertex3f(-1.25, 1, 2.5);
		glNormal3f(-1, 0.5, 0);
		glVertex3f(-3, 0, 1.5);
		glVertex3f(-3, 0, -1.5);
		glVertex3f(-2.5, 1, -1.25);
		glVertex3f(-2.5, 1, 1.25);
		glNormal3f(-1, 1, -1);
		glVertex3f(-3, 0, -1.5);
		glVertex3f(-1.5, 0, -3);
		glVertex3f(-1.25, 1, -2.5);
		glVertex3f(-2.5, 1, -1.25);
		glNormal3f(0, 0.5, -1);
		glVertex3f(-1.5, 0, -3);
		glVertex3f(1.5, 0, -3);
		glVertex3f(1.25, 1, -2.5);
		glVertex3f(-1.25, 1, -2.5);
		glNormal3f(1, 1, -1);
		glVertex3f(1.5, 0, -3);
		glVertex3f(3, 0, -1.5);
		glVertex3f(2.5, 1, -1.25);
		glVertex3f(1.25, 1, -2.5);
		glNormal3f(1, 0.25, 0);
		glVertex3f(2.5, 1, -1.25);
		glVertex3f(2.5, 1, 1.25);
		glVertex3f(2, 3, 1);
		glVertex3f(2, 3, -1);
		glNormal3f(1, 0.5, 1);
		glVertex3f(2.5, 1, 1.25);
		glVertex3f(1.25, 1, 2.5);
		glVertex3f(1, 3, 2);
		glVertex3f(2, 3, 1);
		glNormal3f(0, 0.25, 1);
		glVertex3f(1.25, 1, 2.5);
		glVertex3f(-1.25, 1, 2.5);
		glVertex3f(-1, 3, 2);
		glVertex3f(1, 3, 2);
		glNormal3f(-1, 0.5, 1);
		glVertex3f(-1.25, 1, 2.5);
		glVertex3f(-2.5, 1, 1.25);
		glVertex3f(-2, 3, 1);
		glVertex3f(-1, 3, 2);
		glNormal3f(-1, 0.25, 0);
		glVertex3f(-2.5, 1, 1.25);
		glVertex3f(-2.5, 1, -1.25);
		glVertex3f(-2, 3, -1);
		glVertex3f(-2, 3, 1);
		glNormal3f(-1, 0.5, -1);
		glVertex3f(-2.5, 1, -1.25);
		glVertex3f(-1.25, 1, -2.5);
		glVertex3f(-1, 3, -2);
		glVertex3f(-2, 3, -1);
		glNormal3f(0, 0.25, -1);
		glVertex3f(-1.25, 1, -2.5);
		glVertex3f(1.25, 1, -2.5);
		glVertex3f(1, 3, -2);
		glVertex3f(-1, 3, -2);
		glNormal3f(1, 0.5, -1);
		glVertex3f(1.25, 1, -2.5);
		glVertex3f(2.5, 1, -1.25);
		glVertex3f(2, 3, -1);
		glVertex3f(1, 3, -2);
		glNormal3f(1, 0, 0);
		glVertex3f(2, 3, -1);
		glVertex3f(2, 3, 1);
		glVertex3f(2, 7, 1);
		glVertex3f(2, 7, -1);
		glNormal3f(1, 0, 1);
		glVertex3f(2, 3, 1);
		glVertex3f(1, 3, 2);
		glVertex3f(1, 7, 2);
		glVertex3f(2, 7, 1);
		glNormal3f(0, 0, 1);
		glVertex3f(1, 3, 2);
		glVertex3f(-1, 3, 2);
		glVertex3f(-1, 7, 2);
		glVertex3f(1, 7, 2);
		glNormal3f(-1, 0, 1);
		glVertex3f(-1, 3, 2);
		glVertex3f(-2, 3, 1);
		glVertex3f(-2, 7, 1);
		glVertex3f(-1, 7, 2);
		glNormal3f(-1, 0, 0);
		glVertex3f(-2, 3, 1);
		glVertex3f(-2, 3, -1);
		glVertex3f(-2, 7, -1);
		glVertex3f(-2, 7, 1);
		glNormal3f(-1, 0, -1);
		glVertex3f(-2, 3, -1);
		glVertex3f(-1, 3, -2);
		glVertex3f(-1, 7, -2);
		glVertex3f(-2, 7, -1);
		glNormal3f(0, 0, -1);
		glVertex3f(-1, 3, -2);
		glVertex3f(1, 3, -2);
		glVertex3f(1, 7, -2);
		glVertex3f(-1, 7, -2);
		glNormal3f(1, 0, -1);
		glVertex3f(1, 3, -2);
		glVertex3f(2, 3, -1);
		glVertex3f(2, 7, -1);
		glVertex3f(1, 7, -2);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 7, 2);
		glVertex3f(-1, 7, 2);
		glVertex3f(-1, 7, -2);
		glVertex3f(1, 7, -2);
		glVertex3f(2, 7, 1);
		glVertex3f(2, 7, -1);
		glVertex3f(-2, 7, -1);
		glVertex3f(-2, 7, 1);
		glVertex3f(2, 7, 1);
		glVertex3f(1, 7, 2);
		glVertex3f(-2, 7, -1);
		glVertex3f(-1, 7, -2);
		glVertex3f(2, 7, -1);
		glVertex3f(1, 7, -2);
		glVertex3f(-2, 7, 1);
		glVertex3f(-1, 7, 2);
		glEnd();
	}, [](){}, [](){
		parts[15].explode(parts[14].explodedBy); //Part above also explodes
	}, 17, 15, -3, 6, 7, 6);
	
	//Windmill cap
	parts[15] = MillPart(20, 22, 0, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodLightgreen);
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(2.5, 0, -1.25);
		glVertex3f(2.5, 0, 1.25);
		glVertex3f(-2.5, 0, 1.25);
		glVertex3f(-2.5, 0, -1.25);
		glVertex3f(1.25, 0, 2.5);
		glVertex3f(-1.25, 0, 2.5);
		glVertex3f(-1.25, 0, -2.5);
		glVertex3f(1.25, 0, -2.5);
		glVertex3f(2.5, 0, 1.25);
		glVertex3f(1.25, 0, 2.5);
		glVertex3f(-2.5, 0, -1.25);
		glVertex3f(-1.25, 0, -2.5);
		glVertex3f(2.5, 0, -1.25);
		glVertex3f(1.25, 0, -2.5);
		glVertex3f(-2.5, 0, 1.25);
		glVertex3f(-1.25, 0, 2.5);
		glEnd();
		//cap top
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(1.25, 0, 2.5);
		glVertex3f(-1.25, 0, 2.5);
		glNormal3f(0, 1, 0);
		glVertex3f(0, 1.5, 2);
		glVertex3f(-2.5, 0, 1.25);
		glVertex3f(0, 1, -1);
		glVertex3f(-2.5, 0, -1.25);
		glVertex3f(-1.25, 0, -2.5);
		glVertex3f(0, 1, -1);
		glVertex3f(1.25, 0, -2.5);
		glVertex3f(2.5, 0, -1.25);
		glVertex3f(0, 1, -1);
		glVertex3f(2.5, 0, 1.25);
		glVertex3f(0, 1.5, 2);
		glVertex3f(1.25, 0, 2.5);
		glEnd();
	}, [](){}, [](){
		parts[16].explode(parts[15].explodedBy); //Part attached also explodes
	}, 17.5, 22, -2.5, 5, 1.5, 5);
	
	//Blades
	parts[16] = MillPart(20, 15.5, 0, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodDarkgreen);
		glTranslatef(0, 7.0, 0);
		glRotatef(angle, 0, 0, 1);
		//axle
		drawBox(-0.1, -0.1, 1.0, 0.2, 0.2, 3.0);
		drawBox(-0.1, -7.0, 3.8, 0.2, 14.0, 0.2);
		drawBox(-7.0, -0.1, 3.8, 14.0, 0.2, 0.2);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodWhite);
		drawBox(-0.1, -0.1, 4.0, 0.2, 0.2, 0.1);
		//sheets
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffFabricWhite);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		drawBox(0.5, -1.55, 3.85, 6.4, 1.5, 0.1);
		drawBox(-6.9, -0.05, 3.85, 6.4, 1.5, 0.1);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffFabricOrange);
		drawBox(-0.05, 0.5, 3.85, 1.5, 6.4, 0.1);
		drawBox(-1.55, -6.9, 3.85, 1.5, 6.4, 0.1);
		glRotatef(-angle, 0, 0, 1);
		glTranslatef(0, -7.0, 0);
	}, [](){
		//Roll away after hitting the ground (and ground is a thing)
		if(spinflag && gravity_toggle && parts[16].exploded && parts[16].y <= 0) parts[16].vx = -7.0 * M_PI * dangle / 180.0;
		else parts[16].vx = 0;
	}, [](){}, 13, 8.5, 3.5, 14, 14, 1);
	
	//Shed parts
	parts[17] = MillPart(17.5, 0, -22.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[25].explode(parts[17].explodedBy); //Part above also explodes
	}, 15, 0, -25, 5, 5, 5);
	parts[18] = MillPart(17.5, 0, -17.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[26].explode(parts[18].explodedBy); //Part above also explodes
	}, 15, 0, -20, 5, 5, 5);
	parts[19] = MillPart(17.5, 0, -12.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[27].explode(parts[19].explodedBy); //Part above also explodes
	}, 15, 0, -15, 5, 5, 5);
	parts[20] = MillPart(17.5, 0, -7.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[28].explode(parts[20].explodedBy); //Part above also explodes
	}, 15, 0, -10, 5, 5, 5);
	parts[21] = MillPart(22.5, 0, -22.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[29].explode(parts[21].explodedBy); //Part above also explodes
	}, 20, 0, -25, 5, 5, 5);
	parts[22] = MillPart(22.5, 0, -17.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[30].explode(parts[22].explodedBy); //Part above also explodes
	}, 20, 0, -20, 5, 5, 5);
	parts[23] = MillPart(22.5, 0, -12.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[31].explode(parts[23].explodedBy); //Part above also explodes
	}, 20, 0, -15, 5, 5, 5);
	parts[24] = MillPart(22.5, 0, -7.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		drawBox(-2.5, 0, -2.5, 5, 5, 5);
	}, [](){}, [](){
		parts[32].explode(parts[24].explodedBy); //Part above also explodes
	}, 20, 0, -10, 5, 5, 5);
	
	//Shed roof parts
	parts[25] = MillPart(17.5, 5, -22.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 5, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(1, 0, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glVertex3f(2.5, 5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(-1, 1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glVertex3f(2.5, 5, -2.5);
		glEnd();
	}, [](){}, [](){}, 15, 5, -25, 5, 5, 5);
	parts[26] = MillPart(17.5, 5, -17.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 5, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(1, 0, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glVertex3f(2.5, 5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(-1, 1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glVertex3f(2.5, 5, -2.5);
		glEnd();
	}, [](){}, [](){}, 15, 5, -20, 5, 5, 5);
	parts[27] = MillPart(17.5, 5, -12.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 5, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(1, 0, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glVertex3f(2.5, 5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(-1, 1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glVertex3f(2.5, 5, -2.5);
		glEnd();
	}, [](){}, [](){}, 15, 5, -15, 5, 5, 5);
	parts[28] = MillPart(17.5, 5, -7.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 5, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(1, 0, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glVertex3f(2.5, 5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(-1, 1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 5, 2.5);
		glVertex3f(2.5, 5, -2.5);
		glEnd();
	}, [](){}, [](){}, 15, 5, -10, 5, 5, 5);
	parts[29] = MillPart(22.5, 5, -22.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 5, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(-1, 0, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(-2.5, 5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(1, 1, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(-2.5, 5, -2.5);
		glEnd();
	}, [](){}, [](){}, 20, 5, -25, 5, 5, 5);
	parts[30] = MillPart(22.5, 5, -17.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 5, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(-1, 0, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(-2.5, 5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(1, 1, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(-2.5, 5, -2.5);
		glEnd();
	}, [](){}, [](){}, 20, 5, -20, 5, 5, 5);
	parts[31] = MillPart(22.5, 5, -12.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 5, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(-1, 0, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(-2.5, 5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(1, 1, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(-2.5, 5, -2.5);
		glEnd();
	}, [](){}, [](){}, 20, 5, -15, 5, 5, 5);
	parts[32] = MillPart(22.5, 5, -7.5, [](){
		if(mflag) glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffWoodBlack);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, -1);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 5, -2.5);
		glVertex3f(2.5, 0, -2.5);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glEnd();
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(2.5, 0, -2.5);
		glNormal3f(-1, 0, 0);
		glVertex3f(-2.5, 0, -2.5);
		glVertex3f(-2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(-2.5, 5, -2.5);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffRooftiles);
		glNormal3f(1, 1, 0);
		glVertex3f(2.5, 0, -2.5);
		glVertex3f(2.5, 0, 2.5);
		glVertex3f(-2.5, 5, 2.5);
		glVertex3f(-2.5, 5, -2.5);
		glEnd();
	}, [](){}, [](){}, 20, 5, -10, 5, 5, 5);
}

void drawWindmill(){
	for(int i = 0;i < numMillParts;i++) parts[i].draw();
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
	
	drawWindmill();
	drawParticles();
	
	glTranslatef(g_posX, g_posY, g_posZ);
	glRotatef(-g_orientation, 0.0, 1.0, 0.0);
	
	glTranslatef(lx, ly, lz);
	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	if(mflag){
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	}else{
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	}
	glTranslatef(-lx, -ly, -lz);
	
	glutSwapBuffers();
}

void timer(int value) {
	static int lastTime;
	int thisTime;
	float time;
	thisTime = glutGet(GLUT_ELAPSED_TIME);
	time = (thisTime - lastTime) / 1000.0;
	lastTime = thisTime;
	for (int i = 0; i < MAXPARTICLES + explosionCounter * particlesPerExplosion;i++) {
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
	
	//Only original particles that haven't caused an explosion yet may explode a part
	for(int i = 0;i < numMillParts;i++) if(!parts[i].exploded) for(int j = 0;j < MAXPARTICLES;j++) {
		if(!particles[j].exploded && parts[i].testBounding(particles[j].x, particles[j].y, particles[j].z)){
			parts[i].explode(j);
			for(int k = 0;k < particlesPerExplosion;k++){
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].x = particles[j].x;
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].y = particles[j].y;
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].z = particles[j].z;
				//Spread in velocities
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].v_x = particles[j].v_x * (-0.25 + 1.5 * ((float) rand() / RAND_MAX));
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].v_y = particles[j].v_y * (-0.25 + 1.5 * ((float) rand() / RAND_MAX));
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].v_z = particles[j].v_z * (-0.25 + 1.5 * ((float) rand() / RAND_MAX));
				//Make them smaller
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].width = particles[j].width / 2.0;
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].r = particles[j].r;
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].g = particles[j].g;
				particles[MAXPARTICLES + explosionCounter * particlesPerExplosion + k].b = particles[j].b;
			}
			particles[j].exploded = true;
			explosionCounter++;
		}
	}
	
	if(spinflag) angle += dangle * time;
	for(int i = 0;i < numMillParts;i++) parts[i].update(time);
	
	glutPostRedisplay();
	glutTimerFunc(1.0 / targetFPS, &timer, 0);
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); //set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat) w / (GLfloat) h, 1.0, 1000.0); //set the perspective (angle of sight, width, height, , depth)
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
	gluPerspective(120.0, 1.0, 1.0, 1000.0);
	glEnable(GL_DEPTH_TEST);
	
	initWindmill();
	
	glutDisplayFunc(&update);
	glutIdleFunc(&update);
	glutReshapeFunc(&reshape);
	glutKeyboardFunc(&keyboard);
	glutTimerFunc(1.0 / targetFPS, &timer, 0);
	glutMainLoop();
	return 0;
}
