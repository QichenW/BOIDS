//
// Created by Qichen on 12/13/16.
//

#include "Predator.h"
const int Predator::X_DIRECTION = 0;
const int Predator::Y_DIRECTION = 1;
const int Predator::Z_DIRECTION = 2;
const GLfloat Predator:: VELOCITY_CONSTANT= 15;

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
Predator::Predator(int oId, int lId, bool isF, GLfloat *orienttn,
           GLfloat *translatn, GLfloat * velocity, GLfloat * angularVelocity, GLfloat r)
        :Object(oId, lId, isF, orienttn, translatn){ // call the base class constructor first
    int i;
    radius = r;
    for (i = 0 ; i < 3; i++){
        *(acceleration + i) = 0;
        *(Predator::velocity + i) = *(velocity + i);
        *(Predator::angluarVelo + i) = *(angularVelocity + i);
        *(angularAcclrtn + i) = 0;
        *(directionOfCollision + i) = 0;
    }
    setUnitTravelDirection();
    isPredator = true;
}

/***
 * update translation, rotation, velocity and angular velocity, then get the transformation matrix
 * @param t - delta t
 */
void Predator::updateFlattenedTransformationMatrix(GLfloat t) {
    setUnitTravelDirection();
    updateAcclrtn();
    int i;
    for (i = 0 ; i < 3; i++){
        *(translation + i) += *(velocity + i) * t;
        *(velocity + i) += *(acceleration + i) * t;

        *(orientation + i) += *(angluarVelo + i) * t;
        *(angluarVelo + i) += *(angularAcclrtn + i) * t;
    }

    setFlattenedTransformationMatrix(RotationHelper::
                                     generateFlattenedTransformationMatrix(orientation,translation,false));
}

/**
 * @param direction indicate x or y or z direction
 * @return  the velocity in one direction
 */
GLfloat Predator::getVelocityIn(const int direction) {
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
 * set unit travel direction, which is used to determine the acceleration caused by friction.
 */
void Predator::setUnitTravelDirection() {
    Geometry::getUnitDirection(unitTravelDirection, velocity);
}

/**
 * update the acceleration for every time instance, if ball is in the air, it has g, if not in the air, it has
 * acceleration caused by friction
 */
void Predator::updateAcclrtn() {
    //TODO how to update acceleration
}

/**
 * This function defines how predator chase preies
 */
void Predator::hunt() {
//TODO
}

/**
 * This function eliminates caught preies.
 */
void Predator::kill(Prey prey) {
//TODO
    prey.fall();
}
