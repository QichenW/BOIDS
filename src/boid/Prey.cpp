//
// Created by Qichen on 11/12/16.
//
#include "Prey.h"
const int Prey::X_DIRECTION = 0;
const int Prey::Y_DIRECTION = 1;
const int Prey::Z_DIRECTION = 2;
const GLfloat Prey::VELOCITY_CONSTANT = 10;

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
     GLfloat *translatn, GLfloat * velocity, GLfloat r)
        :Object(oId, lId, isF, orienttn, translatn){ // call the base class constructor first
    int i;
    radius = r;
    for (i = 0 ; i < 3; i++){
        *(acclrtn + i) = 0;
        *(Prey::velocity + i) = *(velocity + i);
    }
    isPreyDead = false;
    isPredator = false;
    //TODO subject to change
    vicinityRadius = 4 * radius;
    setUnitTravelDirection();
}

/***
 * update translation, rotation, velocity and angular velocity, then get the transformation matrix
 * @param t - delta t
 */
void Prey::updateFlattenedTransformationMatrix(GLfloat t) {
    int i;
    for (i = 0 ; i < 3; i++){
        *(translation + i) += *(velocity + i) * t;
        *(velocity + i) += *(acclrtn + i);
    }
    //TODO test this maintain constant velocity
    VectorCalculation::getUnitDirection(velocity, velocity);
    for(i = 0; i < 3; i++){
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
    switch (direction){
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
void Prey::setAcclrtnWithDesires(){
    //TODO
}

//TODO change this function - get combinded desires
void Prey::getCombinedDesires(Object **objects, int numberOfObjects) {
    int i;
    unsigned long j;
    Prey * currentPrey;
    for(i = 0; i < numberOfObjects; i++){
        if( objects[i] ->isFixed){
            continue;
        } else if( objects[i]->isPredator){
            continue;
        }
        currentPrey = (Prey *) objects[i];
        int noOfNeighbours = (int) currentPrey->vectorOfNeighbours.size();
        if(noOfNeighbours == 0){
            continue;
        }
        // alignment, cohesion
        GLfloat alignmentDesire[3]={}, magnitudeA = 0;
        GLfloat cohesionDesire[3]={}, magnitudeC = 0;
        //TODO determine the separation desire and its magnitude
        GLfloat separationDesire[3]={}, magnitudeS = 0;
        for(j = 0; j < noOfNeighbours; j++){
            cohesionDesire[0] += currentPrey->vectorOfNeighbours.at(j)->translation[0];
            cohesionDesire[1] += currentPrey->vectorOfNeighbours.at(j)->translation[1];
            cohesionDesire[2] += currentPrey->vectorOfNeighbours.at(j)->translation[2];
            alignmentDesire[0] += currentPrey->vectorOfNeighbours.at(j)->velocity[0];
            alignmentDesire[1] += currentPrey->vectorOfNeighbours.at(j)->velocity[1];
            alignmentDesire[2] += currentPrey->vectorOfNeighbours.at(j)->velocity[2];
        }
        magnitudeA = VectorCalculation::getMagnitude(alignmentDesire);
        for (j = 0; j <3 ; j++) {
            alignmentDesire[j] *= VELOCITY_CONSTANT/magnitudeA;
        }
        //TODO combine all desires

    }

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
    if(magnitude_axis == 0) {
            //if the two vectors are in same direction, no needs to detour
            if(VectorCalculation::areTwoVectorSameDirection(newVelo, unitTravelDirection)){
                return;
            }
            // if the two vectors are opposite
            axis_q[0] = 0; axis_q[2] = 0; axis_q[1] = 1;
            halfAngle = (GLfloat) (M_PI / 2);
        } else {
            // normalize the axis vector
            for (j = 0; j < 3; j++) {
                axis_q[j] /= magnitude_axis;
            }
            GLfloat sinValue2 = magnitude_axis / VELOCITY_CONSTANT;
            halfAngle = asinf(sinValue2) / 2;

        }
    GLfloat quatForAlignment[4] ={cosf(halfAngle), sinf(halfAngle) * axis_q[0],sinf(halfAngle) * axis_q[1],
                                         sinf(halfAngle) * axis_q[2]};
    //TODO this function is tested
    setFlattenedTransformationMatrix(RotationHelper::updateFlattenedMatrixWithQuaternion(
                getFlattenedTransformationMatrix(),quatForAlignment));
}
