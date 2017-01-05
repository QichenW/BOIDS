//
// Created by Qichen on 11/12/16.
//
#include "Fish.h"
#include "BoundaryDetector.h"

const int Fish::X_DIRECTION = 0;
const int Fish::Y_DIRECTION = 1;
const int Fish::Z_DIRECTION = 2;
const GLfloat Fish::MAX_VELOCITY= 30;
GLfloat ZERO_VECTOR[3]= {0,0,0}; //TODO why this cannot be a field
const GLfloat Fish::BODY_ANGLE_MARGIN= 35;
const GLfloat Fish::DEFAULT_DIRECTION[3]= {1,0,0};
const GLfloat Fish::BODY_UNIT_ALIGN_1[3]= {(const GLfloat) -0.7, 0, 0};
const GLfloat Fish::BODY_UNIT_ALIGN_2[3]= {(const GLfloat) -1.3, 0, 0};
const GLfloat Fish::TAIL_UNIT_ALIGN_1[3]= {(const GLfloat) -0.5, 0, 0};
const GLfloat Fish::TAIL_UNIT_ALIGN_2[3]= {(const GLfloat) -1.5, 0, 0};
int Fish::amount = 0;
GLfloat Fish::biggestRadius = 1.0;
GLfloat Fish::goal[3] = {};
GLfloat Fish::flockCentroid[3] = {};

/**
 *  Prey is a subclass of Object
 * @param oId - id
 * @param mainListId - polygon list id
 * @param om - mass
 * @param isF - is fixed (should be false)
 * @param orienttn - initial orientation
 * @param translatn - initial translation
 * @param velocity - initial velocity
 * @param angularVelocity - initial angular velocity
 * @param r - radius
 */
Fish::Fish(int oId, GLuint hLID, bool isF, GLfloat *orienttn,
           GLfloat *translatn, GLfloat *velocity, GLfloat r, GLuint bLId, GLuint tLId, int scaleM)
        : Object(oId, hLID, isF, orienttn, translatn) { // call the base class constructor first
    int i;
    radius = r;
    scaleMultiplier = scaleM;
    bodyListId = bLId;
    tailListId = tLId;
    for (i = 0; i < 3; i++) {
        *(acclrtn + i) = 0;
        *(Fish::velocity + i) = *(velocity + i);
    }
    //because of the way the code is written, must rotate first then set translation
    rotateBodyAndSetTranslation(Fish::velocity);
   //TODO subject to change
    vicinityRadius = 10 * radius;
    setIndividualCentroid(*translatn, *(translatn + 1), *(translatn + 2));
    setAlignFlatMatrices();
    bodyLocalRotation[1] = rand()%(2 * (int) BODY_ANGLE_MARGIN + 1) - BODY_ANGLE_MARGIN;
}

/***
 * update translation, rotation, velocity and angular velocity, then get the transformation matrix
 * @param t - delta t
 */
void Fish::updateFlattenedTransformationMatrix(GLfloat t) {
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
GLfloat Fish::getVelocityIn(const int direction) {
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
void Fish::setAcclrtnWithDesires(GLfloat *sDesire, GLfloat *aDesire, GLfloat *cDesire) {
    //TODO change the weights and magnitude of acceleration, now are 1 2 1
    GLfloat combinedDesire[3] ={};
    GLfloat directionToGoal[3] = {goal[0] - translation[0], goal[1] - translation[1], goal[2] - translation[2]};
    VectorCalculation::getUnitDirection(directionToGoal, directionToGoal);
    int i;
    for(i = 0; i< 3; i++){
        combinedDesire[i] += *(sDesire + i) * 50 + *(aDesire + i) * 20 + *(cDesire + i) * 20;
    }
    for(i = 0; i< 3; i++){
        combinedDesire[i] += 20 * directionToGoal[i];
    }
    for( i = 0; i < 3; i++){
        *(acclrtn + i) = *(combinedDesire + i);
    }

    // TODO test this, finally rotate velocity if there is a potential collision with "reef" cylinders
    BoundaryDetector::avoidObstacles(this);
}

//TODO change this function - get combined desires
void Fish::findDesiresThenSetAcclrtn() {
    unsigned long j;
    int noOfNeighbours = (int) vectorOfNeighbours.size();
    // alignment, cohesion, separation
    GLfloat alignmentDesire[3] = {}, cohesionDesire[3] = {}, separationDesire[3] = {};
    Fish *currentNeighbour;
    //accumulatively build up the desires
    for (j = 0; j < noOfNeighbours; j++) {
        currentNeighbour = vectorOfNeighbours.at(j);
        alignmentDesire[0] += currentNeighbour->velocity[0] - velocity[0];
        alignmentDesire[1] += currentNeighbour->velocity[1] - velocity[1];
        alignmentDesire[2] += currentNeighbour->velocity[2] - velocity[2];
        if (VectorCalculation::getDistance(apprxCentroid, currentNeighbour->apprxCentroid) <=
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
void Fish::rotateBodyAndSetTranslation(GLfloat *newVelo) {
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
    setFlattenedTransformationMatrix(RotationHelper::getFlattenedMatrixWithQuatAndTrans(
            translation, quatForAlignment));
}
/**
 * update the centroid equivalent of the group of boids
 * @param sumOfPos the sum of the positions of boids
 */
void Fish::updateFlockCentroid(GLfloat *sumOfPos) {
    int i;
    for(i = 0; i < 3; i++){
        flockCentroid[i] = *(sumOfPos + i) / amount;
    }
}

/**
 * check if current goal is met (if the centroid is close enough to the goal); if not return,
 * otherwise randomly update the goal
 */
void Fish::updateGoal() {
    if (VectorCalculation::getDistance(goal, flockCentroid) > amount / 2 * biggestRadius){
       return;
    }
    randomizeGoal();
    bool isGoalValid = false;
    int i;
    Cylinder * c;
    while(!isGoalValid){
        for (i = 0; i < Cylinder::amount; i++) {
            c = &Cylinder::cylinders[i];
            isGoalValid = true;
            if(goal[1] < c->yPosOfTopSurface + biggestRadius &&
                    sqrtf((float) (pow(goal[0] - c->axisCoordinateX, 2) + pow(goal[2] - c->axisCoordinateZ, 2)))
                    < c->radius + biggestRadius) {
               // if the goal is inside one cylinder, randomize it again and check again
                isGoalValid = false;
                randomizeGoal();
                break;
            }
        }
    }
    cout<< "Current goal is " << goal[0] <<'\t';
    cout<< goal[1] <<'\t';
    cout<< goal[2] <<endl;
}

/**
 * randomize the goal position
 */
void Fish::randomizeGoal() {
    goal[0] = rand() % 187 - 93;
    goal[1] = rand()%87 - 43;
    goal[2] = rand()%87 - 43;
}

/**
 * The approximate position of the center of the fish, (translation is the position of fish head)
 * which is determined when drawing the body of the fish
 */
void Fish::setIndividualCentroid(GLfloat x, GLfloat y, GLfloat z) {
      apprxCentroid[0] = x; apprxCentroid[1] = y; apprxCentroid[2] =z;
}

/**
 * This method is for aligning parts in the articulated fish
 */
void Fish::setAlignFlatMatrices() {
    float fb[3],sb[3],ft[3],st[3];
    int i;
    for (i = 0; i < 3; i++){
        fb[i] = BODY_UNIT_ALIGN_1[i] * scaleMultiplier;
        sb[i] = BODY_UNIT_ALIGN_2[i] * scaleMultiplier;
        ft[i] = TAIL_UNIT_ALIGN_1[i] * scaleMultiplier;
        st[i] = TAIL_UNIT_ALIGN_2[i] * scaleMultiplier;
    }
    float *temp = RotationHelper::generateFlattenedTransformationMatrix(ZERO_VECTOR, fb, false);
    for (i = 0; i< 16; i++){
        *(bodyAlignFlat1 + i) = *(temp+ i);
    }
    temp = RotationHelper::generateFlattenedTransformationMatrix(ZERO_VECTOR, sb, false);
    for (i = 0; i< 16; i++){
        *(bodyAlignFlat2 + i) = *( temp + i);
    }
    temp = RotationHelper::generateFlattenedTransformationMatrix(ZERO_VECTOR, ft, false);
    for (i = 0; i< 16; i++){
        *(tailAlignFlat1 + i) = *( temp + i);
    }
    temp = RotationHelper::generateFlattenedTransformationMatrix(ZERO_VECTOR, st, false);
    for (i = 0; i< 16; i++){
        *(tailAlignFlat2 + i) = *( temp + i);
    }
}

/**
 * the body and tail each has one degree of freedom.
 */
void Fish::rotateBodyAndTail() {
    //TODO increment the angles
    if(bodyLocalRotation[1] >= BODY_ANGLE_MARGIN || bodyLocalRotation[1] <= -1 * BODY_ANGLE_MARGIN){
        deltaAngle *= -1;
    }
    bodyLocalRotation[1] += deltaAngle;
    tailLocalRotation[1] = (GLfloat) (-1.4 * bodyLocalRotation[1]);
}
