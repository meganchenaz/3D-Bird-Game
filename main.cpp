#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include "RGBpixmap.h"
#include <cmath>
#include <iostream>
#include <random>
#define rad (3.1416/180)
#define EN_SIZE 20
// #include<bits/stdc++.h>
// negative pellet in y

using namespace std;

RGBpixmap pix[6];

// global variables
int TIME = 0;
const int initScore = 20;
int SCORE = initScore;
int tola[5000][5000];

// camera parameters
float zoom = 4;
float angle = 0;
float radius = 0;
float theta = 0;
float slope = 0;
float speed = 0.1;
float angleBackFrac = 0.2;
float xEye = 0.0f, yEye = 5.0f, zEye = 30.0f;
float cenX = 0, cenY = 0, cenZ = 0, roll = 0;

// axes parameters
float rotX = 0, rotY = 0, rotZ = 0;
float cosX = 0, cosY = 1, cosZ = 0;
float tX = 0, tY = 0, rX = 0, rY = 0, rZ = 4;

// flags for controlling animation & visibility
bool START = false;
bool rot = false;
bool saheedMinarVisible = false;

// RGB arrays
float r[] = { 0.1, 0.4, 0.0, 0.9, 0.2, 0.5, 0.0, 0.7, 0.5, 0.0 };
float g[] = { 0.2, 0.0, 0.4, 0.5, 0.2, 0.0, 0.3, 0.9, 0.0, 0.2 };
float b[] = { 0.4, 0.5, 0.0, 0.7, 0.9, 0.0, 0.1, 0.2, 0.5, 0.0 };

// keep track of the bird's position
double birdPosX = tX;
double birdPosY = tY;
double birdPosZ = 0.0;

// seed the random number generator with the current time
std::random_device rd;
std::mt19937 gen(rd());

// define the coordinate ranges for the random float (pellet pos)
float minX = -4;
float maxX = 4;
float minY = 0;
float maxY = 15;
float minZ = -120;
float maxZ = -20;

// create uniform distributions for the specified ranges (pellet pos)
std::uniform_real_distribution<float> distributionX(minX, maxX);
std::uniform_real_distribution<float> distributionY(minY, maxY);
std::uniform_real_distribution<float> distributionZ(minZ, maxZ);

// create uniform distribution for pellet color
std::uniform_int_distribution<int> distributionColor(0, 1);

// track Day/Night mode
bool isDayMode = true;

// create pellet struct with 7 pellets
const int pelletCount = 7;
float tZ[7] = { -8 , -20, -40, -60, -80, -100, -120 };
struct {
    float posX;
    float posY;
    float posZ;
    bool alrHit;
    int color;
} pellet[pelletCount];

/**
 * Resize the viewport when the window size changes.
 *
 * @param width The width of the window.
 * @param height The height of the window.
 */
static void resize(int width, int height) {
    // calculate the aspect ratio
    const float ar = (float)width / (float)height;

    // set the viewport to cover the entire window
    glViewport(0, 0, width, height);

    // set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // set up the perspective projection
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 1000.0);

    // set the modelview matrix
    glMatrixMode(GL_MODELVIEW);

    // reset modelview matrix to identity matrix
    glLoadIdentity();
}


/**
 * Draw shohid minar structure.
 */
void drawShohidMinar() {
    // main structure
    glColor3d(0.4, 0.2, 0.2);
    glPushMatrix();
    glTranslated(0, 1.55, 0);
    glScaled(2, 0.05, 1.5);
    glutSolidCube(1);
    glPopMatrix();

    glColor3d(0.4, 0.2, 0.2);
    glPushMatrix();
    glTranslated(0, 1.6, 0);
    glScaled(1.9, 0.05, 1.4);
    glutSolidCube(1);
    glPopMatrix();

    glColor3d(0.4, 0.2, 0.2);
    glPushMatrix();
    glTranslated(0, 1.65, 0);
    glScaled(1.8, 0.05, 1.3);
    glutSolidCube(1);
    glPopMatrix();

    // top platform
    glColor3d(1, 1, 1);
    glPushMatrix();
    glTranslated(0, 1.68, -0.4);
    glScaled(0.5, 0.02, 0.08);
    glutSolidCube(1);
    glPopMatrix();

    // main pillar
    glPushMatrix();
    glTranslated(0, 1.99, -0.4);
    glScaled(0.06, 0.7, 0.04);
    glutSolidCube(1);
    glPopMatrix();

    // vertical rods
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(0.07, 1.99, -0.4);
    glScaled(0.003, 0.7, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.11, 1.99, -0.4);
    glScaled(0.003, 0.7, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.15, 1.99, -0.4);
    glScaled(0.003, 0.7, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.22, 0, 0);
    glPushMatrix();
    glTranslated(0.07, 1.99, -0.4);
    glScaled(0.003, 0.7, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.11, 1.99, -0.4);
    glScaled(0.003, 0.7, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.15, 1.99, -0.4);
    glScaled(0.003, 0.7, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    // horizontal rod
    glPushMatrix();
    glTranslated(2.2, 0, -0.1);
    glScaled(4.2, 1, 1);
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.528, 1.85, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.02, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.18, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glColor3d(1, 1, 1);
    glPopMatrix();

    // top pillar end
    glColor3d(1, 1, 1);
    glPushMatrix();
    glTranslated(-0.22, 1.99, -0.4);
    glScaled(0.06, 0.7, 0.04);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.22, 1.99, -0.4);
    glScaled(0.06, 0.7, 0.04);
    glutSolidCube(1);
    glPopMatrix();

    // upper pillar
    glPushMatrix();
    glTranslated(0, 0.743, -1.424);
    glRotated(45, 1, 0, 0);

    glPushMatrix();
    glTranslated(0, 1.99, -0.4);
    glScaled(0.06, 0.3, 0.04);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.22, 1.99, -0.4);
    glScaled(0.06, 0.3, 0.04);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.22, 1.99, -0.4);
    glScaled(0.06, 0.3, 0.04);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 2.15, -0.4);
    glScaled(0.5, 0.04, 0.04);
    glutSolidCube(1);
    glPopMatrix();

    // vertical rod
    glColor3d(0, 0, 0);

    glPushMatrix();
    glTranslated(0.07, 1.99, -0.4);
    glScaled(0.003, 0.277, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.11, 1.99, -0.4);
    glScaled(0.003, 0.277, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.15, 1.99, -0.4);
    glScaled(0.003, 0.277, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glColor3d(0, 0, 0);

    glPushMatrix();
    glTranslated(-0.22, 0, 0);
    glPushMatrix();
    glTranslated(0.07, 1.99, -0.4);
    glScaled(0.003, 0.277, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.11, 1.99, -0.4);
    glScaled(0.003, 0.277, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.15, 1.99, -0.4);
    glScaled(0.003, 0.277, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    // horizontal rod
    glPushMatrix();
    glTranslated(2.2, 0, -0.1);
    glScaled(4.2, 1, 1);
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.528, 1.85, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.15, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glColor3d(1, 1, 1);
    glPopMatrix();

    glPopMatrix();

    // left pillar
    glColor3d(1, 1, 1);
    glPushMatrix();
    glTranslated(0.1, 0, -0.4);
    glRotated(45, 0, 1, 0);

    glPushMatrix();
    glTranslated(-0.605, 1.94, -0.3);
    glScaled(0.045, 0.65, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.45, 1.94, -0.3);
    glScaled(0.045, 0.65, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.258, -0.3);
    glScaled(0.199, 0.04, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 1.68, -0.3);
    glScaled(0.199, 0.02, 0.06);
    glutSolidCube(1);
    glPopMatrix();

    // vertical rod
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.64, -0.05, 0.1);
    glScaled(1, 1.02, 1);
    glPushMatrix();
    glTranslated(0.078, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.11, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.145, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    glColor3d(1, 1, 1);

    // horizontal rod
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.528, 1.85, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.15, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glColor3d(1, 1, 1);
    glPopMatrix();

    // left pillar
    glPushMatrix();
    glTranslated(0.65, 0, 0.3);
    glRotated(-45, 0, 1, 0);

    glPushMatrix();
    glTranslated(-0.605, 1.94, -0.3);
    glScaled(0.045, 0.65, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.45, 1.94, -0.3);
    glScaled(0.045, 0.65, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.258, -0.3);
    glScaled(0.199, 0.04, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 1.68, -0.3);
    glScaled(0.199, 0.02, 0.06);
    glutSolidCube(1);
    glPopMatrix();

    // vertical rod
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.64, -0.05, 0.1);
    glScaled(1, 1.02, 1);
    glPushMatrix();
    glTranslated(0.078, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.11, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.145, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    glColor3d(1, 1, 1);

    // horizontal rod
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.528, 1.85, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.15, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glColor3d(1, 1, 1);

    glPopMatrix();

    // bottom pillar
    glPushMatrix();
    // left pillar
    glTranslated(0.06, 0, 0.14);
    glPushMatrix();

    glTranslated(-0.2, 0, -0.31);
    glRotated(45, 0, 1, 0);

    glPushMatrix();
    glTranslated(-0.605, 1.88, -0.3);
    glScaled(0.045, 0.4, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.45, 1.88, -0.3);
    glScaled(0.045, 0.4, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.08, -0.3);
    glScaled(0.2, 0.04, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 1.68, -0.3);
    glScaled(0.199, 0.02, 0.06);
    glutSolidCube(1);
    glPopMatrix();

    // vertical rod
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.641, 0.43, 0.1);
    glScaled(1, 0.73, 1);
    glPushMatrix();
    glTranslated(0.078, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.11, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.145, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    // horizontal rod
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.528, 1.8, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 1.96, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glColor3d(1, 1, 1);
    glPopMatrix();

    // left pillar
    glPushMatrix();
    glTranslated(0.83, 0, 0.39);
    glRotated(-45, 0, 1, 0);

    glPushMatrix();
    glTranslated(-0.605, 1.88, -0.3);
    glScaled(0.045, 0.4, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.45, 1.88, -0.3);
    glScaled(0.045, 0.4, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 2.1, -0.3);
    glScaled(0.199, 0.04, 0.03);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 1.68, -0.3);
    glScaled(0.199, 0.02, 0.06);
    glutSolidCube(1);
    glPopMatrix();

    // horizontal rod
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.528, 1.8, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.528, 1.96, -0.3);
    glScaled(0.1, 0.003, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glColor3d(1, 1, 1);

    // vertical rod
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.641, 0.43, 0.1);
    glScaled(1, 0.73, 1);
    glPushMatrix();
    glTranslated(0.078, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.11, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.145, 1.99, -0.4);
    glScaled(0.003, 0.56, 0.003);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    glColor3d(1, 1, 1);
    glPopMatrix();

    glPopMatrix();

    // circles
    glColor3d(1, 0, 0);
    glPushMatrix();
    glTranslated(0, 2.1, -0.44);
    glScaled(0.35, 0.35, 0.01);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.18, 1.9, -0.45);
    glScaled(0.01, 0.5, 0.01);
    glutSolidCube(1);
    glPopMatrix();

    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(0.18, 1.9, -0.45);
    glScaled(0.01, 0.5, 0.01);
    glutSolidCube(1);
    glPopMatrix();
}


/**
 * Draw a bird.
 */
void bird() {
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double a = t * 90.0;

    float lifeColor = 1.0;
    // color factor is 0.01, if score is less than 5 (close to black)
    if (SCORE < 5) {
        lifeColor = ((float) SCORE / 20.0) + 0.01;
    }
    // color factor is 0.25, if score is between 5 and 10
    else if (SCORE < 10) {
        lifeColor = (((float)SCORE - 5.0) / 20.0) + 0.25;
    }
    // color factor is 0.5, if score is between 10 and 20
    else if (SCORE < 20) {
        lifeColor = (((float)SCORE - 10.0) / 20.0) + 0.5;
    }

    // body (white sphere)
    glColor3d(1.0 * lifeColor, 1.0 * lifeColor, 1.0 * lifeColor);
    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(0.9, 0.9, 0.9);
    glutSolidSphere(1, 30, 30);
    glPopMatrix();

    // wings (white ellipsical spheres)
    glColor3d(1.0 * lifeColor, 1.0 * lifeColor, 1.0 * lifeColor);
    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(0.6, 0.2, 1.6);
    glutSolidSphere(1, 30, 30);
    glPopMatrix();

    // beak (yellow cone)
    glColor3d(1.0 * lifeColor, 1.0 * lifeColor, 0.0);
    glPushMatrix();
    glTranslated(0.85, 0, 0);
    glRotated(90, 0, 1, 0);
    glScaled(0.3, 0.3, 0.2);
    glutSolidCone(0.5, 1.5, 30, 30);
    glPopMatrix();

    // eyes (black spheres)
    glColor3d(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslated(0.85, 0.3, 0.3);
    glutSolidSphere(0.1, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.85, 0.3, -0.3);
    glutSolidSphere(0.1, 30, 30);
    glPopMatrix();

    // comb (red partial spheres)
    glColor3d(1.0 * lifeColor, 0.0, 0.0);
    glPushMatrix();
    glTranslated(0.4, 0.8, 0);
    glRotated(90, 0, 1, 0);
    glScaled(0.5, 0.5, 0.5);
    glutSolidSphere(0.5, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 1, 0);
    glRotated(90, 0, 1, 0);
    glScaled(0.5, 0.5, 0.5);
    glutSolidSphere(0.5, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.4, 0.8, 0);
    glRotated(90, 0, 1, 0);
    glScaled(0.5, 0.5, 0.5);
    glutSolidSphere(0.5, 30, 30);
    glPopMatrix();
}



/**
 * Draw a house, composed of one main cube and four smaller
 * cubes that represent windows and doors.
 *
 * @param R The red color component.
 * @param G The green color component.
 * @param B The blue color component.
 */
void singleTolaHouse(int R, int G, int B) {
    // set color based on the input RGB components
    glColor3d(r[R % 11], g[G % 11], b[B % 11]);

    // draw body of the house
    glPushMatrix();
    glTranslated(0, 0, 0);
    glutSolidCube(1);
    glPopMatrix();

    // draw the right window/door
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(0.2, 0, 0);
    glScaled(0.3, 0.3, 1.001);
    glutSolidCube(1);
    glPopMatrix();

    // draw the left window/door
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.2, 0, 0);
    glScaled(0.3, 0.3, 1.001);
    glutSolidCube(1);
    glPopMatrix();

    // draw the top window/door
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(0, 0, 0.2);
    glScaled(1.001, 0.3, 0.3);
    glutSolidCube(1);
    glPopMatrix();

    // draw the bottom window/door
    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(0, 0, -0.2);
    glScaled(1.001, 0.3, 0.3);
    glutSolidCube(1);
    glPopMatrix();
}


/**
 * Draw a vertically stacked structure of houses.
 *
 * @param n The number of houses to be stacked.
 * @param R The red color component of the house.
 * @param G The green color component of the house.
 */
void house(int n, int R, int G) {
    for (int i = 0; i < n; i++) {
        glPushMatrix();
        glTranslated(0, 0.8 + i, 0);
        singleTolaHouse(G, R, i);
        glPopMatrix();
    }
}


/**
 * Draw a scene with a ground and 2 shohid minar structures.
 */
void shohidMinarEnv() {
    // draw the ground
    glColor3d(0, 0.5, 0.1);
    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(EN_SIZE * 2, 0.3, EN_SIZE * 2);
    glutSolidCube(1);
    glPopMatrix();

    // draw the first shohid minar
    glPushMatrix();
    glTranslated(-8, -2.7, -5);
    glRotated(65, 0, 1, 0);
    glScaled(2, 2, 2);
    drawShohidMinar();
    glPopMatrix();

    // draw the second shohid minar
    glPushMatrix();
    glTranslated(8, -2.7, -5);
    glRotated(-65, 0, 1, 0);
    glScaled(2, 2, 2);
    drawShohidMinar();
    glPopMatrix();
}


/**
 * Draw the complete environment.
 *
 * @param n The index of the x and y positions for the rings to be drawn.
 */
void environment(int n) {
    // draw the ground
    glColor3d(0, 0.5, 0.1);                 // dark green
    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(EN_SIZE * 2, 0.3, EN_SIZE * 2);
    glutSolidCube(1);
    glPopMatrix();

    // draw the pellets
    float colorFact = 1.0;
    if (pellet[n].alrHit) {
        colorFact = 0.3;                    // already-hit pellet should be dimmer
    }

    // check if the pellet is green
    if (pellet[n].color == 0) {
        glColor3d(0, 1 * colorFact, 0.1);   // green pellet
    }
    else {
        glColor3d(1 * colorFact, 0, 0.1);   // red pellet
    }

    glPushMatrix();
    glTranslated(pellet[n].posX, pellet[n].posY + 1, 0);
    glScaled(0.3, 0.3, 0.3);
    glutSolidSphere(2, 50, 50);
    glPopMatrix();

    // draw the houses
    for (int i = -(EN_SIZE / 2) + 1; i < (EN_SIZE / 2); i += 2) {
        for (int j = -(EN_SIZE / 2) + 1; j < (EN_SIZE / 2); j += 2) {
            // if house specifications exist for the current location
            if (tola[i + (EN_SIZE / 2) + 1][j + (EN_SIZE / 2) + 1] != 0) {
                // draw the specified house
                glPushMatrix();
                glTranslated(i, 0, j);
                house(tola[i + (EN_SIZE / 2) + 1][j + (EN_SIZE / 2) + 1], i, j);
                glPopMatrix();
            }

            // middle section (no houses)
            else if (i >= -5 && i <= 5) {
                ;
            }

            // if no house specifications exist for the current location
            else {
                // pick a random type of house
                tola[i + (EN_SIZE / 2) + 1][j + (EN_SIZE / 2) + 1] = (rand() % 5) + 1;

                // draw the house
                glPushMatrix();
                glTranslated(i, 0, j);
                house(tola[i + (EN_SIZE / 2) + 1][j + (EN_SIZE / 2) + 1], i, j);
                glPopMatrix();
            }
        }
    }
}


/**
 * Check for a collision between the bird and the given pellet.
 *
 * @param pelletX the x-coordinate of the pellet's position.
 * @param pelletY the y-coordinate of the pellet's position.
 * @param pelletZ the z-coordinate of the pellet's position.
 */
int checkCollision(double pelletX, double pelletY, double pelletZ) {
    // calculate the distance between the bird and the pellet
    double xDiff = (birdPosX + pelletX) * 1;
    double yDiff = (birdPosY + pelletY) * 1;
    double zDiff = (birdPosZ + pelletZ) * 1;
    double distance = sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));

    // check if the distance is less than 1 (the radius of the bird)
    if (distance < 1) {
        return 1;  // collision
    }
    else {
        return 0;  // no collision
    }
}


/**
 * Render the 3D scene.
 */
void draw() {
    // get the elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    // calculate the rotation angle based on time
    double a = t * 90.0;

    // update the global time variable with the elapsed time
    TIME = t;

    // draw the bird
    if (rotX > 11) rotX = 11;
    if (rotX < -11) rotX = -11;
    if (rotZ > 10) rotZ = 10;
    if (rotZ < -15) rotZ = -15;

    glPushMatrix();
    glTranslated(0, 1, 0);
    glRotated(90, 0, 1, 0);
    glRotated(5, 0, 0, 1);
    glRotated(rotX, 1, 0, 0);
    glRotated(rotY, 0, 1, 0);
    glRotated(rotZ, 0, 0, 1);
    glScaled(0.4, 0.4, 0.4);
    bird();
    glPopMatrix();

    // keep the bird within range and decrement the score, if the bird 
    // crashes into buildings or the ground
    if (tX >= 4.1) {
        tX = 4.1;       // left side
        SCORE--;      
    }

    if (tX <= -4.1) {
        tX = -4.1;      // right side
        SCORE--;
    }

    if (tY > 0.1) {
        tY = 0;         // ground
        SCORE--;
    }

    if (tY < -15) {
        tY = -15;
    }

    // iterate through the pellets
    for (int i = 0; i < pelletCount; i++) {
        // draw the environments
        glPushMatrix();
        glTranslated(tX, tY, tZ[i]);
        if (i == 1) {
            shohidMinarEnv();
        }
        else {
            environment(i);
        }
        glPopMatrix();

        // update the z pos of the environments for animation
        tZ[i] += speed;

        // ensure that the environments wrap around when reaching a
        // certain z pos, to create a continous animation effect
        if (tZ[i] >= 20) {
            pellet[i].posX = distributionX(gen);
            pellet[i].posY = distributionY(gen);
            pellet[i].posZ = distributionZ(gen);
            pellet[i].color = distributionColor(gen);
            tZ[i] = -110;
            pellet[i].alrHit = false;
        }

        // update the z pos of the pellets for collision check
        pellet[i].posZ = tZ[i];

        // check for collisions
        if (!pellet[i].alrHit && checkCollision(pellet[i].posX, pellet[i].posY, pellet[i].posZ)) {
            // indicate that the pellet has been hit
            pellet[i].alrHit = true;

            // check if the pellet is green (add 2 points 
            // if it's green & remove 1 point if it's red)
            if (pellet[i].color == 0) {
                SCORE += 2;                 // green 
            }
            else {
                SCORE--;                    // red 
            }
        }
        
        // the pellet falls if it's been hit
        if (pellet[i].alrHit == true) {
            pellet[i].posY -= 0.075;
        }
    }

    // keep track of the bird's position
    birdPosX = tX;
    birdPosY = tY;
    birdPosZ = 0.0;

    // adjust the rotation angles to simulate a gradual
    // return to the original position
    if (rotX > 0) rotX -= angleBackFrac;
    if (rotX < 0) rotX += angleBackFrac;
    if (rotY > 0) rotY -= angleBackFrac;
    if (rotY < 0) rotY += angleBackFrac;
    if (rotZ > 0) rotZ -= angleBackFrac;
    if (rotZ < 0) rotZ += angleBackFrac;

    /*
    // increase the animation speed and limit it to
    // a max value (currently set to 0.7)
    speed += 0.0001;
    if( speed >= 0.7) speed = 0.7;
    */
}


/**
 * Render text using OpenGl bitmap fonts.
 *
 * @param *str The input string to be rendered.
 * @param x, y, z The 3D coordinates where the text will be positioned.
 */
void drawBitmapText(char* str, float x, float y, float z) {
    char* c;

    // set the position for 3D rasterization
    glRasterPos3f(x, y + 8, z);

    // set font thickness
    glLineWidth(2);

    // loop through each character in the input string
    for (c = str; *c != '\0'; c++) {
        // use GLUT to render each character in Times Roman, size 10 font
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
    }
}


/**
 * Render text using OpenGL stroke fonts (0.002 scale).
 *
 * @param str The input string to be rendered.
 * @param x, y, z The 3D coordinates where the text will be positioned.
 */
void drawStrokeText(char* str, int x, int y, int z) {
    char* c;

    // push the current matrix onto the matrix stack
    glPushMatrix();

    // set font color based on Day/Night Mode
    if (isDayMode) {
        glColor3f(0, 0, 0);         // black for Day Mode    
    }
    else {
        glColor3f(255, 255, 255);   // white for Night Mode
    }

    // translate to the specified position
    glTranslatef(x, y + 8, z);

    // scale the text
    glScalef(0.002f, 0.002f, z);

    // set font thickness
    glLineWidth(2);

    // loop through each character in the input string
    for (c = str; *c != '\0'; c++) {
        // use GLUT to render each character in stroke fonts
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }

    // pop the matrix stack, restoring the previous matrix
    glPopMatrix();
}


/**
 * Render text using OpenGL stroke fonts (0.005 scale).
 *
 * @param str The input string to be rendered.
 * @param x, y, z The 3D coordinates where the text will be positioned.
 */
void drawStrokeText2(char* str, int x, int y, int z) {
    char* c;

    // push the current matrix onto the matrix stack
    glPushMatrix();

    // set font color based on Day/Night Mode
    if (isDayMode) {
        glColor3f(0, 0, 0);             // black for Day Mode
    }
    else {
        glColor3f(255, 255, 255);       // white for Night Mode
    }

    // translate to a specified position
    glTranslatef(x, y + 8, z);

    // scale the text
    glScalef(0.005f, 0.005f, z);

    // set font thickness
    glLineWidth(2);

    // loop through each character in the input string
    for (c = str; *c != '\0'; c++) {
        // use GLUT to render each character in stroke fonts
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }

    // pop the matrix stack, restoring the previous matrix
    glPopMatrix();
}


/**
 * Draw a stroke at a specifed position.
 *
 * @param c The character to be drawn.
 * @param x, y, z The 3D coordinates where the character will be positioned.
 */
void drawStrokeChar(char c, float x, float y, float z) {
    glPushMatrix();

    // set font color based on Day/Night Mode
    if (isDayMode) {
        glColor3f(0, 0, 0);             // black for Day Mode
    }
    else {  
        glColor3f(255, 255, 255);       // white for Night Mode
    }

    // set font thickness
    glLineWidth(2);

    glTranslatef(x, y + 8, z);
    glScalef(0.002f, 0.002f, z);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    glPopMatrix();
}


/**
 * Render the scene and UI based on user interactions.
 */
static void display(void) {
    // get the elapsed time in seconds
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    // calculate the rotation angle based on the elapsed time
    double a = t * 90.0;
    double aa = a;

    if (!rot) {
        a = 0;
    }

    // clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set up the camera (identity & viewing matrix)
    glLoadIdentity();
    gluLookAt(0.0, 4.5, 10.0,
        0, 4, 0,
        0, 1.0f, 0.0f);

    // render the scene
    if (START) {
        // draw the scene elements and apply transformations
        glPushMatrix();
        glTranslated(0, 0, 0);
        glScaled(zoom, zoom, zoom);
        glRotated(a, 0, 1, 0);
        draw();
        glPopMatrix();

        // render text 
        drawStrokeText(const_cast<char*>("UP: W, DOWN: S, LEFT: A, RIGHT: D, MAIN MENU: M"), -7, 0.9, 0);  // user instructions
        drawStrokeText(const_cast<char*>("TIME : "), 1, 0, 0);                                             // time display
        drawStrokeText(const_cast<char*>("SCORE : "), 3, 0, 0);                                            // score display

        // extract elapsed time digits
        int mod, digCount = 0, number = 0;
        do {
            // store the digits in reverse order
            mod = TIME % 10;
            number = number * 10 + mod;
            TIME /= 10;
            digCount++;
        } while (TIME);

        // render the time digits
        float tmp = 0;
        while (digCount--) {
            // convert digit to ASCII and render them
            mod = number % 10;
            drawStrokeChar(mod + 48, 2 + tmp, 0, 0);
            number /= 10;
            tmp += 0.2;
        }

        // end the game, if the score is negative
        if (SCORE < 0) {
            START = false;
        }
        else {
            // extract score digits
            int score = 0, tempScore = SCORE;
            digCount = 0;
            do {
                // store the digits in reverse order
                mod = tempScore % 10;
                score = score * 10 + mod;
                tempScore /= 10;
                digCount++;
            } while (tempScore);

            // render the score digits
            tmp = 0;
            while (digCount--) {
                // convert digit to ASCII and render them
                mod = score % 10;
                drawStrokeChar(mod + 48, 4.5 + tmp, 0, 0);
                score /= 10;
                tmp += 0.2;
            }
        }
    }
    else {
        // draw the rotating bird
        glPushMatrix();
        glTranslated(0, 3, 0);
        glRotated(aa, 0, 1, 0);
        glScaled(1.5, 1.5, 1.5);
        bird();
        glPopMatrix();

        // render text for start menu
        if (SCORE < 0) {
            // "Game Over" menu
            drawStrokeText2(const_cast<char*>("GAME OVER"), -2, 0, 0);
            drawStrokeText(const_cast<char*>("Press 1 to play again in Day Mode."), -2, -1, 0);
            drawStrokeText(const_cast<char*>("Press 2 to play again in Night Mode"), -2, -2, 0);
        }
        else {
            // initial start menu
            drawStrokeText2(const_cast<char*>("Bird Game"), -2, 0, 0);
            drawStrokeText(const_cast<char*>("Press 1 to play in Day Mode."), -2, -1, 0);
            drawStrokeText(const_cast<char*>("Press 2 to play in Night Mode."), -2, -2, 0);
        }
    }

    // swap the front and back buffers to display the rendered image
    glutSwapBuffers();
}


/**
 * Callback function for different keyboard keys.
 *
 * @param key ASCII value of the pressed key.
 * @param x, y The mouse's cursor position during the key press.
 */
static void key(unsigned char key, int x, int y) {
    float frac = 0.5;
    float rotFrac = 1;

    switch (key)
    {
    case 27:

    // exit the program
    case 'q':
        exit(0);
        break;

    // turn on scene rotation
    case 'r':
        rot = true;
        break;

    // turn off scene rotation
    case 't':
        rot = false;
        break;

    // zoom in
    case 'z':
        zoom += 0.05;
        break;

    // zoom out
    case 'Z':
        zoom -= 0.05;

    // move the character up
    case 'w':
        tY -= frac;
        rotZ += rotFrac;
        break;

    // move the character down
    case 's':
        tY += frac;
        rotZ -= rotFrac;
        break;

    // move the character left
    case 'a':
        tX += frac;
        rotX -= rotFrac * 3;
        rotY += rotFrac / 2;
        break;

    // move the character right
    case 'd':
        tX -= frac;
        rotX += rotFrac * 3;
        rotY -= rotFrac / 2;
        break;

    // switch to the game in Day Mode
    case '1':
        START = true;
        isDayMode = true;
        SCORE = initScore;
        tX = 0;
        tY = 0;
        glClearColor(1, 1, 1, 1);
        break;

    // switch to the game in Night Mode
    case '2':
        START = true;
        isDayMode = false;
        SCORE = initScore;
        tX = 0;
        tY = 0;
        glClearColor(0.1, 0.2, 0.3, 1);
        break;

    // switch to the start menu
    case 'm':
        START = false;
        break;
    }

    // update the display based on the key press
    glutPostRedisplay();
}


/**
 * Ensure that the scene is continuously rendered.
 */
static void idle(void) {
    glutPostRedisplay();
}


// lighting parameters
const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

// material parameters
const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };



/* PROGRAM ENTRY POINT */

int main(int argc, char* argv[]) {
    // create pellets in random positions
    for (int i = 0; i < pelletCount; i++) {
        pellet[i].posX = distributionX(gen);
        pellet[i].posY = distributionY(gen);
        pellet[i].posZ = distributionZ(gen);
        pellet[i].color = distributionColor(gen);
        pellet[i].alrHit = false;
    }

    // initialize GLUT
    glutInit(&argc, argv);

    // set initial window position and size
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1366, 720);

    // set display mode with RGB, double buffering, depth buffer, and RGBA color
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    // create a window with the "GLUT Shapes" title
    glutCreateWindow("CS155 Final Project, Megan Chen & Julia Hsing");

    // register callback functions
    glutReshapeFunc(resize);    // handle window resize
    glutDisplayFunc(display);   // handle rendering
    glutKeyboardFunc(key);      // handle keyboard input
    glutIdleFunc(idle);         // handle idle tasks

    // play a sound file
    // PlaySound("starwars.wav", NULL, SND_ASYNC | SND_FILENAME| SND_LOOP);

    // set clear color
    glClearColor(1, 1, 1, 1);

    // enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // enable lighting 
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    // set up light properties for lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // set up material properties for lighting
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    // enter the GLUT main loop
    glutMainLoop();

    // return success status
    return EXIT_SUCCESS;
}
