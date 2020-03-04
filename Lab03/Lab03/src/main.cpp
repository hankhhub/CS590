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
#include <GL/glew.h>
#include <GL/freeglut.h>


//in house created libraries
#include "math/vect3d.h"    //for vector manipulation
#include "trackball.h"
#include "helper.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

//some trackball variables -> used in mouse feedback
TrackBallC trackball;
bool mouseLeft, mouseMid, mouseRight;

GLuint points = 0;  //number of points to display the object
int steps = 20;     //# of subdivisions
bool needRedisplay = false;
GLfloat  sign = +1; //diretcion of rotation
const GLfloat defaultIncrement = 0.7f; //speed of rotation
GLfloat  angleIncrement = defaultIncrement;

//window size
GLint wWindow = 1200;
GLint hWindow = 800;

//this defines what will be rendered
//see Key() how is it controlled
bool tangentsFlag = false;
bool pointsFlag = false;
bool curveFlag = true;



/**********************
LAB related MODIFY
***********************/

// 
inline void AddVertex(vector <GLfloat> *a, const GLfloat *A)
{
	a->push_back(A[0]); a->push_back(A[1]); a->push_back(A[2]);
}

class CTriangle {
public:
	CTriangle() {};
	CTriangle(Vect3d v1, Vect3d v2, Vect3d v3);
	void AddTriangle(vector<GLfloat> *a, CTriangle *tri);
	void Set(Vect3d v1, Vect3d v2, Vect3d v3);

private:
	Vect3d v1, v2, v3;
};


CTriangle::CTriangle(Vect3d v1, Vect3d v2, Vect3d v3) {
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
}

void CTriangle::Set(Vect3d v1, Vect3d v2, Vect3d v3) {
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
}

void CTriangle::AddTriangle(vector <GLfloat> *a, CTriangle *tri) 
{
	AddVertex(a, tri->v1);
	AddVertex(a, tri->v2);
	AddVertex(a, tri->v3);
}

class Quad {
public:
	Quad() {};
	Quad(Vect3d v1, Vect3d v2, Vect3d v3, Vect3d v4);
	void AddQuad(vector<GLfloat> *a, Quad *q);
	void Set(Vect3d v1, Vect3d v2, Vect3d v3, Vect3d v4);

private:
	Vect3d v1, v2, v3, v4;
};

Quad::Quad(Vect3d v1, Vect3d v2, Vect3d v3, Vect3d v4) {
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->v4 = v4;
}
void Quad::Set(Vect3d v1, Vect3d v2, Vect3d v3, Vect3d v4) {
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->v4 = v4;
}

void Quad::AddQuad(vector <GLfloat> *a, Quad *q)
{
	AddVertex(a, q->v1);
	AddVertex(a, q->v2);
	AddVertex(a, q->v3);
	AddVertex(a, q->v4);
}


class Face {
public:
	//constructors for the mesh
	Face() {};
	Face(int a, int b, int c, int d);
	
	// four vertices(indices) of face
	int indices[4];
};
Face::Face(int p1, int p2, int p3, int p4) {
	this->indices[0] = p1;
	this->indices[1] = p2;
	this->indices[2] = p3;
	this->indices[3] = p4;
}

typedef std::vector<unsigned int> AdjacentVertices;
typedef std::vector<unsigned int> AdjacentFaces;

class Vertex {
public:
	Vertex() {};
	Vertex(Vect3d vertex);
	Vect3d v;
	AdjacentVertices adjV;
	AdjacentFaces adjF;
};
Vertex::Vertex(Vect3d vertex) {
	this->v = vertex;
}

typedef std::vector<Vertex> Vertices;
typedef std::vector<Face> Faces;
struct Mesh
{
	Vertices vertices;
	Faces faces;
};
Mesh mesh;

//returns random number from <-1,1>
inline float random11() {
	return 2.f*rand() / (float)RAND_MAX - 1.f;
}



void CreateCube(vector <GLfloat> *a ) 
{
	CTriangle top1, top2, bot1, bot2;
	CTriangle frnt1, frnt2, back1, back2;
	CTriangle left1, left2, right1, right2;
	Vect3d v1, v2, v3;
	v1.Set(-0.5f, -0.5f, -0.5f); v2.Set(0.5f, -0.5f, -0.5f); v3.Set(0.5f, 0.5f, -0.5f);
	back1.Set(v1, v2, v3);
	back1.AddTriangle(a, &back1);

	v2.Set(-0.5f, 0.5f, -0.5f);
	back2.Set(v1, v2, v3);
	back2.AddTriangle(a, &back2);

	v1.Set(-0.5f, -0.5f, 0.5f); v2.Set(0.5f, -0.5f, 0.5f); v3.Set(0.5f, 0.5f, 0.5f);
	frnt1.Set(v1, v2, v3);
	frnt1.AddTriangle(a, &frnt1);

	v2.Set(-0.5f, 0.5f, 0.5f);
	frnt2.Set(v1, v2, v3);
	frnt2.AddTriangle(a, &frnt2);

	v1.Set(-0.5f, 0.5f, 0.5f); v2.Set(-0.5f, 0.5f, -0.5f); v3.Set(-0.5f, -0.5f, -0.5f);
	left1.Set(v1, v2, v3);
	left1.AddTriangle(a, &left1);

	v2.Set(-0.5f, -0.5f, 0.5f);
	left2.Set(v1, v2, v3);
	left2.AddTriangle(a, &left2);

	v1.Set(0.5f, 0.5f, 0.5f); v2.Set(0.5f, 0.5f, -0.5f); v3.Set(0.5f, -0.5f, -0.5f);
	right1.Set(v1, v2, v3);
	right1.AddTriangle(a, &right1);

	v2.Set(0.5f, -0.5f, 0.5f);
	right2.Set(v1, v2, v3);
	right2.AddTriangle(a, &right2);

	v1.Set(-0.5f, -0.5f, -0.5f); v2.Set(0.5f, -0.5f, -0.5f); v3.Set(0.5f, -0.5f, 0.5f);
	bot1.Set(v1, v2, v3);
	bot1.AddTriangle(a, &bot1);

	v2.Set(-0.5f, -0.5f, 0.5f);
	bot2.Set(v1, v2, v3);
	bot2.AddTriangle(a, &bot2);

	v1.Set(-0.5f, 0.5f, -0.5f); v2.Set(0.5f, 0.5f, -0.5f); v3.Set(0.5f, 0.5f, 0.5f);
	top1.Set(v1, v2, v3);
	top1.AddTriangle(a, &top1);

	v2.Set(-0.5f, 0.5f, 0.5f);
	top2.Set(v1, v2, v3);
	top2.AddTriangle(a, &top2);
}

void CubeVertices(Mesh* m, Vect3d c, float delta) {
	float x, y, z;
	float startx = c.v[0] - delta; float endx = c.v[0] + delta;
	float starty = c.v[1] - delta; float endy = c.v[1] + delta;
	float startz = c.v[2] - delta; float endz = c.v[2] + delta;
	
	for (x = startx; x <= endx; x += 2*delta)
		for (y = starty; y <= endy; y += 2*delta)
			for (z = startz; z <= endz; z += 2*delta) 
			{
				m->vertices.push_back(Vect3d(x, y, z));
			}
}
void CubeFaces(Mesh *m, Face initface) {
	int p1 = initface.indices[0];
	int p2 = initface.indices[1];
	int p3 = initface.indices[2];
	int p4 = initface.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4));
	m->faces.push_back(Face(p1+6, p2+6, p3+2, p4+2));
	m->faces.push_back(Face(p1+4, p2+4, p3-2, p4-2));
	m->faces.push_back(Face(p1+2, p2+2, p3+4, p4+4));
	m->faces.push_back(Face(p1, p2+1, p3+3, p4+2));
	m->faces.push_back(Face(p1+5, p2+6, p3, p4-1));
}

void FindAdjacencies(Mesh *m) {
	for (int i = 0;  i < m->vertices.size(); ++i) {
		m->vertices[i].adjF.clear();
		m->vertices[i].adjV.clear();
	}

	for (int f = 0; f < m->faces.size(); ++f) {
		Face curFace = m->faces[f];
		for (int p = 0; p < 4; ++p) {
			m->vertices[curFace.indices[p]].adjF.push_back(f);
			m->vertices[curFace.indices[(p + 1) % 4]].adjV.push_back(curFace.indices[p]);
			m->vertices[curFace.indices[(p + 1) % 4]].adjV.push_back(curFace.indices[(p + 2) % 4]);
		}
	}
}

void CreateCube2(Mesh *m) {
	float scale = 0.1;
	float x = 0;
	float y = 0;
	float z = 0;
	float offset = 0.1;
	Vect3d center = Vect3d(x, y, z);

	CubeVertices(m, center, offset);
	/*m->f.push_back(Face(0, 1, 3, 2));
	m->f.push_back(Face(6, 7, 5, 4));
	m->f.push_back(Face(4, 5, 1, 0)); 
	//m->f.push_back(Face(2, 3, 7, 6));
	m->f.push_back(Face(0, 2, 6, 4));
	m->f.push_back(Face(5, 7, 3, 1));*/
	CubeFaces(m, Face(0, 1, 3, 2));
	
	center.v[1] += 0.5f;
	center.v[0] -= 0.2f;
	CubeVertices(m, center, offset-0.05);

	m->faces.push_back(Face(3, 7, 13, 9));
	m->faces.push_back(Face(7, 6, 12, 13));
	m->faces.push_back(Face(2, 6, 12, 8));
	m->faces.push_back(Face(2, 3, 9, 8));

	m->faces.push_back(Face(8, 9, 11, 10));
	m->faces.push_back(Face(14, 15, 13, 12));
	m->faces.push_back(Face(12, 13, 9, 8));
	m->faces.push_back(Face(10, 11, 15, 14));
	m->faces.push_back(Face(8, 10, 14, 12));
	m->faces.push_back(Face(13, 15, 11, 9));

	center.v[1] += 0.2f;
	center.v[0] += 0.4f;
	CubeVertices(m, center, offset - 0.03);
	CubeFaces(m, Face(16, 17, 19, 18));
}

//this is the actual code for the lab
void Lab03(Mesh *m) {
	Vect3d origin(0, 0, 0);
	Vect3d red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

	// Create your structure here
	CreateCube2(m);
	FindAdjacencies(m);
}
// Prepares all vertices for drawing
void InitArray(int n)
{
	GLuint vao;

	mesh.faces.clear();
	mesh.vertices.clear();
	Lab03(&mesh);
	/*glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	points = v.size();
	glBufferData(GL_ARRAY_BUFFER, points * sizeof(GLfloat), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	v.clear();*/
}

void drawMesh() {
	glBegin(GL_QUADS);
	for (Faces::const_iterator f = mesh.faces.begin(); f != mesh.faces.end(); ++f)
	{
		glVertex3fv(mesh.vertices[f->indices[0]].v);
		glVertex3fv(mesh.vertices[f->indices[1]].v);
		glVertex3fv(mesh.vertices[f->indices[2]].v);
		glVertex3fv(mesh.vertices[f->indices[3]].v);
	}
	glEnd();
}
//the main rendering function
void RenderObjects()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//set camera
	glMatrixMode(GL_MODELVIEW);
	trackball.Set3DViewCamera();
	//call the student's code from here
	CoordSyst();
	//glDrawArrays(GL_TRIANGLES, 0, 3 * points);
	drawMesh();
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
		break;
	}
	case 's': {sign = -sign; break; }
	case '-': {
		steps--;
		if (steps < 1) steps = 1;
		InitArray(steps);
		break;
	}
	case '+': {
		steps++;
		InitArray(steps);
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
	glClearColor(0.5f, 0.5f, 0.5f, 1); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLMessage("Lab 3 - CS 590CGS");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (GLfloat)wWindow / (GLfloat)hWindow, 0.01, 100); //set the camera
	glMatrixMode(GL_MODELVIEW); //set the scene
	glLoadIdentity();
	gluLookAt(0, 10, 10, 0, 0, 0, 0, 1, 0); //set where the camera is looking at and from. 
	static GLfloat angle = 0;
	angle += angleIncrement;
	if (angle >= 360.f) angle = 0.f;
	glRotatef(sign*angle, 0, 1, 0);
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
	glutInitWindowSize(wWindow, hWindow);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Catmull Clark Subdivision");
	GLenum err = glewInit();
	 if (GLEW_OK != err){
	 fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
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
