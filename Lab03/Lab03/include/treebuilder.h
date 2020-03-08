#pragma once
#include "vertex.h"
#include "face.h"
struct Mesh
{
	Vertices vertices;
	Faces faces;
};

enum RotationAxis { ROTX, ROTY, ROTZ, NONE };

void CubeVertices(Mesh* m, Vect3d c, float delta, RotationAxis axis, int angle);
void CubeFaces(Mesh *m, Face initface);

void TreeTipZ(Mesh*m, Face curFace);
void TreeTipNegZ(Mesh*m, Face curFace);
void TreeTipX(Mesh*m, Face curFace);
void TreeTipNegX(Mesh*m, Face curFace);
void TreeTipY(Mesh*m, Face curFace);
void TreeTipNegY(Mesh*m, Face curFace);

void TrunkX(Mesh*m, Face curFace);
void TrunkY(Mesh*m, Face curFace);
void ConnectY(Mesh*m, Face curFace);
void TrunkZ(Mesh*m, Face curFace);