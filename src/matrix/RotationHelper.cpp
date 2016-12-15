//
// Created by Qichen on 9/22/16.
//

#include "RotationHelper.h"
/* sine cosine */
#include <math.h>
#include <algorithm>

#include <iostream>
using namespace std;

static float transformationMatrix[4][4] ={};
static float flattenedTransformationMatrix[16] ={};

/**
 * prepare the flattened matrix to be feed into glMultMatrixf(), which is multipy to all points in the scene
 * @parm is either a quaternion or a vector suggesting Euler Angle
 * **/
float* RotationHelper::generateFlattenedTransformationMatrix(float *tuple, float *trip, bool isQuaternion){
    // initialize the transformation matrix as identity matrix then multiply it with three rotation
    // matrices for euler angle approach; 1 matrix for quaternion approach
    initTransformationMatrixAsIdentity();

    if(isQuaternion){
        getHomogeneousFromQuaternion(tuple);
    } else {
        getHomogeneousFromEulerAngle(tuple);
    }
    if(trip != nullptr) {
        // add the translation to transform matrix
        transformationMatrix[0][3] += *trip; // translation x
        transformationMatrix[1][3] += *(trip + 1); // translation y
        transformationMatrix[2][3] += *(trip + 2); // translation z
    }
    /**
     * openGL has column major matrix (so my matrix should be transposed), the flattened matrix looks like m[] below
     * **/
//    static float m[] ={ 1.0f, 0.0f, 0.0f, 0.0f,
//                 0.0f, 1.0f, 0.0f, 0.0f,
//                 0.0f, 0.0f, 1.0f, 0.0f,
//                 0.0f, 0.0f, -10.0f, 1.0f };
    return flattenTransformationMatrix(transformationMatrix);
}

/**
 * given a 4*4 matrix and flatten it
 * @param matrix input
 * @return a 1*16 flattened matrix
 */
float *RotationHelper::flattenTransformationMatrix(GLfloat matrix[4][4]) {
    int i,j;
    for(i = 0; i < 4; i++){
        for (j=0; j< 4; j++){
            //opengl needs column major matrix
            flattenedTransformationMatrix[i * 4 + j] = matrix[j][i];
        }
    }
    return  flattenedTransformationMatrix;
}

/****
 * Hard coded
 * get the homogenuous matrix for quaternion approach, store it in the global variable transformationMatrix
 * @param quaternion must be NORMALIZED before
 */
void RotationHelper::getHomogeneousFromQuaternion(float *quaternion) {
    GLfloat quaternionRotationMatrix[4][4] = {};
    getTranslationMatrixFromQuat(quaternionRotationMatrix,quaternion);

    // multiply the transformation matrix with the rotation matrix
    applyRotation(quaternionRotationMatrix);
}

/**
 * given the quaternion, get the 4*4 matrix about rotation
 * @param quaternionRotationMatrix where the result is stored
 * @param quaternion  the input quaternion
 */
void RotationHelper::getTranslationMatrixFromQuat(float quaternionRotationMatrix[4][4], float *quaternion) {
    GLfloat twoXsqure = (GLfloat) pow(*(quaternion + 1), 2) * 2;
    GLfloat twoYsqure = (GLfloat) pow(*(quaternion + 2), 2) * 2;
    GLfloat twoZsqure = (GLfloat) pow(*(quaternion + 3), 2) * 2;
    GLfloat twoxy = *(quaternion + 1) * (*(quaternion + 2)) * 2;
    GLfloat twoxz = *(quaternion + 1) * (*(quaternion + 3)) * 2;
    GLfloat twoyz = *(quaternion + 2) * (*(quaternion + 3)) * 2;
    GLfloat twowx = *quaternion * (*(quaternion + 1)) * 2;
    GLfloat twowy = *quaternion * (*(quaternion + 2)) * 2;
    GLfloat twowz = *quaternion * (*(quaternion + 3)) * 2;

    quaternionRotationMatrix[0][0] = 1 - twoYsqure - twoZsqure;
    quaternionRotationMatrix[0][1] = twoxy - twowz;
    quaternionRotationMatrix[0][2] = twoxz + twowy;
    quaternionRotationMatrix[1][0] = twoxy + twowz;
    quaternionRotationMatrix[1][1] = 1 - twoXsqure - twoZsqure;
    quaternionRotationMatrix[1][2] = twoyz - twowx;
    quaternionRotationMatrix[2][0] = twoxz - twowy;
    quaternionRotationMatrix[2][1] = twoyz + twowx;
    quaternionRotationMatrix[2][2] = 1 - twoXsqure - twoYsqure;
    quaternionRotationMatrix[3][3] = 1;
}

/****
 * This function generates a homogeneous matrix given the euler angle
 * @param eulerAngle points to an array holding pith, yaw, roll
 */
void RotationHelper::getHomogeneousFromEulerAngle(float *eulerAngle) {
    /**
     * pitch, yaw, roll
     * */
    float pitch = (float) M_PI / 180 * (*eulerAngle);
    float yaw = (float) M_PI / 180 * ( *(eulerAngle +1));
    float roll = (float) M_PI / 180 * ( *(eulerAngle +2));

    float homogeneousPitch[4][4] = {}, homogeneousYaw[4][4] ={},homogeneousRoll[4][4] = {};

    initEulerAngleMatrices((float *)homogeneousPitch, (float *)homogeneousYaw, (float *)homogeneousRoll);


    // rotation pitch
    homogeneousPitch[1][1] = cosf(pitch);
    homogeneousPitch[2][2] = cosf(pitch);
    homogeneousPitch[1][2] = -1 * sinf(pitch);
    homogeneousPitch[2][1] = sinf(pitch);
    // rotation yaw
    homogeneousYaw[0][0] = cosf(yaw);
    homogeneousYaw[2][2] = cosf(yaw);
    homogeneousYaw[0][2] = sinf(yaw);
    homogeneousYaw[2][0] = -1 * sinf(yaw);
    // rotation roll
    homogeneousRoll[0][0] = cosf(roll);
    homogeneousRoll[1][1] = cosf(roll);
    homogeneousRoll[0][1] = -1 * sinf(roll);
    homogeneousRoll[1][0] = sinf(roll);

    // multiply the transformation matrix with three rotation matrices
    applyRotation(homogeneousPitch);
    applyRotation(homogeneousYaw);
    applyRotation(homogeneousRoll);
}

/****
 * matrices multiplication, transformationMatrix is initially an unit matrix,
 * call this function to multiply one rotation matrix to it.
 * @param rotationMatrix is to be multiplied to transformationMatrix
 */
void RotationHelper::applyRotation(float rotationMatrix[4][4]) {
    // temporarily hold the result
    float tempMatrix[4][4] ={};
    int i,j,m;
    for (i = 0; i < 4; i++){
        for(j =0; j< 4; j++){
            for (m = 0; m < 4; m++){
                tempMatrix[i][j] += *(rotationMatrix[i]+m) * transformationMatrix[m][j];
                //tempMatrix[i][j] = rotationMatrix[i][m] * transformationMatrix[m][j];
            }
        }
    }

    std::copy(&tempMatrix[0][0], &tempMatrix[0][0] + 16, &transformationMatrix[0][0]);
}

/**
 * Initialize the Euler Angle pitch, yaw, roll matrices as identity matrices
 * **/
void RotationHelper::initEulerAngleMatrices(float * matrix1, float * matrix2, float * matrix3) {

    std::copy(&transformationMatrix[0][0], &transformationMatrix[0][0] + 16, matrix1);
    std::copy(&transformationMatrix[0][0], &transformationMatrix[0][0] + 16, matrix2);
    std::copy(&transformationMatrix[0][0], &transformationMatrix[0][0] + 16, matrix3);
}

/**
 * Initialize the transformation matrix as an identity matrix
 * **/
void RotationHelper::initTransformationMatrixAsIdentity() {
    int i,j;
    for(i = 0; i < 4; i++) {
        for (j =0; j< 4; j++){
            if ( i == j) {
                transformationMatrix[i][j] = 1;
            } else {
                transformationMatrix[i][j] = 0;
            }
        }
    }
}

/**
 * given the quaternion and original vector get the rorated vector
 * @param quat the quaternion
 * @param src the original vector
 */
void RotationHelper::rotateAvectorWithQuaternion(GLfloat dest[3], GLfloat *quat, GLfloat *src) {
//TODO test this
    GLfloat rotationMatrix[4][4] = {};
    getTranslationMatrixFromQuat(rotationMatrix, quat);
    int i, j;
    for(i = 0 ; i < 3; i++){
        dest[i] = 0;
        for(j = 0; j < 3; j++){
            dest[i] += rotationMatrix[i][j] * src[j];
        }
    }
}

//TODO test this
GLfloat *
RotationHelper::updateFlattenedMatrixWithQuaternion(GLfloat *oldFlattened, GLfloat *detourQuaternion) {
    int i,j, k;
    GLfloat tempMatrix[4][4] = {}; // make old flattened matrix a 4*4 matrix
    GLfloat tempNewMatrix[4][4] = {}; // a 4*4 matrix for new rotation
    GLfloat detourQuatMatrix[4][4] = {}; // the detour quaternion's matrix
    getTranslationMatrixFromQuat(detourQuatMatrix, detourQuaternion);
    for (i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            tempMatrix[j][i] = oldFlattened[i * 4 + j];
        }
    }
     for (i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 4; k++){
                tempNewMatrix[i][j] += detourQuatMatrix[i][k] * tempMatrix[k][j];
            }
        }
    }
    return flattenTransformationMatrix(tempNewMatrix);
}
