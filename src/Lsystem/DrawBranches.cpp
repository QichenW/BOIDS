//
// Created by Qichen on 1/3/17.
//

#include "DrawBranches.h"

const char * DrawBranches::BRANCH_OBJ_MAME = "branch.obj";
GLuint DrawBranches::xlBranch;
GLuint DrawBranches::lBranch;
GLuint DrawBranches::mBranch;
GLuint DrawBranches::sBranch;

/**
 * load the obj file and manipulate its size for different level of branches
 */
void DrawBranches::prepare() {
    xlBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 1.0, true, false, true);
    lBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 0.66, true, false, true);
    mBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 0.44, true, false, true);
    sBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 0.30, true, false, true);
}

/**
 * draw all branches
 * @param v is the vector of all branches
 */
void DrawBranches::draw(vector<Branch> v) {
    int i;
    for(i = 0; i < v.size(); i++){
        glPushMatrix();
        glMultMatrixf(v[i].flattenedMatrix);
        switch (v[i].currentLevel){
            case 0 :
                glCallList(xlBranch);
                break;
            case 1 :
                glCallList(lBranch);
                break;
            case 2 :
                glCallList(mBranch);
                break;
            case 3 :
                glCallList(sBranch);
                break;
            default:
                break;
        }
        glPopMatrix();
    }
}
