//
// Created by Qichen on 11/12/16.
//

#ifndef BOIDS_PREY_H
#define BOIDS_PREY_H
using namespace std;
#include "Object.h"
#include "Geometry.h"
#include <vector>
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
              GLfloat r);
    void updateFlattenedTransformationMatrix(GLfloat t);

    GLfloat radius;
    GLfloat vicinityRadius;
    GLfloat getVelocityIn(const int direction);
    GLfloat velocity[3];
    GLfloat acclrtn[3];
    void fall();
    static const GLfloat VELOCITY_CONSTANT;
    void setVelocity(GLfloat *v);
    GLfloat unitTravelDirection[3];

    static void rotateVelo(Object **objects, int numberOfObjects);

    vector<Prey *> vectorOfNeighbours;
private:
    static const int X_DIRECTION;
    static const int Y_DIRECTION;
    static const int Z_DIRECTION;

    void setUnitTravelDirection();

    bool isPreyDead;

    void getDesires();

};

#endif //BOIDS_PREY_H
