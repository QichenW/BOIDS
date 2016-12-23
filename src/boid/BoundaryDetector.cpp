//
// Created by Qichen on 11/12/16.
//


#include "BoundaryDetector.h"


const int BoundaryDetector::NUMBER_OF_WALLS = 6;
const int BoundaryDetector::X_DIRECTION = 0;
const int BoundaryDetector::Y_DIRECTION = 1;
const int BoundaryDetector::Z_DIRECTION = 2;
/***
 *
 * @param all - all[0,1,2,3,4,5] are walls (right, left, top, bottom, near, rear)
 * @param numberOfObjects
 */
void BoundaryDetector::detectAll(Object **all, int numberOfObjects) {
    int i,j;
    Prey * b1, * b2;
    for(i = NUMBER_OF_WALLS; i < numberOfObjects; i++){
        b1 = (Prey *) all[i];
        // clear the vector of neighbours and add in later
        b1->vectorOfNeighbours.clear();
        for (j = 0; j < i; j++){
            //detecting if collision with a wall
            if (all[j]->isFixed){
                //TODO change these boundary behaviours
                switch (j) {
                    case 0:
                        if(all[j]->getX() - b1->getX() <= b1->radius && b1->getVelocityIn(X_DIRECTION) > 0){
                            halt(b1->velocity, b1->acclrtn);
                        }
                        break;
                    case 1:
                        if(b1->getX() - all[j]->getX() <= b1->radius && b1->getVelocityIn(X_DIRECTION) < 0){
                            halt(b1->velocity, b1->acclrtn);
                        }
                        break;
                    case 2:
                        if(all[j]->getY() - b1->getY() <= b1->radius && b1->getVelocityIn(Y_DIRECTION) > 0){
                            halt(b1->velocity + 1, b1->acclrtn + 1);
                        }
                        break;
                    case 3:
                        if(b1->getY() - all[j]->getY() <= b1->radius && b1->getVelocityIn(Y_DIRECTION) < 0){
                            halt(b1->velocity + 1, b1->acclrtn + 1);
                        }
                        break;
                    case 4:
                        if(all[j]->getZ() - b1->getZ() <= b1->radius && b1->getVelocityIn(Z_DIRECTION) > 0){
                            halt(b1->velocity + 2, b1->acclrtn + 2);
                        }
                        break;
                    case 5:
                        if(b1->getZ() - all[j]->getZ() <= b1->radius && b1->getVelocityIn(Z_DIRECTION) < 0){
                            halt(b1->velocity + 2, b1->acclrtn + 2);
                        }
                        break;
                    default:
                        break;
                }
            } else{ //detecting if collision with another
                b2 = ((Prey *) all[j]);
                // add a neighbour if close enough
                if(VectorCalculation::getDistance(b1->getTranslation(), b2->getTranslation()) <= b1->vicinityRadius){
                    b1->vectorOfNeighbours.push_back(b2);
                }
            }
        }
    }
}

/**
 * halt the object if reach the boundary
 * @param i identifies which direction
 * @param ratio the amount of velocity presists
 */
void BoundaryDetector::halt(GLfloat *veloOneDir, GLfloat * acclrtnOneDir){
   *veloOneDir = 0;
    *acclrtnOneDir = 0;
}
