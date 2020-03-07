#include "treebuilder.h"

void CubeVertices(Mesh* m, Vect3d c, float delta) {
	float x, y, z;
	float startx = c.v[0] - delta; float endx = c.v[0] + delta;
	float starty = c.v[1] - delta; float endy = c.v[1] + delta;
	float startz = c.v[2] - delta; float endz = c.v[2] + delta;

	for (x = startx; x <= endx; x += 2 * delta)
		for (y = starty; y <= endy; y += 2 * delta)
			for (z = startz; z <= endz; z += 2 * delta)
			{
				m->vertices.push_back(Vect3d(x, y, z));
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

	m->faces.push_back(Face(p1 + 6, p2 + 6, p3 + 2, p4 + 2)); //right
	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2)); //bottom
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

void ConnectX(Mesh*m, Face curFace) {
	int p1 = curFace.indices[0];
	int p2 = curFace.indices[1];
	int p3 = curFace.indices[2];
	int p4 = curFace.indices[3];

	m->faces.push_back(Face(p1 + 4, p2 + 4, p3 - 2, p4 - 2));
	m->faces.push_back(Face(p1 + 2, p2 + 2, p3 + 4, p4 + 4));
	m->faces.push_back(Face(p1, p2 + 1, p3 + 3, p4 + 2));
	m->faces.push_back(Face(p1 + 5, p2 + 6, p3, p4 - 1));
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
	m->faces.push_back(Face(p1, p2 + 3, p3 + 3, p4 )); //connect back
	m->faces.push_back(Face(p1 +1, p2 + 4, p3 + 4, p4 + 1)); //connect front

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