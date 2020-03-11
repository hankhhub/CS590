/*********************************/
/*  CS 590CGS Lab03              */
/* (C) Hank Huang 2020           */
/* huang670 ~ at ~ purdue.edu    */
/* Press +,- to add/remove points*/
/*       r to randomize          */
/*       s to change rotation    */
/*       c to render curve       */
/*       t to render tangents    */
/*       p to render points      */
/*       s to change rotation    */
/*********************************/

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <string>
#include <vector>			//Standard template library class
#include <GL/glew.h>
#include <assert.h>
#include <GL/freeglut.h>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

//in house created libraries
#include "math/vect3d.h"    //for vector manipulation
#include "trackball.h"
#include "helper.h"
#include "face.h"
#include "vertex.h"
#include "treebuilder.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

using namespace std;

#define MAXLEN 20
#define BINARY 2
#define TERNARY 3
#define QUATERNARY 4

//some trackball variables -> used in mouse feedback
TrackBallC trackball;
bool mouseLeft, mouseMid, mouseRight;

GLuint points = 0;  //number of points to display the object
int steps = 0;     //# of subdivisions
bool needRedisplay = false;
GLfloat  sign = +1; //direction of rotation
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

RotationAxis axis = NONE;
float scale = 0.1f;
int angle = 0;
float length = 2.0f;
float width = scale /(float) 2;
int n = 1;

int branch_type = BINARY;


/**********************
LAB related MODIFY
***********************/
struct FacePoint {
	Vect3d facePoint;
	int facePointID;
};
typedef unordered_map<int, FacePoint> FaceMap;
Mesh mesh;

void FindAdjacencies(Mesh *m) {
	for (int i = 0; i < m->vertices.size(); ++i) {
		m->vertices[i].adjF.clear();
		m->vertices[i].adjV.clear();
		m->vertices[i].faceCentroidSum = Vect3d(0, 0, 0);
	}

	for (int f = 0; f < m->faces.size(); ++f) {
		Face * curFace = &m->faces[f];
		for (int p = 0; p < 4; ++p) {
			m->vertices[curFace->indices[p]].adjF.push_back(f);
			m->vertices[curFace->indices[p]].fSet.insert(f);
			m->vertices[curFace->indices[(p + 1) % 4]].adjV.insert(curFace->indices[p]);
			m->vertices[curFace->indices[(p + 1) % 4]].adjV.insert(curFace->indices[(p + 2) % 4]);
		}
	}

}

void MeshCleanUp(Mesh *m) {	
	for (int i = 0; i < m->vertices.size(); ++i) {
		m->vertices[i].adjF.clear();
		m->vertices[i].adjV.clear();
		m->vertices[i].faceCentroidSum = Vect3d(0, 0, 0);
		m->vertices[i].edgeMap.clear();
		m->vertices[i].fSet.clear();
	}
	m->faces.clear();
	m->vertices.clear();
}

int ComputeEdgePoint(Mesh *m, Mesh *new_m, Vertex *vertex, Face *face, int p, int f, int e, int vertCount) {
	// Compute two edge points if haven't already exist
	if (vertex->edgeMap.find(e) == vertex->edgeMap.end()) {
		Vect3d edgePoint;
		AdjacentFaces * edgeFaces = &m->vertices[e].adjF;
		for (AdjacentFaces::iterator it = edgeFaces->begin(); it != edgeFaces->end(); ++it) {
			// Check if edge share two faces
			if (vertex->fSet.find(*it) != vertex->fSet.end() && *it != f) {
				Vect3d p1 = vertex->v;
				Vect3d p2 = m->vertices[e].v;
				Vect3d p3 = face->centroid;
				Vect3d p4 = m->faces[*it].centroid;
				edgePoint = (p1 + p2 + p3 + p4) / 4;
				vertex->edgeMap[e].edgePoint = edgePoint;
				vertex->edgeMap[e].edgePointID = vertCount;
				m->vertices[e].edgeMap[p].edgePoint = edgePoint;
				m->vertices[e].edgeMap[p].edgePointID = vertCount;

				new_m->vertices.push_back(edgePoint); //1										
				vertCount++;
				break;
			}
		}
	}
	return vertCount;
}

void ComputeFacePoints(Mesh *m) {
	for (int f = 0; f < m->faces.size(); f++) {
		Face* face = &m->faces[f];
		Vect3d a = m->vertices[face->indices[0]].v;
		Vect3d b = m->vertices[face->indices[1]].v;
		Vect3d c = m->vertices[face->indices[2]].v;
		Vect3d d = m->vertices[face->indices[3]].v;
		face->centroid = (a + b + c + d) / 4;
	}
}
void SubdivideMesh(Mesh *m) {
	//Mesh new_mesh;
	Mesh new_mesh;

	ComputeFacePoints(m);

	int newFaceCount = 0;
	int newVertCount = 0;
	FaceMap faceMap;
	for (int p = 0; p < m->vertices.size(); ++p) {
		Vertex * vertex = &m->vertices[p];
		Vertices * new_vertices = &new_mesh.vertices;

		int adjFCount = vertex->adjF.size();
		int adjVCount = vertex->adjV.size();

		for (int f = 0; f < adjFCount; ++f) {
			int f_idx = vertex->adjF[f];
			Face* face = &m->faces[f_idx];
			vertex->faceCentroidSum += face->centroid;

			if (faceMap.find(f_idx) == faceMap.end()) {
				faceMap[f_idx].facePointID = newVertCount;
				faceMap[f_idx].facePoint = face->centroid;
				new_mesh.vertices.push_back(face->centroid);
				newVertCount++;
			}

			int edge[2];
			int cnt = 0;
			// Find edges adjacent to center vertex
			for (int corner = 0; corner < 4; ++corner) {
				AdjacentVertices * adjVert = &m->vertices[face->indices[corner]].adjV;
				if (adjVert->find(p) != adjVert->end()) {
					edge[cnt] = face->indices[corner];
					cnt++;
				}
			}

			newVertCount = ComputeEdgePoint(m, &new_mesh, vertex, face, p, f_idx, edge[0], newVertCount);
			newVertCount = ComputeEdgePoint(m, &new_mesh, vertex, face, p, f_idx, edge[1], newVertCount);

			new_mesh.faces.push_back(Face(vertex->edgeMap[edge[0]].edgePointID, faceMap[f_idx].facePointID, vertex->edgeMap[edge[1]].edgePointID, 0));
			newFaceCount++;
		}

		// Compute new gravity point
		Vect3d edgeMidpointSum = Vect3d(0, 0, 0);
		for (EdgeMap::iterator edge_it = vertex->edgeMap.begin(); edge_it != vertex->edgeMap.end(); ++edge_it) {
			edgeMidpointSum += edge_it->second.edgePoint;
		}
		Vect3d R = edgeMidpointSum / (float)adjVCount;
		Vect3d S = vertex->faceCentroidSum / (float)adjFCount;
		float n = (float)adjVCount;
		Vect3d Center = ((n - 3)*vertex->v + (2 * R) + S) / n;

		new_mesh.vertices.push_back(Center);
		newVertCount++;

		// Fill in gravity point to new faces
		for (int i = adjFCount; i > 0; i--) {
			new_mesh.faces[newFaceCount - i].indices[3] = newVertCount - 1;
		}

	}
	faceMap.clear();

	// Replace old mesh vert/faces with new ones
	MeshCleanUp(m);
	m->vertices = new_mesh.vertices;
	m->faces = new_mesh.faces;
	assert(newVertCount == new_mesh.vertices.size());
	assert(newFaceCount == new_mesh.faces.size());
}


void CatmullClark(Mesh *m, int n) {
	for (int i = 0; i < n; i++) {
		FindAdjacencies(m);
		SubdivideMesh(m);
	}
}

void ConstructTree(Mesh *m) {
	int connect = 0;
	
	Vect3d center = Vect3d(0, 0, 0);
	CubeVertices(m, center, scale, NONE, 0);
	TreeTipNegY(m, Face(0, 1, 3, 2));
	
	center.v[1] += scale * 10.0f;
	CubeVertices(m, center, scale - 0.01f, ROTY, 20);
	ConnectY(m, Face(2, 3, 9, 8));
	//TrunkY(m, Face(8, 9, 11, 10));
	TrunkBinary(m, Face(8, 9, 11, 10));
	
	connect = m->vertices.size();

	RecursiveBranch(m, &center, scale, axis, angle, connect, length,width, n, width/n);

			/* Hard Coded Tree */	
	/*LeftBranchConnect(m, &center, scale, ROTY, 20, connect);
	connect = m->vertices.size();
	BranchLeft(m, center, scale, axis, angle, 3, dimension);

	RightBranchConnect(m, &center, scale, ROTZ, 10, connect);
	connect = m->vertices.size();
	BranchRight(m, center, scale, axis, angle, 3, dimension);

	if (branch_type == TERNARY || branch_type == QUATERNARY) {
		LeftBranchConnect(m, &center, scale, ROTY, 20, connect);
		connect = m->vertices.size();
		BranchLeft(m, center, scale, axis, angle, 3, dimension);
	}
	
	if (branch_type == QUATERNARY) {
		RightBranchConnect(m, &center, scale, ROTY, 10, connect);
		connect = m->vertices.size();
		BranchRight(m, center, scale, axis, angle, 3, dimension);
	}

	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	center.v[1] += scale * 4.0f;
	CubeVertices(m, center, scale - 0.05f, ROTZ, 0);
	ConnectY(m, Face(connect - 6, connect - 5, b, a));
	TrunkY(m, Face(a, b, c, d));
	
	BranchTop(m, center, scale, NONE, 0, 1);*/
}


// Prepares all vertices for drawing
void InitArray(int n)
{
	mesh.faces.clear();
	mesh.vertices.clear();

	// Create your structure here
	ConstructTree(&mesh);
	CatmullClark(&mesh, n);
}

void SaveOBJ(Faces f, Vertices v, char *filename, bool triangulate) {

	ofstream myfile;
	myfile.open(filename);

	myfile << "# Generated by Hank Huang huang670@purdue.edu\n";
	myfile << "# vertices\n";
	for (unsigned int i = 0; i < v.size(); i++) {
		myfile << "v " << v.at(i).v.GetZ() << " " << v.at(i).v.GetY() << " " << v.at(i).v.GetX() << "\n";
	}

	myfile << "# faces\n";

	if (triangulate) {
		for (unsigned int i = 0; i < f.size(); i++) {
			myfile << "f " << f[i].indices[0] + 1; // <--- wavefront obj face indices must start at 1 not 0!
			myfile << " " << f[i].indices[2] + 1;
			myfile << " " << f[i].indices[1] + 1 << " " << "\n";

			myfile << "f " << f[i].indices[0] + 1;
			myfile << " " << f[i].indices[2] + 1;
			myfile << " " << f[i].indices[3] + 1 << " " << "\n";
		}
	}
	else {
		for (unsigned int i = 0; i < f.size(); i++) {
			myfile << "f " << f[i].indices[0] + 1;
			myfile << " " << f[i].indices[1] + 1;
			myfile << " " << f[i].indices[2] + 1;
			myfile << " " << f[i].indices[3] + 1 << " " << "\n";
		}
	}
	myfile.close();

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
		if (steps < 1) steps = 0;
		InitArray(steps);
		break;
	}
	case '+': {
		steps++;
		InitArray(steps);
		break;
	}
	case 'x': {
		axis = ROTX;
		InitArray(steps);
		break;
	}
	case 'y': {
		axis = ROTY;
		InitArray(steps);
		break;
	}
	case 'z': {
		axis = ROTZ;
		InitArray(steps);
		break;
	}
	case '>': {
		if (angle < 180) {
			angle += 5;
			InitArray(steps);
		}
		break;
	}
	case '<': {
		if (angle > -180) {
			angle -= 5;
			InitArray(steps);
		}
		break;
	}

	case '[': {
		if (n > 1) {
			n -= 1;
			InitArray(steps);
		}
		break;
	}
	case ']': {
		if (n < 10) {
			n += 1;
			InitArray(steps);
		}
		break;
	}
	case 'w': {
		SaveOBJ(mesh.faces, mesh.vertices, "mesh_tri.obj", true); break;
	}

	case 'b': {
		if (branch_type < 4) {
			branch_type += 1;
		}
		else {
			branch_type = BINARY;
		}
		InitArray(steps);		
		break;
	}

	}
	cout << "[points]=[" << steps << "]" << endl;
	glutPostRedisplay();
}

void DirectionKeys(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP: {
		if (width > scale/10) {
			width -= 0.01f;
			InitArray(steps);	
		}
		break;
	}
	case GLUT_KEY_DOWN: {		
		if (width < scale) {
			width += scale / 10;
		}
		if(width >= scale){
			width -= scale / 10;
		}
		InitArray(steps);
		break;		
	}
	case GLUT_KEY_LEFT: {
		if (length > 1.0f) {
			length -= 1.0f;
			InitArray(steps);
			
		}
		break;
	}
	case GLUT_KEY_RIGHT: {
		if (length < MAXLEN) {
			length += 1.0;
			InitArray(steps);
		}		
		break;
	}
	}
	cout << "[width]=[" << width << "]" << endl;
	cout << "[length]=[" << length << "]" << endl;
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
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Kbd); //+ and -
	glutSpecialUpFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpecialFunc(DirectionKeys);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	InitArray(steps);
	glutMainLoop();
	return 0;
}
