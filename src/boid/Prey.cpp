//
// Created by Qichen on 11/12/16.
//
#include "Prey.h"

const int Prey::X_DIRECTION = 0;
const int Prey::Y_DIRECTION = 1;
const int Prey::Z_DIRECTION = 2;
const GLfloat Prey::MAX_VELOCITY= 30;
const GLfloat Prey::DEFAULT_DIRECTION[3]= {1,0,0};
int Prey::amount = 0;
GLfloat Prey::biggestRadius = 1.0;
GLfloat Prey::goal[3] = {};
GLfloat Prey::centroid[3] = {};

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
    for (i = 0; i < 3; i++) {
        *(acclrtn + i) = 0;
        *(Prey::velocity + i) = *(velocity + i);
    }
    //because of the way the code is written, must rotate first then set translation
    rotateBodyAndSetTranslation(Prey::velocity);
   //TODO subject to change
    vicinityRadius = 10 * radius;
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
    GLfloat veloMagnitude = VectorCalculation::getMagnitude(velocity);
    if ( veloMagnitude > MAX_VELOCITY){
        for (i = 0; i < 3; i++){
            *(velocity + i) *= MAX_VELOCITY / veloMagnitude;
        }
    }
    // rotate body by referring to old travel direction and new velocity
    rotateBodyAndSetTranslation(velocity);
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

/**
 * get the combined desire as acceleration
 */
void Prey::setAcclrtnWithDesires(GLfloat *sDesire, GLfloat *aDesire, GLfloat *cDesire) {
    //TODO change the weights and magnitude of acceleration, now are 1 2 1
    GLfloat combinedDesire[3] ={};
    GLfloat directionToGoal[3] = {goal[0] - translation[0], goal[1] - translation[1], goal[2] - translation[2]};
    VectorCalculation::getUnitDirection(directionToGoal, directionToGoal);
    int i;
    for(i = 0; i< 3; i++){
        combinedDesire[i] += *(sDesire + i) * 50 + *(aDesire + i) * 10 + *(cDesire + i) * 10;
    }
    for(i = 0; i< 3; i++){
        combinedDesire[i] += 20 * directionToGoal[i];
    }
    for( i = 0; i < 3; i++){
        *(acclrtn + i) = *(combinedDesire + i);
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
void Prey::rotateBodyAndSetTranslation(GLfloat *newVelo) {
    int j;
    GLfloat axis_q[3] = {};
    VectorCalculation::getCrossProduct(axis_q, (GLfloat *) DEFAULT_DIRECTION, newVelo);
    GLfloat magnitudeNewVelo = VectorCalculation::getMagnitude(newVelo);
    GLfloat magnitude_axis = VectorCalculation::getMagnitude(axis_q);
    GLfloat halfAngle;
    if (magnitude_axis == 0) {
        //if the two vectors are in same direction, no needs to detour
        if (VectorCalculation::areTwoVectorSameDirection(newVelo,  (GLfloat *) DEFAULT_DIRECTION)) {
            return;
        }
        // if the two vectors are opposite
        axis_q[0] = 0;
        axis_q[2] = 0;
        axis_q[1] = 1;
        halfAngle = (GLfloat) (M_PI / 2);
    } else {
        // normalize the axis vector
        VectorCalculation::getUnitDirection(axis_q,axis_q);
        GLfloat sinValue2 = magnitude_axis / magnitudeNewVelo;
        //TODO write about this - use dot product to determine the angle
        if(VectorCalculation::dotProduct((GLfloat *) DEFAULT_DIRECTION, newVelo) >= 0){
            halfAngle = asinf(sinValue2) / 2;
        } else {
            if(sinValue2 > 0) {
             halfAngle = (GLfloat) ((M_PI - asinf(sinValue2)) / 2);
            } else{
                halfAngle = (GLfloat) ((asinf(sinValue2) - M_PI) / 2);
            }
        }

    }
    GLfloat quatForAlignment[4] = {cosf(halfAngle), sinf(halfAngle) * axis_q[0], sinf(halfAngle) * axis_q[1],
                                   sinf(halfAngle) * axis_q[2]};
    //TODO this function is tested
    setFlattenedTransformationMatrix(RotationHelper::getFlattenedMatrixWithQuatAndTrans(
            translation, quatForAlignment));
}
/**
 * update the centroid equivalent of the group of boids
 * @param sumOfPos the sum of the positions of boids
 */
void Prey::updateCentroid(GLfloat *sumOfPos) {
    int i;
    for(i = 0; i < 3; i++){
        centroid[i] = *(sumOfPos + i) / amount;
    }
}

/**
 * check if current goal is met (if the centroid is close enough to the goal); if not return,
 * otherwise randomly update the goal
 */
void Prey::updateGoal() {
    if (VectorCalculation::getDistance(goal, centroid) > amount / 2 * biggestRadius){
       return;
    }
    goal[0] = rand() % 187 - 93;
    cout<< "Current goal is " << goal[0] <<'\t';
    goal[1] = rand()%87 - 43;
    cout<< goal[1] <<'\t';
    goal[2] = rand()%87 - 43;
    cout<< goal[2] <<endl;
}
