//
// Created by Qichen on 9/22/16.
//

#ifndef BOUNCINGBALLS_MATRIXGENERATOR_H
#define BOUNCINGBALLS_MATRIXGENERATOR_H
// glut
#if defined(__APPLE__)

#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif

class RotationHelper
{
public:
    static float* generateFlattenedTransformationMatrix(float *tuple, float *trip, bool isQuaternion);

    static void rotateAvectorWithQuaternion(GLfloat dest[3], GLfloat quat[4], GLfloat src[3]);

    static GLfloat *
    updateFlattenedMatrixWithQuaternion(GLfloat *oldFlattened, GLfloat detourQuaternion[4]);

private:
    static void getHomogeneousFromEulerAngle(float *eulerAngle);
    static void initTransformationMatrixAsIdentity();

    static void initEulerAngleMatrices(float * matrix1, float * matrix2, float * matrix3);

    static void applyRotation(float rotationMatrix[4][4]);

    static void getHomogeneousFromQuaternion(float *quaternion);

    static void getTranslationMatrixFromQuat(float des[4][4], float *quaternion);

    static float *flattenTransformationMatrix(float matrix[4][4]);
};
#endif //BOUNCINGBALLS_MATRIXGENERATOR_H
