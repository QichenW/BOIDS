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
     GLfloat *translatn, GLfloat * velocity, GLfloat * angularVelocity, GLfloat r)
        :Object(oId, lId, isF, orienttn, translatn){ // call the base class constructor first
    int i;
    radius = r;
    for (i = 0 ; i < 3; i++){
        *(Prey::velocity + i) = *(velocity + i);
        *(directionOfCollision + i) = 0;
    }
    isPreyDead = false;
    isPredator = false;
    detour = false;
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
    }
    //TODO test this
    flattenedTransformationMatrix[12] = translation[0];
    flattenedTransformationMatrix[13] = translation[1];
    flattenedTransformationMatrix[14] = translation[2];
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
 * boid behavior
 */
void Prey::boidSeperation(){
    //TODO
}

/**
 * boid behavior
 */
void Prey::boidAlignment(){
    //TODO
}

/**
 * boid behavior
 */
void Prey::boidCohesion(){
    //TODO
}

void Prey::setVelocity(GLfloat *v) {
    //TODO test this
    int i;
    for(i = 0; i < 3; i++){
        velocity[i] = *(v + i);
    }
}

void Prey::hasDetour(bool i) {
    detour = i;

}
