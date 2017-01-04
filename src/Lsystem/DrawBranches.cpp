//
// Created by Qichen on 1/3/17.
//

#include "DrawBranches.h"

const char * DrawBranches::BRANCH_OBJ_MAME = "branch.obj";
GLuint DrawBranches::xxlBranch;
GLuint DrawBranches::xlBranch;
GLuint DrawBranches::lBranch;
GLuint DrawBranches::mBranch;
GLuint DrawBranches::sBranch;
GLuint DrawBranches::xsBranch;

/**
 * load the obj file and manipulate its size for different level of branches,
 * then create a vector of instances of Branch to represent reefs
 */
void DrawBranches::prepare(ScenePrefs *p) {
    //There are 6 "levels" of branches {-1, 0, 1, 2, 3, 4}
    xxlBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 1.5, true, false, true);
    xlBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 1.0, true, false, true);
    lBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 0.66, true, false, true);
    mBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 0.44, true, false, true);
    sBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 0.30, true, false, true);
    xsBranch = SimpleObjLoader::loadObj((char *) BRANCH_OBJ_MAME, 1, 0.20, true, false, true);
    int i;
    GLfloat * fm;
    vector<GLfloat *> eulerAngles = p->listOfObsEulerAngle;
    vector<GLfloat *> positions= p->listOfObsPositions;
    vector<int> sizes= p->listOfReefSizes;
    for(i = 0; i < p->numberOfObstacles; i++){
        fm = RotationHelper::generateFlattenedTransformationMatrix(eulerAngles[i], positions[i], false);
        // map sizes {3,2,1} to "levels" of branches {-1,0,1}; branch level is the first parameter
        Branch(2 - sizes[i] , fm);
    }
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
            case -1 :
                glCallList(xxlBranch);
                break;
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
            case 4 :
                glCallList(xsBranch);
                break;
            default:
                break;
        }
        glPopMatrix();
    }
}
