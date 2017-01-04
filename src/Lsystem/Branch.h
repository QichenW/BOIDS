//
// Created by Qichen on 1/3/17.
//

#ifndef BOIDS_NODE_H
#define BOIDS_NODE_H
#include "matrix/RotationHelper.h"
#include <vector>
#include <math.h>
#if defined(__APPLE__)
#include <GLUT/glut.h>


#else
#include <GL/glut.h>
#endif

class Branch {
public:
    Branch(int level, GLfloat *fM);
    static vector<Branch> listOfNodes;
    int currentLevel;
    GLfloat flattenedMatrix[16] = {};
private:
    static const int LOWEST_LEVEL;
    static const GLfloat UNIT_CHILD_1_TRANS_1[3];
    static const GLfloat UNIT_CHILD_1_TRANS_2[3];
    static const GLfloat CHILD_1_ROTTN[3];
    static const GLfloat UNIT_CHILD_2_TRANS_1[3];
    static const GLfloat UNIT_CHILD_2_TRANS_2[3];
    static const GLfloat CHILD_2_ROTTN[3];
    static const GLfloat UNIT_CHILD_3_TRANS_1[3];
    static const GLfloat UNIT_CHILD_3_TRANS_2[3];
    static const GLfloat CHILD_3_ROTTN[3];
    static  GLfloat ZERO_VECTOR[3];
    GLfloat childBranchSize;
};


#endif //BOIDS_NODE_H
