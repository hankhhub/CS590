#ifndef __FACE_H__
#define __FACE_H__
#include "math/vect3d.h"
#include <vector>

class Face{
public:
	//constructors for the mesh
	Face();
	Face(int a, int b, int c, int d);
	Vect3d centroid;
	// four vertices(indices) of face
	int indices[4];
};

typedef std::vector<Face> Faces;
#endif