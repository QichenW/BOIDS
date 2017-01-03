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

class Fish: public Object {
public:
    static int amount;
    Fish(int oId, GLuint hLID, bool isF, GLfloat *orienttn, GLfloat *translatn, GLfloat *velocity,
              GLfloat r, GLuint bLId, GLuint tListId, int scaleM);
    void updateFlattenedTransformationMatrix(GLfloat t);

    GLfloat radius;
    GLfloat vicinityRadius;
    GLfloat getVelocityIn(const int direction);
    GLfloat velocity[3];
    GLfloat acclrtn[3];
    GLfloat apprxCentroid[3];
    GLfloat bodyLocalRotation[3] = {};
    GLfloat tailLocalRotation[3] = {};

    void getCombinedDesires();
    void setIndividualCentroid(GLfloat x, GLfloat y, GLfloat z);

    vector<Fish *> vectorOfNeighbours;

    static void updateFlockCentroid(GLfloat sumOfPos[3]);

    static void updateGoal();

    static GLfloat biggestRadius;
    GLuint bodyListId;
    GLuint tailListId;
    GLfloat bodyAlignFlat1[16];
    GLfloat bodyAlignFlat2[16];
    GLfloat tailAlignFlat1[16];
    GLfloat tailAlignFlat2[16];

    void rotateBodyAndTail();

private:
    static const int X_DIRECTION;
    static const int Y_DIRECTION;
    static const int Z_DIRECTION;
    static const GLfloat MAX_VELOCITY;
    static const GLfloat DEFAULT_DIRECTION[3];
    static const GLfloat BODY_UNIT_ALIGN_1[3];
    static const GLfloat BODY_UNIT_ALIGN_2[3];
    static const GLfloat TAIL_UNIT_ALIGN_1[3];
    static const GLfloat TAIL_UNIT_ALIGN_2[3];
    static const GLfloat BODY_ANGLE_MARGIN;
    static GLfloat goal[3];
    static GLfloat flockCentroid[3];
    void setAcclrtnWithDesires(GLfloat *sDesire, GLfloat *aDesire, GLfloat *cDesire);

    void rotateBodyAndSetTranslation(GLfloat *newVelo);
    void setAlignFlatMatrices();

    int scaleMultiplier;
    GLfloat deltaAngle = 8; //may change
};

#endif //BOIDS_PREY_H
