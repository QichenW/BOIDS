//
// Created by Qichen on 11/12/16. Last edited on 1/4/2017
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
void BoundaryDetector::detectFellowsAndWalls(Object **all, int numberOfObjects) {
    int i,j;
    Fish * b1, * b2;
    for(i = NUMBER_OF_WALLS; i < numberOfObjects; i++){
        b1 = (Fish *) all[i];
        // clear the vector of neighbours and add in later
        b1->vectorOfNeighbours.clear();
        for (j = 0; j < numberOfObjects; j++){
            if(i ==j) {
                continue;
            }
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
                b2 = ((Fish *) all[j]);
                // add a neighbour if close enough
                if(VectorCalculation::getDistance(b1->apprxCentroid,b2->apprxCentroid) <= b1->vicinityRadius){
                    b1->vectorOfNeighbours.push_back(b2);
                }
            }
        }
    }
}

/**
 * halt the object if reach the boundary
 */
void BoundaryDetector::halt(GLfloat *veloOneDir, GLfloat * acclrtnOneDir){
   *veloOneDir = 0;
    *acclrtnOneDir = 0;
}

/**
 * This is the final adjustment to velocity and acceleration of boids, before updating transformation matrix.
 * If a boid is getting into the cylinder from the top, eliminate its the velo and accleartion in y direction;
 * If a boid is getting into the cylinder from side, rotate its velo projection in x-z planar (in the direction of
 * shorter detour distance expected).
 * @param all - array of pointers to instances of Object
 * @param listOfCylinders - vector of instances of Cylinder
 */
void BoundaryDetector::avoidObstacles(Fish *pFish) {
    int i, j;
    Cylinder * pCylinder;
    GLfloat disToAxis;
    GLfloat vectorToAxis[3] = {};
    GLfloat recognitionBound;
    GLfloat xzPlanarVelo[3] = {};
    //TODO the additional recognition bound may be tweaked
    recognitionBound = pFish->radius * 2;
    for (j = 0; j < Cylinder::amount; j++) {
        pCylinder = &Cylinder::cylinders[j];
        //get the vector pointing to the axis of the cylinder from the centroid of boid
        vectorToAxis[0] = pCylinder->axisCoordinateX - pFish->apprxCentroid[0];
        vectorToAxis[2] = pCylinder->axisCoordinateZ - pFish->apprxCentroid[2];
        //get the distance from the boid to the vertical axis of the cylinder
        disToAxis = VectorCalculation::getMagnitude(vectorToAxis);
//        //if the boid is in the cylinder, do nothing
//        if (disToAxis < pFish->radius + pCylinder->radius &&
//            pFish->apprxCentroid[1] < pCylinder->yPosOfTopSurface) {
//            continue;
//        }
        //if in the recognition volume but not in the cylinder
        if (disToAxis <= pCylinder->radius + recognitionBound &&
            pFish->apprxCentroid[1] <= pCylinder->yPosOfTopSurface + recognitionBound) {
            // TODO rotate velo in x-z plane
            xzPlanarVelo[0] = pFish->velocity[0];
            xzPlanarVelo[2] = pFish->velocity[2];
            GLfloat angleMargin; // the margin of angle between vectorToAxis and xzPlanarVelo in degrees
            angleMargin = (GLfloat) ( acosf(VectorCalculation::
                                                          dotProduct(xzPlanarVelo, vectorToAxis) / VectorCalculation::
                                                          getMagnitude(xzPlanarVelo) / disToAxis));
            //if getting into the cylinder from side, i.e. use dot product to identify
            if (pFish->getTranslation()[1] < pCylinder->yPosOfTopSurface &&
                angleMargin < M_PI/2) {
                GLfloat halfAngle = (GLfloat) ((M_PI / 2 - angleMargin) / 2);
                //the result of cross product is the axis of rotation, it should be {0,x,0}
                GLfloat rotationAxis[3] = {};
                VectorCalculation::getCrossProduct(rotationAxis, vectorToAxis, xzPlanarVelo);
                rotationAxis[1] = (rotationAxis[1] > 0) ? 1 : -1; // normalize it
                // get the quaternion representing this detour rotation
                GLfloat tempQuat[4] = {cosf(halfAngle), 0, sinf(halfAngle) * rotationAxis[1], 0};
                // update the fish's velocity by the result of rotating the xzPlanarVelo
                GLfloat desiredVelo[3] = {};
                GLfloat veloY= pFish->velocity[1];
                RotationHelper::rotateAvectorWithQuaternion(pFish->velocity, tempQuat, xzPlanarVelo);
                pFish->velocity[1] = veloY;
                //TODO test above code
            } else if (pFish->getTranslation()[1] > pCylinder->yPosOfTopSurface &&
                       pFish->velocity[1] < 0) {
                //if getting into the cylinder from top
                pFish->velocity[1] = 0; // eliminate the velocity in y direction
                if(pFish->acclrtn[1] < 0) {
                    // eliminate the acceleration in y direction if going downwards
                    pFish->acclrtn[1] = 0;
                }
            }
        }
    }
}
