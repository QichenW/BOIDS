//
// Created by Qichen on 11/12/16.
//

#ifndef BOUNCINGBALLS_COLLISIONDETECTOR_H
#define BOUNCINGBALLS_COLLISIONDETECTOR_H


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
class CollisionDetector {
public:
    static void detectAll(Object **all, int numberOfObjects);
    static const int NUMBER_OF_WALLS;

    static const int X_DIRECTION;
    static const int Y_DIRECTION;
    static const int Z_DIRECTION;
    static const GLfloat ratio;

    static void fakeTheRotation(Prey *ball, int hitInTheDirectionOfWhichAxe);

    static void reverseVelocity(GLfloat *veloInOneDirection);

    static void applyCollisionAvoidance(Prey *p1, Prey *p2);
};


#endif //BOUNCINGBALLS_COLLISIONDETECTOR_H
