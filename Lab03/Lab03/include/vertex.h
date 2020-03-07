#pragma once
#include "math/vect3d.h"
#include <unordered_map>
#include <unordered_set>

typedef std::unordered_set<int> AdjacentVertices;
typedef std::vector<unsigned int> AdjacentFaces;
typedef std::unordered_set<int> AdjFaceSet;

struct Edge {
	Vect3d edgePoint;
	int edgePointID;
};

typedef std::unordered_map<int, Edge> EdgeMap;

class Vertex {
public:
	Vertex() {};
	Vertex(Vect3d vertex);

	Vect3d v;
	AdjacentVertices adjV;
	AdjacentFaces adjF;
	AdjFaceSet fSet;
	Vect3d faceCentroidSum;
	EdgeMap edgeMap;
};

typedef std::vector<Vertex> Vertices;