//
// Created by Qichen on 1/4/17.
//

#ifndef BOIDS_CYLINDER_H
#define BOIDS_CYLINDER_H
using namespace std;
#include "Object.h"
#include "VectorCalculation.h"
#include <vector>
#include "matrix/RotationHelper.h"
#include <cstdlib>

#if defined(__APPLE__)
#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif
class Cylinder {
public:
    Cylinder(int size, GLfloat *trunkCentroidPosition);
    static void setBottomOfTankY(GLfloat y);

    static vector<Cylinder> cylinders;
    GLfloat axisCoordinateX;
    GLfloat axisCoordinateZ;
    GLfloat yPosOfTopSurface;
    GLfloat radius;
    static int amount;
private:
    static const GLfloat L_TRUNK_HEIGHT;
    static const GLfloat M_TRUNK_HEIGHT;
    static const GLfloat S_TRUNK_HEIGHT;
    static const GLfloat RATIO_H;
    static const GLfloat RATIO_R;
    static GLfloat bottomOfTankY;
};


#endif //BOIDS_CYLINDER_H
