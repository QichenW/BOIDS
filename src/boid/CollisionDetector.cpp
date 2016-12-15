//
// Created by Qichen on 11/12/16.
//


#include "CollisionDetector.h"


const int CollisionDetector::NUMBER_OF_WALLS = 6;
const int CollisionDetector::X_DIRECTION = 0;
const int CollisionDetector::Y_DIRECTION = 1;
const int CollisionDetector::Z_DIRECTION = 2;
/***
 *
 * @param all - all[0,1,2,3,4,5] are walls (right, left, top, bottom, near, rear)
 * @param numberOfObjects
 */
void CollisionDetector::detectAll(Object **all, int numberOfObjects) {
    int i,j;
    Prey * b1, * b2;
    for(i = NUMBER_OF_WALLS; i < numberOfObjects; i++){
        b1 = (Prey *) all[i];
        b1->hasDetour(false);
        for (j = 0; j < i; j++){
            //detecting if collision with a wall
            if (all[j]->isFixed){
                //TODO change these boundary behaviours
                switch (j) {
                    case 0:
                        if(all[j]->getX() - b1->getX() <= b1->radius && b1->getVelocityIn(X_DIRECTION) > 0){
                            reverseVelocity(b1->velocity);
                        }
                        break;
                    case 1:
                        if(b1->getX() - all[j]->getX() <= b1->radius && b1->getVelocityIn(X_DIRECTION) < 0){
                            reverseVelocity(b1->velocity);
                        }
                        break;
                    case 2:
                        if(all[j]->getY() - b1->getY() <= b1->radius && b1->getVelocityIn(Y_DIRECTION) > 0){
                            reverseVelocity(b1->velocity+1);
                        }
                        break;
                    case 3:
                        if(b1->getY() - all[j]->getY() <= b1->radius && b1->getVelocityIn(Y_DIRECTION) < 0){
                            reverseVelocity(b1->velocity+1);
                        }
                        break;
                    case 4:
                        if(all[j]->getZ() - b1->getZ() <= b1->radius && b1->getVelocityIn(Z_DIRECTION) > 0){
                            reverseVelocity(b1->velocity+2);
                        }
                        break;
                    case 5:
                        if(b1->getZ() - all[j]->getZ() <= b1->radius && b1->getVelocityIn(Z_DIRECTION) < 0){
                            reverseVelocity(b1->velocity+2);
                        }
                        break;
                    default:
                        break;
                }
            } else{ //detecting if collision with another
                b2 = ((Prey *) all[j]);
                if(b2->detour) {
                    continue;
                }
                if(Geometry::getDistance(b1->getTranslation(), b2->getTranslation())
                        <= b1->radius +  b2->radius) {
                    if(!b1->detour) {
                        applyCollisionAvoidance(b1, b2);
                    }
                    if(!b2->detour) {
                        applyCollisionAvoidance(b2, b1);
                    }
               }
            }
        }
    }
}

/**
 * reverse the velocity when there is a collsion.
 * Reverse the ball's velocity in one direction due to collision with walls
 * @param i identifies which direction
 * @param ratio the amount of velocity presists
 */
void CollisionDetector::reverseVelocity(GLfloat *veloInOneDirection){
   *veloInOneDirection *= -1;
}

/**
 * when two boids are too close, apply collision avoidance behaviours
 * @param p1
 * @param p2
 */
void CollisionDetector::applyCollisionAvoidance(Prey *p1, Prey *p2) {
    GLfloat relativeVelocity[3], c2c[3];
    GLfloat minDistance =p1->radius + p2->radius;
    GLfloat dy = p2->getY() - p1->getY();
    GLfloat dx = p2->getX() - p1->getX();
    GLfloat dz = p2->getZ() - p1->getZ();
    if(fabsf(dx) > minDistance ||
       fabsf(dy) > minDistance ||
       fabsf(dz) > minDistance){
       return;
    } else if (pow(dx, 2) + pow(dy, 2)
               + pow(dz, 2) <= pow(minDistance, 2)) {
        c2c[0]= dx; c2c[1]= dy; c2c[2] = dz;
        relativeVelocity[0] = p1->velocity[0] - p2->velocity[0];
        relativeVelocity[1] = p1->velocity[1] - p2->velocity[1];
        relativeVelocity[2] = p1->velocity[2] - p2->velocity[2];
        if(c2c[0] * relativeVelocity[0] + c2c[1] * relativeVelocity[1] + c2c[2] + relativeVelocity[2] <= 0){
            return;
        }
    }
    //now that close enough and not getting apart, collision avoidance needs to be applied
    GLfloat axis_1[3] = {c2c[1]*relativeVelocity[2] - c2c[2] * relativeVelocity[1],
                        c2c[2] * relativeVelocity[0] - c2c[0] * relativeVelocity[2],
                        c2c[0] * relativeVelocity[1] - c2c[1] * relativeVelocity[0]};
    int i;
    GLfloat magnitude_1 = sqrtf((float) (pow(axis_1[0], 2) + pow(axis_1[1], 2) + pow(axis_1[2], 2)));

    if(magnitude_1 == 0){ // if the two vectors are parallel arbitrarily set the axis to y axis
        axis_1[0] = 0; axis_1[1] = 1; axis_1[2] = 0;
    } else {
        // normalize the axis vector
        for (i = 0; i < 3; i++) {
            axis_1[i] /= magnitude_1;
        }
    }
    GLfloat sinValue = sinf((float) (M_PI / 4));
    GLfloat quaternionForTangent[4] = {cosf((float) (M_PI / 4)),  sinValue * axis_1[0],
                                       sinValue * axis_1[1], sinValue * axis_1[2]};
    GLfloat tangent[3] = {};
    //TODO this function is tested
       RotationHelper::rotateAvectorWithQuaternion(tangent, quaternionForTangent, c2c);
    GLfloat magnitude_tangent = sqrtf((float) (pow(tangent[0], 2) + pow(tangent[1], 2)
                                               + pow(tangent[2], 2)));

    GLfloat axis_2[3] = {p1->unitTravelDirection[1]*tangent[2] - p1->unitTravelDirection[2] * tangent[1],
                         p1->unitTravelDirection[2] * tangent[0] - p1->unitTravelDirection[0] * tangent[2],
                         p1->unitTravelDirection[0] * tangent[1] - p1->unitTravelDirection[1] * tangent[0]};

    GLfloat magnitude_2 = sqrtf((float) (pow(axis_2[0], 2) + pow(axis_2[1], 2)
                                               + pow(axis_2[2], 2)));
    // normalize the axis vector
    for (i = 0; i < 3; i++){
        tangent[i] *= Prey::VELOCITY_CONSTANT / magnitude_tangent;
        axis_2[i] /= magnitude_2;
    }
    GLfloat sinValue2 = magnitude_2/magnitude_tangent;
    GLfloat halfAngle = asinf(sinValue2)/2;
    GLfloat quaternionForDetour[4] ={cosf(halfAngle), sinf(halfAngle) * axis_2[0],sinf(halfAngle) * axis_2[1],
                                     sinf(halfAngle) * axis_2[2]};
    //TODO this function is tested
    p1->setFlattenedTransformationMatrix(RotationHelper::updateFlattenedMatrixWithQuaternion(
                                                        p1->getFlattenedTransformationMatrix(),quaternionForDetour));
    //TODO this
    p1->hasDetour(true);
    p1->setVelocity(tangent);
}
