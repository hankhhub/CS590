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
void ConnectLeft(Mesh*m, Face curFace);
void ConnectRight(Mesh *m, Face curFace);
void TrunkY(Mesh*m, Face curFace);
void TrunkYLeft(Mesh*m, Face curFace);
void TrunkYRight(Mesh*m, Face curFace);
void ConnectY(Mesh*m, Face curFace);
void TrunkZ(Mesh*m, Face curFace);

void BranchTop(Mesh *m, Vect3d center, float scale, RotationAxis axis, int angle, int segments);
void BranchRight(Mesh *m, Vect3d center, float scale, RotationAxis axis, int angle, int segments, float *dim);
void BranchLeft(Mesh *m, Vect3d center, float scale, RotationAxis axis, int angle, int segments, float *dim);

void LeftBranchConnect(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect);
void RightBranchConnect(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect);

void TrunkBinary(Mesh*m, Face curFace);
void BinaryBranchConnect(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect, float *dim);
void RecursiveBranch(Mesh *m, Vect3d *center, float scale, RotationAxis axis, int angle, int connect, float len,float width, int n, float steps);

void LeftToBot(Mesh*m, Face curFace);
void RightToBot(Mesh*m, Face curFace);
void BranchTip(Mesh *m, Vect3d center, float scale, RotationAxis axis, int angle, int connect);