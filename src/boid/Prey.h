//
// Created by Qichen on 11/12/16.
//

#ifndef BOIDS_PREY_H
#define BOIDS_PREY_H
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

class Prey: public Object {
public:
    static int amount;
    Prey(int oId, int lId, bool isF, GLfloat *orienttn, GLfloat *translatn, GLfloat *velocity,
              GLfloat r);
    void updateFlattenedTransformationMatrix(GLfloat t);

    GLfloat radius;
    GLfloat vicinityRadius;
    GLfloat getVelocityIn(const int direction);
    GLfloat velocity[3];
    GLfloat acclrtn[3];

    void getCombinedDesires();

    vector<Prey *> vectorOfNeighbours;

    static void updateCentroid(GLfloat sumOfPos[3]);

    static void updateGoal();

    static GLfloat biggestRadius;
private:
    static const int X_DIRECTION;
    static const int Y_DIRECTION;
    static const int Z_DIRECTION;
    static const GLfloat MAX_VELOCITY;
    static const GLfloat DEFAULT_DIRECTION[3];

    void setAcclrtnWithDesires(GLfloat *sDesire, GLfloat *aDesire, GLfloat *cDesire);

    void rotateBodyAndSetTranslation(GLfloat *newVelo);

    static GLfloat centroid[3];
    static GLfloat goal[3];
};

#endif //BOIDS_PREY_H
