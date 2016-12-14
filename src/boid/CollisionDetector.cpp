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
        for (j = 0; j < i; j++){
            //detecting if collision with a wall
            if (all[j]->isFixed){
                switch (j) {
                    case 0:
                        if(all[j]->getX() - b1->getX() <= b1->radius && b1->getVelocityIn(X_DIRECTION) > 0){
                            reverseVelocity(b1->velocity);
                            fakeTheRotation(b1, 0);
                        }
                        break;
                    case 1:
                        if(b1->getX() - all[j]->getX() <= b1->radius && b1->getVelocityIn(X_DIRECTION) < 0){
                            reverseVelocity(b1->velocity);
                            fakeTheRotation(b1, 0);
                        }
                        break;
                    case 2:
                        if(all[j]->getY() - b1->getY() <= b1->radius && b1->getVelocityIn(Y_DIRECTION) > 0){
                            reverseVelocity(b1->velocity+1);
                            fakeTheRotation(b1, 1);
                        }
                        break;
                    case 3:
                        if(b1->getY() - all[j]->getY() <= b1->radius && b1->getVelocityIn(Y_DIRECTION) < 0){
                            reverseVelocity(b1->velocity+1);
                            //TODO test this; Hitting the floor only change the rotation about x and z axes
                            fakeTheRotation(b1, 1);
                        }
                        break;
                    case 4:
                        if(all[j]->getZ() - b1->getZ() <= b1->radius && b1->getVelocityIn(Z_DIRECTION) > 0){
                            reverseVelocity(b1->velocity+2);
                            fakeTheRotation(b1, 2);
                        }
                        break;
                    case 5:
                        if(b1->getZ() - all[j]->getZ() <= b1->radius && b1->getVelocityIn(Z_DIRECTION) < 0){
                            reverseVelocity(b1->velocity+2);
                            fakeTheRotation(b1, 2);
                        }
                        break;
                    default:
                        break;
                }


            } else{ //detecting if collision with another ball
                if(i == j) {
                    continue;
                }
                b2 = ((Prey *) all[j]);
                if(Geometry::getDistance(b1->getTranslation(), b2->getTranslation())
                        <= b1->radius +  b2->radius) {
                    applyCollisionAvoidance(b1, b2);
                    //Fake the rotation for either or both of them, if on the floor
                    if(b1->getY() - Prey::BOTTOM_WALL_Y <= b1->radius){
                        // Hitting the floor only change the rotation about x and z axes
                        fakeTheRotation(b1, 1);
                    }
                    if(b2->getY() - Prey::BOTTOM_WALL_Y <= b2->radius){
                        //Hitting the floor only change the rotation about x and z axes
                        fakeTheRotation(b2, 1);
                    }
                }
            }
        }
    }
}

/***
 * fake the rotation when the ball hit 1 of the 6 walls
 * based on the relative velocity w.r.t. one of the three axes
 * v = rω (beware the positive direction of rotation about each axes)
 * @param ball
 * @param hitInTheDirectionOfWhichAxe 0 - x/1 - y/ 2 - z
 */
void CollisionDetector::fakeTheRotation(Prey *ball, int hitInTheDirectionOfWhichAxe) {
    switch(hitInTheDirectionOfWhichAxe) {
        case 0:
            ball->angluarVelo[1] = -57 * ball->velocity[2] / ball->radius;
            ball->angluarVelo[2] = -57 * ball->velocity[0] / ball->radius;
            break;
        case 1:
            ball->angluarVelo[0] = 57 * ball->velocity[2] / ball->radius;
            ball->angluarVelo[2] = -57 * ball->velocity[0] / ball->radius;
            break;
        case 2:
            ball->angluarVelo[0] = 57 * ball->velocity[2] / ball->radius;
            ball->angluarVelo[1] = 57 * ball->velocity[0] / ball->radius;
            break;
        default:
            break;
    }
}

/**
 * reverse the velocity when there is a collsion.
 * r is set arbitarily to let bouncing stop at some point in time
 * Reverse the ball's velocity in one direction due to collision with walls
 * @param i identifies which direction
 * @param ratio the amount of velocity presists
 */
void CollisionDetector::reverseVelocity(GLfloat *veloInOneDirection){
    GLfloat r;
    if(fabs(*veloInOneDirection) > 30){
        r = 0.8;
    }else if(fabs(*veloInOneDirection) > 14){
        r = 0.7;
    } else if(fabs(*veloInOneDirection) > 4){
        r = 0.5;
    } else {
        r = 0;
    }
    *veloInOneDirection *= -1*r;
}

/**
 * when two boids are too close, apply collision avoidance behaviours
 * @param p1
 * @param p2
 */
void CollisionDetector::applyCollisionAvoidance(Prey *p1, Prey *p2) {
//TODO implement this
}
