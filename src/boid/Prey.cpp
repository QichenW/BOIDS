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
    setUnitTravelDirection();
    int i;
    for (i = 0 ; i < 3; i++){
        *(translation + i) += *(velocity + i) * t;
        *(velocity + i) += *(acclrtn + i);
    }
    //TODO test this maintain constant velocity
    GLfloat veloTempMagnitude = Geometry::getMagnitude(velocity);
    for(i = 0; i < 3; i++){
        *(velocity + i) *= VELOCITY_CONSTANT/veloTempMagnitude;
        flattenedTransformationMatrix[12 + i] = translation[i];
    }
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
    Geometry::getUnitDirection(unitTravelDirection, velocity);
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
void Prey::getDesires(){
    //TODO
}

void Prey::setVelocity(GLfloat *v) {
    //TODO test this
    int i;
    for(i = 0; i < 3; i++){
        velocity[i] = *(v + i);
    }
}

void Prey::rotateVelo(Object **objects, int numberOfObjects) {
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
        GLfloat tempVelo[3]={}, magnitude_tempVelo = 0;
        GLfloat tempCenter[3]={}, magnitude_tempCenter = 0;
        for(j = 0; j < noOfNeighbours; j++){
            tempCenter[0] += currentPrey->vectorOfNeighbours.at(j)->translation[0];
            tempCenter[1] += currentPrey->vectorOfNeighbours.at(j)->translation[1];
            tempCenter[2] += currentPrey->vectorOfNeighbours.at(j)->translation[2];
            tempVelo[0] += currentPrey->vectorOfNeighbours.at(j)->velocity[0];
            tempVelo[1] += currentPrey->vectorOfNeighbours.at(j)->velocity[1];
            tempVelo[2] += currentPrey->vectorOfNeighbours.at(j)->velocity[2];
        }
        magnitude_tempVelo = sqrtf((float) (pow(tempVelo[0], 2) + pow(tempVelo[1], 2) + pow(tempVelo[2], 2)));
        for (j = 0; j <3 ; j++) {
            tempVelo[j] *= VELOCITY_CONSTANT/magnitude_tempVelo;
        }
        //TODO rotate the body
        GLfloat axis_q[3] = {};
        axis_q[0] = currentPrey->unitTravelDirection[1] * tempVelo[2]
                    - currentPrey->unitTravelDirection[2] * tempVelo[1];
        axis_q[1] = currentPrey->unitTravelDirection[2] * tempVelo[0]
                    - currentPrey->unitTravelDirection[0] * tempVelo[2];
        axis_q[2] = currentPrey->unitTravelDirection[0] * tempVelo[1]
                    - currentPrey->unitTravelDirection[1] * tempVelo[0];

        GLfloat magnitude_axis = Geometry::getMagnitude(axis_q);
        GLfloat halfAngle;
        if(magnitude_axis == 0) {
            //if the two vectors are parallel, no needs to detour
            if (tempVelo[0] * currentPrey->unitTravelDirection[0] >= 0 &&
                    tempVelo[1] * currentPrey->unitTravelDirection[1] >=0
                    && tempVelo[2] * currentPrey->unitTravelDirection[2] >=0) {
                continue;
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
        currentPrey->setFlattenedTransformationMatrix(RotationHelper::updateFlattenedMatrixWithQuaternion(
                currentPrey->getFlattenedTransformationMatrix(),quatForAlignment));
        //TODO deal with tempCenter later
        currentPrey->setVelocity(tempVelo);

    }

}
