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

    Prey(int oId, int lId, GLfloat om, bool isF, GLfloat *orienttn, GLfloat *translatn, GLfloat *velocity,
             GLfloat *angularVelocity, GLfloat r);

    void updateAcclrtn();

    void updateFlattenedTransformationMatrix(GLfloat t);

    GLfloat radius;

    GLfloat getVelocityIn(const int direction);
    GLfloat velocity[3];
    GLfloat directionOfCollision[3];
    GLfloat angluarVelo[3];

    void fall();

private:

    GLfloat acceleration[3];
    GLfloat angularAcclrtn[3];
    //TODO this is not useful
    GLfloat unitTravelDirection[3];

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
