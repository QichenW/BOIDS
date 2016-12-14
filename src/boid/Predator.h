//
// Created by Qichen on 12/13/16.
//

#ifndef BOIDS_PREDATOR_H
#define BOIDS_PREDATOR_H
#include "Object.h"
#include "Geometry.h"
#include "matrix/RotationHelper.h"
#include <cstdlib>
#if defined(__APPLE__)
#include <GLUT/glut.h>


#else
#include <GL/glut.h>
#endif

class Predator: public Object {
public:

    Predator(int oId, int lId, bool isF, GLfloat *orienttn, GLfloat *translatn, GLfloat *velocity,
         GLfloat *angularVelocity, GLfloat r);

    void updateAcclrtn();

    void updateFlattenedTransformationMatrix(GLfloat t);

    void hunt();

    void kill(Prey prey);
    GLfloat radius;

    GLfloat getVelocityIn(const int direction);
    GLfloat velocity[3];
    GLfloat directionOfCollision[3];
    GLfloat angluarVelo[3];

private:

    GLfloat acceleration[3];
    GLfloat angularAcclrtn[3];
    //TODO this is not useful
    GLfloat unitTravelDirection[3];

    static const int X_DIRECTION;
    static const int Y_DIRECTION;
    static const int Z_DIRECTION;

    void setUnitTravelDirection();

};
#endif //BOIDS_PREDATOR_H
