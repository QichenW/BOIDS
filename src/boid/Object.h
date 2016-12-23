//
// Created by Qichen on 11/12/16.
//

#ifndef BOIDS_OBJECT_H
#define BOIDS_OBJECT_H
#include "matrix/RotationHelper.h"
#if defined(__APPLE__)
#include <GLUT/glut.h>


#else
#include <GL/glut.h>
#endif

class Object {
public:
    Object(int id, GLuint listId, bool isFixed, GLfloat *orientation, GLfloat *translation);

    void setFlattenedTransformationMatrix(GLfloat *flattenedTransformationMatrix);

    GLfloat *getFlattenedTransformationMatrix();

    GLfloat *getTranslation();

    GLuint getListId();

    GLfloat getX();

    GLfloat getY();

    GLfloat getZ();


    bool isFixed;
    bool isPredator;
    const static GLfloat BOTTOM_WALL_Y;
    const static GLfloat TOP_WALL_Y;
    const static GLfloat REAR_WALL_Z;
    const static GLfloat NEAR_WALL_Z;
    const static GLfloat LEFT_WALL_X;
    const static GLfloat RIGHT_WALL_X;
protected:
    int id;
    GLuint listId;
    GLfloat orientation[3]={};  //3
    GLfloat translation[3]={};  //3
    GLfloat flattenedTransformationMatrix[16]={}; //16

};


#endif //BOIDS_OBJECT_H
