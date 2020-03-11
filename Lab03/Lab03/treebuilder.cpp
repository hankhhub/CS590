#include "treebuilder.h"

void CubeVertices(Mesh* m, Vect3d c, float delta, RotationAxis axis, int angle) {

	int startx = c.v[0]; int endx = c.v[0];
	int starty = c.v[1]; int endy = c.v[1];
	int startz = c.v[2]; int endz = c.v[2];
	for (int x = -1; x <= 1; x += 2)
		for (int y = -1; y <= 1; y += 2)
			for (int z = -1; z <= 1; z += 2)
			{
				Vect3d cubeVertex = Vect3d(c.v[0] + delta * x, c.v[1] + delta * y, c.v[2] + delta * z);
				if (axis == ROTX) {
					m->vertices.push_back(cubeVertex.GetRotatedX(angle));
				}
				else if (axis == ROTY) {
					m->vertices.push_back(cubeVertex.GetRotatedY(angle));
				}
				else if (axis == ROTZ) {
					m->vertices.push_back(cubeVertex.GetRotatedZ(angle));
				}
				else {
					m->vertices.push_back(cubeVertex);
				}

			}
}
void CubeFaces(Mesh *m, Face initface) {
	int p1 = initface.indices[0];
	int p2 = initface.indices[1];
	int p3 = initface.indices[2];
	int p4 = initface.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //left
	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2)); //right
	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2)); //bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4)); //top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}

void TreeTipZ(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //left
	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2)); //right
	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2)); //bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4)); //top
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}

void TreeTipNegZ(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //left
	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2)); //right
	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2)); //bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4)); //top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
}

void TreeTipX(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 + 4, p4 + 4)); //right
	m->faces.push_back(Face(p1 , p2 , p3 +2, p4 +2)); //bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4)); //top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}

void TreeTipNegX(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //left
	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2)); //bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4)); //top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}

void TreeTipY(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //left
	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2)); //right
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4)); //top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}

void TreeTipNegY(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //left
	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2)); //right
	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2)); //bottom
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}

void ConnectLeft(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); // connect bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 2, p4 + 2)); // connect top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 1, p4 )); //connect back 
	m->faces.push_back(Face(p1 + 3, p2 , p3, p4 + 3)); //connect front
}

void ConnectRight(Mesh *m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); // connect bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 2, p4 + 2)); // connect top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 1, p4)); //connect back 
	m->faces.push_back(Face(p1 + 3, p2, p3, p4 + 3)); //connect front
}

void TrunkX(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2));
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4));
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2));
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1));

	
}

void ConnectY(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //connect left
	m->faces.push_back(Face(p1 + 5, p2 + 3, p3 + 3, p4 + 5));  //connect right
	m->faces.push_back(Face(p1, p2 + 3, p3 + 3, p4)); //connect back
	m->faces.push_back(Face(p1 + 1, p2 + 4, p3 + 4, p4 + 1)); //connect front
}
void TrunkY(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //left
	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2)); //right
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front

}
void TrunkYLeft(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2)); //right
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}

void TrunkYRight(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); //left
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}
void TrunkZ(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4));
	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2));
	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2));
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4));
}


void BranchLeft(Mesh *m, Vect3d center, float scale, RotationAxis axis, int angle, int segments, float * dim) {
	Vect3d branch = center;
	float len = dim[0];
	float width = dim[1];
	float steps = width / segments;
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	for (int i = 0; i < segments; i++) {
		branch.v[0] -= scale * len;
		CubeVertices(m, branch, scale - width, axis, angle);
		ConnectLeft(m, Face(a - 8, b - 8, c + 2, d + 2));
		TrunkX(m, Face(a, b, c, d));
		a += 8; b += 8; c += 8; d += 8;
		width += steps;
	}

	branch.v[0] -= scale * len;
	CubeVertices(m, branch, scale - width, axis, angle);
	ConnectLeft(m, Face(a - 8, b - 8, c + 2, d + 2));
	TreeTipNegX(m, Face(a, b, c, d));
}

void BranchRight(Mesh *m, Vect3d center, float scale, RotationAxis axis, int angle, int segments, float *dim) {
	Vect3d branch = center;
	float len = dim[0];
	float width = dim[1];
	float steps = width / segments;
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	for (int i = 0; i < segments; i++) {
		branch.v[0] += scale * len;
		CubeVertices(m, branch, scale - width, axis, angle);
		ConnectRight(m, Face(a - 4, b - 4, c - 2, d - 2));
		TrunkX(m, Face(a, b, c, d));
		a += 8; b += 8; c += 8; d += 8;
		width += steps;
	}

	branch.v[0] += scale * 2.0f;
	CubeVertices(m, branch, scale - width, axis, angle);
	ConnectRight(m, Face(a - 4, b - 4, c - 2, d - 2));
	TreeTipX(m, Face(a, b, c, d));
}

void BranchTop(Mesh *m, Vect3d center, float scale, RotationAxis axis, int angle, int segments) {
	Vect3d branch = center;
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	for (int i = 0; i < segments; i++) {
		branch.v[1] += scale * 2.0f;
		CubeVertices(m, branch, scale - 0.065f, axis, angle);
		ConnectY(m, Face(a - 6, b - 6, c - 2, d - 2));
		TrunkY(m, Face(a, b, c, d));
		a += 8; b += 8; c += 8; d += 8;
	}

	branch.v[1] += scale * 3.0f;
	CubeVertices(m, branch, scale - 0.07f, axis, angle);
	ConnectY(m, Face(a - 6, b - 6, c - 2, d - 2));
	TreeTipY(m, Face(a, b, c, d));
}

void LeftBranchConnect(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect) {
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	center->v[1] += scale * 4.0f;
	CubeVertices(m, *center, scale - 0.02f, axis, angle);
	ConnectY(m, Face(connect - 6, connect - 5, c - 2, d - 2));
	TrunkYLeft(m, Face(a, b, c, d));
}



void RightBranchConnect(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect) {
	center->v[1] += scale * 5.0f;
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	CubeVertices(m, *center, scale - 0.02f, axis, angle);
	ConnectY(m, Face(connect - 6, connect - 5, b, a));
	TrunkYRight(m, Face(a, b, c, d));
}

void TrunkBinary(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];
	
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4)); //top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2)); //back
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1)); //front
}

void BinaryBranchConnect(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect, float *dim) {
	center->v[1] += scale * 5.0f;
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	CubeVertices(m, *center, scale - 0.02f, axis, angle);
	ConnectY(m, Face(connect - 6, connect - 5, b, a));
	TrunkBinary(m, Face(a, b, c, d));
}

void LeftToBot(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); // connect bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4)); // connect top
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4)); //connect back 
	m->faces.push_back(Face(p1 + 1, p2 + 2, p3 + 4, p4 + 1)); //connect front
}
void BinaryConnectLeft(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect, float len, float width, bool end) {
	center->v[1] += 0.2f;
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	CubeVertices(m, *center, scale - width, axis, angle);	
	LeftToBot(m, Face(connect - 8, connect - 7, b, a));
	if (end) {
		TreeTipY(m, Face(a, b, c, d));
	}
	else {
		TrunkBinary(m, Face(a, b, c, d));
	}	
}

void RightToBot(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1, p2, p3, p4)); // connect bottom
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3-4, p4-4)); // connect top
	m->faces.push_back(Face(p1, p2 + 1, p3 - 5, p4)); //connect back 
	m->faces.push_back(Face(p1 + 1, p2 + 2, p3 -4, p4 + 1)); //connect front
}
void BinaryConnectRight(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect, float len, float width, bool end) {
	center->v[1] += 0.5f;
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	CubeVertices(m, *center, scale - width, axis, angle);
		
	RightToBot(m, Face(connect - 4, connect - 3, b + 4, a + 4));
	if (end) {
		TreeTipY(m, Face(a, b, c, d));
	}
	else {
		TrunkBinary(m, Face(a, b, c, d));
	}
}
void BranchTip(Mesh *m, Vect3d center, float scale, RotationAxis axis, int angle, int connect) {
	Vect3d branch = center;	
	branch.v[1] += scale * 3.0f;
	int a = m->vertices.size(); int b = a + 1; int c = a + 3; int d = a + 2;
	CubeVertices(m, branch, scale - 0.07f, axis, angle);	
	ConnectY(m, Face(connect - 6, connect - 5, b, a));
	TreeTipY(m, Face(a, b, c, d));
}

void RecursiveBranch(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect, float len, float width, int n, float steps) {

	angle += 5;
	Vect3d left = Vect3d(center->v[0] - (scale * len), center->v[1], center->v[2]);
	Vect3d right = Vect3d(center->v[0] + (scale* len), center->v[1] , center->v[2]);
	if (n == 1) {
		//BranchTip(m, *center, scale, axis, angle, connect);
		BinaryConnectLeft(m, &left, scale, axis, -angle, connect, len, width, true);
		BinaryConnectRight(m, &right, scale, axis, angle, connect, len, width, true);
		return;
	}
	
	BinaryConnectLeft(m, &left, scale, axis, -angle, connect, len, width, false);
	int connectL = m->vertices.size();
	BinaryConnectRight(m, &right, scale, axis, angle, connect, len, width, false);
	int connectR = m->vertices.size();
	RecursiveBranch(m, &left, scale, axis, angle, connectL, len, width+steps, n - 1, steps);
	RecursiveBranch(m, &right, scale, axis, angle, connectR, len, width+steps, n - 1, steps);
}
