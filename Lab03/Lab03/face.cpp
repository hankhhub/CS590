#include "face.h"
Face::Face() {
}

Face::Face(int p1, int p2, int p3, int p4) {
	this->indices[0] = p1;
	this->indices[1] = p2;
	this->indices[2] = p3;
	this->indices[3] = p4;
}