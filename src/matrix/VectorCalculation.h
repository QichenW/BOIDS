//
// Created by Qichen on 11/13/16.
//

#ifndef BOIDS_VECTORCALCULATION_H
#define BOIDS_VECTORCALCULATION_H

#include "boid/Object.h"
#include "boid/Prey.h"
#include "matrix/RotationHelper.h"
#include <cmath>
#if defined(__APPLE__)
#include <GLUT/glut.h>


#else
#include <GL/glut.h>
#endif

class VectorCalculation {

public:
    static GLfloat getDistance(GLfloat *pos1, GLfloat *pos2);

    static void getUnitDirection(GLfloat *des, GLfloat *src);
    static GLfloat getMagnitude(GLfloat *v);
    static bool isZeroVector(GLfloat *v);

    static void getCrossProduct(GLfloat *des, GLfloat *from, GLfloat *to);

    static bool areTwoVectorSameDirection(GLfloat v1[3], GLfloat v2[3]);

    static GLfloat dotProduct(GLfloat *v1, GLfloat *v2);
};


#endif //BOIDS_VECTORCALCULATION_H
