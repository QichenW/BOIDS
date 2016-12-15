//
// Created by Qichen on 11/12/16.
//

#ifndef BOIDS_PREY_H
#define BOIDS_PREY_H
#include "Object.h"
#include "Geometry.h"
#include "matrix/RotationHelper.h"
#include <cstdlib>
#if defined(__APPLE__)
#include <GLUT/glut.h>


#else
#include <GL/glut.h>
#endif

class Prey: public Object {
public:

    Prey(int oId, int lId, bool isF, GLfloat *orienttn, GLfloat *translatn, GLfloat *velocity,
             GLfloat *angularVelocity, GLfloat r);
    void updateFlattenedTransformationMatrix(GLfloat t);

    GLfloat radius;
    bool detour;
    GLfloat getVelocityIn(const int direction);
    GLfloat velocity[3];
    GLfloat directionOfCollision[3];
    void fall();
    static const GLfloat VELOCITY_CONSTANT;
    void setVelocity(GLfloat *v);
    void hasDetour(bool i);
    GLfloat unitTravelDirection[3];
private:
    static const int X_DIRECTION;
    static const int Y_DIRECTION;
    static const int Z_DIRECTION;

    void setUnitTravelDirection();

    bool isPreyDead;

    void boidSeperation();

    void boidAlignment();

    void boidCohesion();
};

#endif //BOIDS_PREY_H
