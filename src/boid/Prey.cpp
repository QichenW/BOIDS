//
// Created by Qichen on 11/12/16.
//
#include "Prey.h"

const int Prey::X_DIRECTION = 0;
const int Prey::Y_DIRECTION = 1;
const int Prey::Z_DIRECTION = 2;
const GLfloat Prey::VELOCITY_CONSTANT = 10;
const GLfloat Prey::ACCLRTN_CONSTANT = 50;

/**
 *  Prey is a subclass of Object
 * @param oId - id
 * @param lId - polygon list id
 * @param om - mass
 * @param isF - is fixed (should be false)
 * @param orienttn - initial orientation
 * @param translatn - initial translation
 * @param velocity - initial velocity
 * @param angularVelocity - initial angular velocity
 * @param r - radius
 */
Prey::Prey(int oId, int lId, bool isF, GLfloat *orienttn,
           GLfloat *translatn, GLfloat *velocity, GLfloat r)
        : Object(oId, lId, isF, orienttn, translatn) { // call the base class constructor first
    int i;
    radius = r;
    unitTravelDirection[0] = 1, unitTravelDirection[1] = 0, unitTravelDirection[2] = 0;
    rotateBody(velocity);
    for (i = 0; i < 3; i++) {
        *(acclrtn + i) = 0;
        *(Prey::velocity + i) = *(velocity + i);
    }
    isPreyDead = false;
    isPredator = false;
    //TODO subject to change
    vicinityRadius = 10 * radius;
    setUnitTravelDirection();
}

/***
 * update translation, rotation, velocity and angular velocity, then get the transformation matrix
 * @param t - delta t
 */
void Prey::updateFlattenedTransformationMatrix(GLfloat t) {
    int i;
    for (i = 0; i < 3; i++) {
        *(translation + i) += *(velocity + i) * t;
        *(velocity + i) += *(acclrtn + i) * t;
    }
    //TODO test this maintain constant velocity
    VectorCalculation::getUnitDirection(velocity, velocity);
    for (i = 0; i < 3; i++) {
        *(velocity + i) *= VELOCITY_CONSTANT;
        flattenedTransformationMatrix[12 + i] = translation[i];
    }
    // rotate body by referring to old travel direction and new velocity
    rotateBody(velocity);
    // update travel direction
    setUnitTravelDirection();
}

/**
 * @param direction indicate x or y or z direction
 * @return  the velocity in one direction
 */
GLfloat Prey::getVelocityIn(const int direction) {
    switch (direction) {
        case X_DIRECTION:
            return *velocity;
        case Y_DIRECTION:
            return *(velocity + 1);
        case Z_DIRECTION:
            return *(velocity + 2);
        default:
            return 0;
    }
}

/***
 * set unit travel direction, which is used to determine the heading of boids.
 */
void Prey::setUnitTravelDirection() {
    VectorCalculation::getUnitDirection(unitTravelDirection, velocity);
}

/**
 * When this function is called, it means the prey is dead.
 */
void Prey::fall() {
//TODO fall
    isPreyDead = true;
}


/**
 * get the combined desire as acceleration
 */
void Prey::setAcclrtnWithDesires(GLfloat *sDesire, GLfloat *aDesire, GLfloat *cDesire) {
    //TODO change the weights and magnitude of acceleration, now are 1 2 1
    GLfloat combinedDesire[3] ={};
    int i;
    for(i = 0; i< 3; i++){
        combinedDesire[i] += *(sDesire + i) + *(aDesire + i) * 2 + *(cDesire + i);
    }
    VectorCalculation::getUnitDirection(combinedDesire, combinedDesire);
    for( i = 0; i < 3; i++){
        *(acclrtn + i) = *(combinedDesire + i) * ACCLRTN_CONSTANT;
    }
}

//TODO change this function - get combinded desires
void Prey::getCombinedDesires() {
    unsigned long j;
    int noOfNeighbours = (int) vectorOfNeighbours.size();
    // alignment, cohesion, separation
    GLfloat alignmentDesire[3] = {}, cohesionDesire[3] = {}, separationDesire[3] = {};
    Prey *currentNeighbour;
    //accumulatively build up the desires
    for (j = 0; j < noOfNeighbours; j++) {
        currentNeighbour = vectorOfNeighbours.at(j);
        alignmentDesire[0] += currentNeighbour->velocity[0] - velocity[0];
        alignmentDesire[1] += currentNeighbour->velocity[1] - velocity[1];
        alignmentDesire[2] += currentNeighbour->velocity[2] - velocity[2];
        if (VectorCalculation::getDistance(translation, currentNeighbour->translation) <=
            2 * radius + 2 * currentNeighbour->radius) {
            separationDesire[0] += translation[0] - currentNeighbour->translation[0];
            separationDesire[1] += translation[1] - currentNeighbour->translation[1];
            separationDesire[2] += translation[2] - currentNeighbour->translation[2];
        } else {
            cohesionDesire[0] += currentNeighbour->translation[0] - translation[0];
            cohesionDesire[1] += currentNeighbour->translation[1] - translation[1];
            cohesionDesire[2] += currentNeighbour->translation[2] - translation[2];
        }
    }
    VectorCalculation::getUnitDirection(alignmentDesire, alignmentDesire);
    VectorCalculation::getUnitDirection(cohesionDesire, cohesionDesire);
    VectorCalculation::getUnitDirection(separationDesire, separationDesire);
    //TODO combine all desires
    setAcclrtnWithDesires(separationDesire, alignmentDesire, cohesionDesire);

}

/**
 * rotate the body to new orientation that is determined by new velocity
 * @param newVelo
 */
void Prey::rotateBody(GLfloat *newVelo) {
    int j;
    GLfloat axis_q[3] = {};
    VectorCalculation::getCrossProduct(axis_q, unitTravelDirection, newVelo);
    GLfloat magnitude_axis = VectorCalculation::getMagnitude(axis_q);
    GLfloat halfAngle;
    if (magnitude_axis == 0) {
        //if the two vectors are in same direction, no needs to detour
        if (VectorCalculation::areTwoVectorSameDirection(newVelo, unitTravelDirection)) {
            return;
        }
        // if the two vectors are opposite
        axis_q[0] = 0;
        axis_q[2] = 0;
        axis_q[1] = 1;
        halfAngle = (GLfloat) (M_PI / 2);
    } else {
        // normalize the axis vector
        for (j = 0; j < 3; j++) {
            axis_q[j] /= magnitude_axis;
        }
        GLfloat sinValue2 = magnitude_axis / VELOCITY_CONSTANT;
        halfAngle = asinf(sinValue2) / 2;

    }
    GLfloat quatForAlignment[4] = {cosf(halfAngle), sinf(halfAngle) * axis_q[0], sinf(halfAngle) * axis_q[1],
                                   sinf(halfAngle) * axis_q[2]};
    //TODO this function is tested
    setFlattenedTransformationMatrix(RotationHelper::updateFlattenedMatrixWithQuaternion(
            getFlattenedTransformationMatrix(), quatForAlignment));
}
