/*********************************/
/* CS 590CGS Lab framework        */
/* (C) Bedrich Benes 2020        */
/* bbenes ~ at ~ purdue.edu      */
/* Press +,- to add/remove points*/
/*       r to randomize          */
/*       s to change rotation    */
/*       c to render curve       */
/*       t to render tangents    */
/*       p to render points      */
/*       s to change rotation    */
/*********************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <string>
#include <vector>			//Standard template library class
#include <GL/freeglut.h>


//in house created libraries
#include "math/vect3d.h"    //for vector manipulation
#include "trackball.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "freeglut.lib")

using namespace std;

//some trackball variables -> used in mouse feedback
TrackBallC trackball;
bool mouseLeft, mouseMid, mouseRight;


GLuint points=0;  //number of points to display the object
int steps=10;     //# of subdivisions
int head = steps / 2;
int tail = head;
float density = 1.0f;
float divThreshold = 0.1f;
bool needRedisplay=false;
GLfloat  sign=+1; //diretcion of rotation
const GLfloat defaultIncrement=0.7f; //speed of rotation
GLfloat  angleIncrement=defaultIncrement;

vector <Vect3d> v;   //all the points will be stored here

//window size
GLint wWindow=1200;
GLint hWindow=800;

//this defines what will be rendered
//see Key() how is it controlled
bool tangentsFlag = false;
bool pointsFlag = false;
bool curveFlag = false;

bool randptFlag = false;
bool cubicptFlag = false;
bool bezierFlag = false;

// Function Declarations
inline float random11(float scale);
void InjectC1( int idx, float scale);

/*********************************
Some OpenGL-related functions DO NOT TOUCH
**********************************/
//displays the text message in the GL window
void GLMessage(char *message)
{
	static int i;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.f, 100.f, 0.f, 100.f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3ub(0, 0, 255);
	glRasterPos2i(10, 10);
	for (i = 0; i<(int)strlen(message); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

//called when a window is reshaped
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glEnable(GL_DEPTH_TEST);
	//remember the settings for the camera
	wWindow = w;
	hWindow = h;
}

//Some simple rendering routines using old fixed-pipeline OpenGL
//draws line from a to b with color 
void DrawLine(Vect3d a, Vect3d b, Vect3d color) {

	glColor3fv(color);
	glBegin(GL_LINES);
		glVertex3fv(a);
		glVertex3fv(b);
	glEnd();
}

//draws point at a with color 
void DrawPoint(Vect3d a, Vect3d color) {

	glColor3fv(color);
	glPointSize(5);
	glBegin(GL_POINTS);
	 glVertex3fv(a);
	glEnd();
}

/**********************
LAB related MODIFY
***********************/

//This defines the actual curve 
inline Vect3d P(GLfloat t)
{
	const float rad = 0.2f;
	const float height = 1.f;
	const float rot = 5.f;
	return Vect3d(rad*(float)sin(rot*M_PI*t),height*t,rad*(float)cos(rot*M_PI*t)); //spiral with radius rad, height, and rotations rot
}

//This fills the <vector> *a with data. 
void CreateCurve(vector <Vect3d> *a, int n)
{
	GLfloat step=1.f/n;
	for (int i=0;i<n;i++)
	{
			a->push_back(P(i*step));
	}
}
inline Vect3d RandomPoints(float s) {
	return Vect3d(random11(s), random11(s), random11(s));
}

inline Vect3d CubicPoints(float t) {
	float cubic_poly = 1.3f*pow(t, 3) -2.4f*pow(t, 2) -5*pow(t, 1) ;	
	return Vect3d(cubic_poly, t, cubic_poly);
}
void CreatePoints(vector <Vect3d> *p, int n) {
	GLfloat step = 1.f / n;
	if (randptFlag) {
		for (int i = 0; i < n; i++)
		{
			p->push_back(RandomPoints(density));
			if (v.size() % 3 == 2 && v.size() > 3) {
				InjectC1( v.size()-1, 0.2f);
			}
		}
	}
	else {
		int start = -(int)(n / 2);
		int end = (int)(n / 2);
		for (int i = start; i < end; i++) {
			p->push_back(CubicPoints(step*i));
			if (v.size() % 3 == 2 && v.size() > 3) {
				InjectC1(v.size() - 1, 0.05f);
			}
		}
	}
}
//Call THIS for a new curve. It clears the old one first
void InitArray(int n)
{
	v.clear();
	if (randptFlag || cubicptFlag) {
		CreatePoints(&v, n);
	}
	else {
		CreateCurve(&v, n);
	}
}

inline void GetBezier(Vect3d &bezCub, Vect3d &P0, Vect3d &P1, Vect3d &P2, Vect3d &P3, float t) {
	float B0, B1, B2, B3;
	B0 = pow(1 - t, 3);
	B1 = 3 * t * pow(1 - t, 2);
	B2 = 3 * t * t * (1 - t);
	B3 = pow(t, 3);
	P0.Normalize();
	
	bezCub.v[0] = P0.v[0]*B0 + P1.v[0]*B1 + P2.v[0]*B2 + P3.v[0]*B3;
	bezCub.v[1] = P0.v[1]*B0 + P1.v[1]*B1 + P2.v[1]*B2 + P3.v[1]*B3;
	bezCub.v[2] = P0.v[2] * B0 + P1.v[2] * B1 + P2.v[2] * B2 + P3.v[2] * B3;
}

inline void DrawBezierCurve(Vect3d p0, Vect3d p1, Vect3d p2, Vect3d p3, int n, Vect3d color) {
	Vect3d src, dest;
	double t, deltat;
	deltat = 1.0 / n;
	src = p0;
	t = 0;

	for (int i = 0; i < n; i++) {
		GetBezier(dest, p0, p1, p2, p3, t+deltat);
		DrawLine(src, dest, color);
		src = dest;
		t += deltat;
	}
}

inline void Subdivide(Vect3d P[], Vect3d left[], Vect3d right[]) {
	Vect3d temp;
	left[0] = P[0];
	left[1] = (P[0] + P[1]) / 2.f;
	temp = (P[1] + P[2]) / 2.f;
	left[2] = (left[1] + temp) / 2.f;
	right[3] = P[3];
	right[2] = (P[2] + P[3]) / 2.f;
	right[1] = (right[2] + temp) / 2.f;
	left[3] = (left[2] + right[1]) / 2.f;
	right[0] = left[3]; // middle point
}

inline float Distance(Vect3d p1, Vect3d p2) {
	float dx = p1.v[0] - p2.v[0];
	float dy = p1.v[1] - p2.v[1];
	float dz = p1.v[2] - p2.v[2];
	return sqrt(dx*dx + dy * dy + dz * dz);
}

inline void DeCasteljau(Vect3d orig[], Vect3d color) {
	Vect3d left[4];
	Vect3d right[4];

	if (Distance(orig[0], orig[3]) < divThreshold) { //caution: computation heavy for threshold < 0.0001
		DrawLine(orig[0], orig[3], color );
		return;
	}
	Subdivide(orig, left, right);
	DeCasteljau(left, color);
	DeCasteljau(right, color);
}

inline void InjectC1( int idx, float scale) {
	Vect3d p0, p1, p2;
	p0 = v[idx - 2];
	p1 = v[idx - 1];
	p2 = v[idx];
	Vect3d T1 = (p1 - p0);
	Vect3d T2 = (p2 - p1);
	// Check if merge point is C1 continuous
	if (T1 == T2) {
		return;
	}
	T1.Normalize();
	T2.Normalize();

	Vect3d src = p1 - scale * T1;	
	Vect3d dest = p1 + scale * T2;	
	v.insert(v.begin() + idx-1, src);
	v.insert(v.begin() + idx, p1);
	v.insert(v.begin() + idx+2, dest);
}



//returns random number from <-1,1>
inline float random11(float scale) { 
	return 2.f*rand() / ((float)RAND_MAX / scale) - 1.f;
}

//randomizes an existing curve by adding random number to each coordinate
void Randomize(vector <Vect3d> *a, float s) {
	const float intensity = 0.01f;
	for (unsigned int i = 0; i < a->size(); i++) {
		Vect3d r(random11(s), random11(s), random11(s));
		a->at(i) = a->at(i) + intensity*r;
	}
}

//display coordinate system
void CoordSyst() {
	Vect3d a, b, c;
	Vect3d origin(0, 0, 0);
	Vect3d red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

	//draw the coordinate system 
	a.Set(1, 0, 0);
	b.Set(0, 1, 0);
	c.Set(Vect3d::Cross(a, b)); //use cross product to find the last vector
	glLineWidth(4);
	DrawLine(origin, a, red);
	DrawLine(origin, b, green);
	DrawLine(origin, c, blue);
	glLineWidth(1);

}

inline void AddSeg() {
	if (randptFlag) {
		CreatePoints(&v, 3);
	}
	else {
		//InitArray(steps);
		//steps += 3;
		GLfloat step = 1.f / steps;
		tail += 3;
		for (int i = head; i < tail; i++) {
			v.push_back(CubicPoints(step*i));
		}
		head = tail;
	}
}
inline void DeleteSeg() {
	if (randptFlag) {
		for (int i = 0; i < 3; i++)
			v.pop_back();
	}
	else {
		//InitArray(steps - 3);
		//steps -= 3;		
		tail -= 3;
		for (int i = 0; i < 3; i++)
			v.pop_back();
		head = tail;
	}
}


//this is the actual code for the lab
void Lab01() {
	Vect3d a,b,c;
	Vect3d origin(0, 0, 0);
	Vect3d red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0), purple(1,0,1);


	CoordSyst();
	//draw the curve
	if (curveFlag)
		for (unsigned int i = 0; i < v.size() - 1; i++) {
		DrawLine(v[i], v[i + 1], almostBlack);
	}

	//draw the points
	if (pointsFlag)
		for (unsigned int i = 0; i < v.size() - 1; i++) {
		DrawPoint(v[i], blue);
	}

//draw the tangents
	if (tangentsFlag)
	for (unsigned int i = 0; i < v.size() - 1; i++) {
		Vect3d tan;
		tan = v[i + 1] - v[i]; //too simple - could be better from the point after AND before
		tan.Normalize(); 
		tan *= 0.2;
		DrawLine(v[i], v[i]+tan, red);
	}

//draw piecewise linear curve
	if (randptFlag || cubicptFlag) {
		for (unsigned int i = 0; i < v.size() - 1; i++) {	
			DrawLine(v[i], v[i + 1], almostBlack);			
		}			
	}

//draw approximated bezier curves
	if (bezierFlag) {
		int steps = 10;
		int numCurves = (int) (v.size() / 3);
		if (v.size() % 3 == 1) {
			for (unsigned int i = 0; i < numCurves; i++) {
				int j = i * 3;
				//DrawBezierCurve(v[j], v[j + 1], v[j + 2], v[j + 3], steps, purple);
				Vect3d CP[4] = { v[j], v[j + 1], v[j + 2], v[j + 3] };
				DeCasteljau(CP, purple);
			}
		}		
	}
}

//the main rendering function
void RenderObjects()
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//set camera
	glMatrixMode(GL_MODELVIEW);
	trackball.Set3DViewCamera();
	//call the student's code from here
	Lab01();
}

//Add here if you want to control some global behavior
//see the pointFlag and how is it used
void Kbd(unsigned char a, int x, int y)//keyboard callback
{
	switch (a)
	{
	case 27: exit(0); break;
	case 't': tangentsFlag = !tangentsFlag; break;
	case 'p': pointsFlag = !pointsFlag; break;
	case 'c': curveFlag = !curveFlag; break;
	case 32: {
		if (angleIncrement == 0) angleIncrement = defaultIncrement;
		else angleIncrement = 0;

		bezierFlag = !bezierFlag;
		break;
	}
	case 's': {sign = -sign; break; }
	case '-': {
		//if (steps<1) steps = 1;
		if (randptFlag) {
			density += 0.5f;
			InitArray(steps);
		}
		else {
			steps /= 2;
			InitArray(steps);
		}
		
		break;
	}
	case '+': {
		if (randptFlag) {
			density -= 0.5f;
			if (density < 1) {
				density = 1.0f;
			}			
			InitArray(steps);
		}
		else {
			steps *= 2;
			InitArray(steps);
		}
		break;
	}
	case 'r': {
		Randomize(&v, density);
		break;
	}
	case 'R': {
		randptFlag = !randptFlag;
		InitArray(steps);
		break;
	}
	case 'B': {
		cubicptFlag = !cubicptFlag;
		InitArray(steps);
		break;
	}
	case '>': {
		AddSeg();
		break;
	}
	case '<': {
		DeleteSeg();
		break;
	}
	
	}
	cout << "[points]=[" << steps << "]" << endl;
	glutPostRedisplay();
}

void DirectionKeys(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_DOWN: {
		divThreshold += 0.001f;
		printf("%f\n", divThreshold);
	}
	case GLUT_KEY_LEFT: {
		divThreshold -= 0.001f;
		printf("%f\n", divThreshold);
	}
	}
}

/*******************
OpenGL code. Do not touch.
******************/
void Idle(void)
{
  glClearColor(0.5f,0.5f,0.5f,1); //background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GLMessage("Lab 2 - CS 590CGS");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40,(GLfloat)wWindow/(GLfloat)hWindow,0.01,100); //set the camera
  glMatrixMode(GL_MODELVIEW); //set the scene
  glLoadIdentity();
  gluLookAt(0,10,10,0,0,0,0,1,0); //set where the camera is looking at and from. 
  static GLfloat angle=0;
  angle+=angleIncrement;
  if (angle>=360.f) angle=0.f;
  glRotatef(sign*angle,0,1,0);
  RenderObjects();
  glutSwapBuffers();  
}

void Display(void)
{

}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseLeft = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		trackball.Set(false, x, y);
		mouseLeft = false;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseMid = true;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		trackball.Set(true, x, y);
		mouseMid = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseRight = true;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		trackball.Set(true, x, y);
		mouseRight = false;
	}
}

void MouseMotion(int x, int y) {
	if (mouseLeft)  trackball.Rotate(x, y);
	if (mouseMid)   trackball.Translate(x, y);
	if (mouseRight) trackball.Zoom(x, y);
	glutPostRedisplay();
}


int main(int argc, char **argv)
{ 
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(500,100);
  glutCreateWindow("Surface of Revolution");
  //GLenum err = glewInit();
  // if (GLEW_OK != err){
  // fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  //}
  glutDisplayFunc(Display);
  glutIdleFunc(Idle);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Kbd); //+ and -
  glutSpecialUpFunc(NULL); 
  glutSpecialFunc(DirectionKeys);
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMotion);
  InitArray(steps);
  glutMainLoop();
  return 0;        
}
