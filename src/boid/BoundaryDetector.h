//
// Created by Qichen on 11/12/16.
//

#ifndef BOIDS_BOUNDARYDETECTOR_H
#define BOIDS_BOUNDARYDETECTOR_H


#include "Object.h"
#include "Prey.h"
#include "matrix/RotationHelper.h"
#include "Geometry.h"
#include <cmath>
#include <iostream>
#if defined(__APPLE__)
#include <GLUT/glut.h>


#else
#include <GL/glut.h>
#endif
using namespace std;
class BoundaryDetector {
public:
    static void detectAll(Object **all, int numberOfObjects);
    static const int NUMBER_OF_WALLS;
    static const int X_DIRECTION;
    static const int Y_DIRECTION;
    static const int Z_DIRECTION;
    static void halt(GLfloat *veloOneDir, GLfloat *acclrtnOneDir);
};


#endif //BOIDS_BOUNDARYDETECTOR_H