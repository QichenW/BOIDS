//
// Created by Qichen on 1/3/17.
//

#ifndef BOIDS_DRAWBRANCHES_H
#define BOIDS_DRAWBRANCHES_H
#include "setup/SimpleObjLoader.h"
#include "matrix/RotationHelper.h"
#if defined(__APPLE__)
#include <GLUT/glut.h>
#include "setup/PhysicsPrefs.h"
#include "Branch.h"
#include <vector>


#else
#include <GL/glut.h>
#endif


class DrawBranches {
public:
    static void draw(vector<Branch> v);

    static void prepare();


private:
    static const char* BRANCH_OBJ_MAME;
    static GLuint xlBranch;
    static GLuint lBranch;
    static GLuint mBranch;
    static GLuint sBranch;
};


#endif //BOIDS_DRAWBRANCHES_H
