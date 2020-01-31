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
int steps=20;     //# of subdivisions
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
bool curveFlag = true;
bool frenetFlag = false;

//curve switch index
int curve_idx = 1;
int p_idx = 0;
vector <Vect3d> *circle;

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
} // spiral along y-axis

//This defines the Tornado curve 
inline Vect3d Tornado(GLfloat t)
{
	const float height = 2.f;
	const float rot = 5.f;
	return Vect3d(t*(float)sin(rot*M_PI*t), height*t, t*(float)cos(rot*M_PI*t));
}

//This defines the Cardioid spiral
inline Vect3d ButtCheek(GLfloat t) { 
	const float rad = 0.5f;
	const float rot = 5.f;
	return Vect3d(-rad*(float)(sin(rot*M_PI*t)*(1 + cos(rot*M_PI*t))), -rad*(float)(cos(rot*M_PI*t)*(1 + cos(rot*M_PI*t))), t);
}

//This fills the <vector> *a with data. 
void CreateCurve(vector <Vect3d> *a, int n, Vect3d (&CurveFunc)(GLfloat))
{
	GLfloat step=1.f/n;
	for (int i=0;i<n;i++)
	{
			a->push_back(CurveFunc(i*step));
	}
}

//Call THIS for a new curve. It clears the old one first
void InitArray(int n)
{
	v.clear();
	if (curve_idx == 1) {
		CreateCurve(&v, n, P);
	} 
	if (curve_idx == 2) {
		CreateCurve(&v, n, Tornado);
	}
	if (curve_idx == 3) {
		CreateCurve(&v, n, ButtCheek);
	}
	
}

//returns random number from <-1,1>
inline float random11() { 
	return 2.f*rand() / (float)RAND_MAX - 1.f;
}

//randomizes an existing curve by adding random number to each coordinate
void Randomize(vector <Vect3d> *a) {
	const float intensity = 0.01f;
	for (unsigned int i = 0; i < a->size(); i++) {
		Vect3d r(random11(), random11(), random11());
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

inline void Tangent(Vect3d &tan, Vect3d v1, Vect3d v2, float scale) {
	tan = (v2 - v1) / 2; //modified tangent for frenet frame
	tan.Normalize();
	tan *= scale;
}
inline void Normal(Vect3d &normal, Vect3d p1, Vect3d p2, Vect3d p3, float scale) {
	Vect3d tan2;
	Vect3d tan1;	
	tan2 = p3 - p2;
	tan1 = p2 - p1;
	tan2.Normalize();
	tan1.Normalize();
	normal = (tan2 - tan1) / 2;
	normal.Normalize();
	normal *= scale;
}

inline void OsculatingCircle(Vect3d Color, float scale) {
	// 1. Get Unit Tangent
	Vect3d tan;
	Tangent(tan, v[p_idx + 1], v[p_idx - 1], scale);

	// 2. Get Unit Normal
	Vect3d normal;
	Vect3d K = v[p_idx - 1];
	Vect3d L = v[p_idx];
	Vect3d M = v[p_idx + 1];
	Normal(normal, K, L, M, scale);
	
	// 3. Compute Area of Triangle
	Vect3d KL = K - L;
	Vect3d LM = L - M;
	Vect3d MK = M - K;
	float A = 0.5 * KL.Cross(LM).Length();

	// 4. Compute radius from Menger Curvature
	float c = (4 * A) / (KL.Length() * LM.Length() * MK.Length());
	float rad = 1 / c;

	// 5. Draw Circle
	Vect3d center = v[p_idx] + rad * normal;
	const float height = 1.f;
	Vect3d start = center + (rad*tan);
	float step = 1.f / 100;
	for (unsigned int t = 1; t < 100; t++) {
		Vect3d next = center + (rad*tan*cos(5 * M_PI*t*step)) + (rad*normal*sin(5 * M_PI*t*step));
		DrawLine(start, next, Color);
		start = next;
	}
}

//this is the actual code for the lab
void Lab01() {
	Vect3d a,b,c;
	Vect3d origin(0, 0, 0);
	Vect3d red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

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

	//draw Frenet Frame
	if (frenetFlag) {
		for (unsigned int i = 1; i < v.size() - 1; i++) {
			float scale = 0.05;
			Vect3d tan;
			Tangent(tan, v[i - 1], v[i + 1], scale);		
			DrawLine(v[i], v[i] + tan, red);

			Vect3d normal;
			Normal(normal, v[i-1], v[i], v[i+1], scale);
			DrawLine(v[i], v[i] + normal, blue);

			Vect3d binormal;
			binormal = tan.Cross(normal);
			//binormal.Set(Vect3d::Cross(tan, normal)); //static method
			binormal.Normalize();
			binormal *= scale;
			DrawLine(v[i], v[i] + binormal, green);
		}
	}

	//draw Osculating Circle
	if (p_idx > 0) {
		OsculatingCircle(red, 0.5);
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
	case 'f': frenetFlag = !frenetFlag; break;
	
	case 32: {
		if (angleIncrement == 0) angleIncrement = defaultIncrement;
		else angleIncrement = 0;
		break;
	}
	case 's': {sign = -sign; break; }
	case '-': {
		steps--;
		if (steps<1) steps = 1;		
		InitArray(steps);	
		p_idx--;
		break;
	}
	case '+': {
		steps++;		
		InitArray(steps);		
		p_idx++;
		break;
	}
	case 'r': {
		Randomize(&v);
		break;
	}
	
	case '1': {
		curve_idx = 1;
		InitArray(steps);
		break;
	}
	case '2': {
		curve_idx = 2;
		InitArray(steps);
		break;
	}
	case '3': {
		curve_idx = 3;
		InitArray(steps);
		break;
	}
	case '<': {
		if (p_idx > 0)
			p_idx--;
		break;
	}
	case '>': {
		if(p_idx < v.size()-2)
			p_idx++;
		break;
	}
	}
	cout << "[points]=[" << steps << "]" << endl;
	glutPostRedisplay();
}


/*******************
OpenGL code. Do not touch.
******************/
void Idle(void)
{
  glClearColor(0.5f,0.5f,0.5f,1); //background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GLMessage("Lab 1 - CS 590CGS");
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
  glutSpecialFunc(NULL);
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMotion);
  InitArray(steps);
  glutMainLoop();
  return 0;        
}
