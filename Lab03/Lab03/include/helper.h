#ifndef __helper_h__
#define __helper_h__

#include "helper.h"
#include "math/vect3d.h"

void GLMessage(char *message);

void Reshape(int w, int h);

void DrawLine(Vect3d a, Vect3d b, Vect3d color);

void DrawPoint(Vect3d a, Vect3d color);

void CoordSyst();
#endif