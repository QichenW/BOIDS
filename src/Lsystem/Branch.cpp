//
// Created by Qichen on 1/3/17.
//

#include "Branch.h"

vector<Branch> Branch::listOfNodes;
//big tree has 6 levels of branches {-1,0,1,2,3,4}
//medium tree has 6 levels of branches {0,1,2,3,4}
//small tree has 6 levels of branches {1,2,3,4}
const int Branch::LOWEST_LEVEL = 4;
const GLfloat Branch::UNIT_CHILD_1_TRANS_1[3] = {0, 15, 0};
const GLfloat Branch::UNIT_CHILD_1_TRANS_2[3] = {0, -15, 0};
const GLfloat Branch::CHILD_1_ROTTN[3] = {45, 100, 0};
const GLfloat Branch::UNIT_CHILD_2_TRANS_1[3] = {0, 15, 0};
const GLfloat Branch::UNIT_CHILD_2_TRANS_2[3] = {0, -5, 0};
const GLfloat Branch::CHILD_2_ROTTN[3] = {45, 200, 0};
const GLfloat Branch::UNIT_CHILD_3_TRANS_1[3] = {0, 15, 0};
const GLfloat Branch::UNIT_CHILD_3_TRANS_2[3] = {0, 5, 0};
const GLfloat Branch::CHILD_3_ROTTN[3] = {45, 300, 0};
GLfloat Branch::ZERO_VECTOR[3] = {0, 0, 0};

/**
 * recursively add to the linked list "listOfNodes" instances of Branch
 * @param level the level of branch, used in DrawBranches to identify which GL drawing list to call
 * to represent this branch
 * @param fM the flattened transformation matrix for this branch.
 */
Branch::Branch(int level, GLfloat *fM) {
    currentLevel = level;
    childBranchSize = (GLfloat) pow(0.66, 1 + level);
    int i;
    for(i = 0; i < 16; i++){
        *(flattenedMatrix + i) = *(fM + i);
    }
    if (currentLevel <LOWEST_LEVEL){
        GLfloat child_1_t_1[3], child_1_t_2[3], child_2_t_1[3], child_2_t_2[3], child_3_t_1[3], child_3_t_2[3];
        for(i = 0; i < 3; i++){
            child_1_t_1[i] = UNIT_CHILD_1_TRANS_1[i] * childBranchSize;
            child_1_t_2[i] = UNIT_CHILD_1_TRANS_2[i] * childBranchSize;
            child_2_t_1[i] = UNIT_CHILD_2_TRANS_1[i] * childBranchSize;
            child_2_t_2[i] = UNIT_CHILD_2_TRANS_2[i] * childBranchSize;
            child_3_t_1[i] = UNIT_CHILD_3_TRANS_1[i] * childBranchSize;
            child_3_t_2[i] = UNIT_CHILD_3_TRANS_2[i] * childBranchSize;
        }
        //create child 1
        GLfloat tempMatrix_1[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        RotationHelper::rightDotProduct(tempMatrix_1, flattenedMatrix);

        RotationHelper::rightDotProduct(tempMatrix_1, RotationHelper::
        generateFlattenedTransformationMatrix(ZERO_VECTOR, child_1_t_2, false));

        RotationHelper::rightDotProduct(tempMatrix_1,
                                        RotationHelper::generateFlattenedTransformationMatrix(
                                                (float *) CHILD_1_ROTTN, nullptr, false));

        RotationHelper::rightDotProduct(tempMatrix_1,  RotationHelper::
        generateFlattenedTransformationMatrix(ZERO_VECTOR, child_1_t_1, false));

         //create child 2
        GLfloat tempMatrix_2[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        RotationHelper::rightDotProduct(tempMatrix_2, flattenedMatrix);

        RotationHelper::rightDotProduct(tempMatrix_2, RotationHelper::
        generateFlattenedTransformationMatrix(ZERO_VECTOR, child_2_t_2, false));

        RotationHelper::rightDotProduct(tempMatrix_2,
                                        RotationHelper::generateFlattenedTransformationMatrix(
                                                (float *) CHILD_2_ROTTN, nullptr, false));

        RotationHelper::rightDotProduct(tempMatrix_2,  RotationHelper::
        generateFlattenedTransformationMatrix(ZERO_VECTOR, child_2_t_1, false));

         //create child 3
        GLfloat tempMatrix_3[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        RotationHelper::rightDotProduct(tempMatrix_3, flattenedMatrix);

        RotationHelper::rightDotProduct(tempMatrix_3, RotationHelper::
        generateFlattenedTransformationMatrix(ZERO_VECTOR, child_3_t_2, false));

        RotationHelper::rightDotProduct(tempMatrix_3,
                                        RotationHelper::generateFlattenedTransformationMatrix(
                                                (float *) CHILD_3_ROTTN, nullptr, false));

        RotationHelper::rightDotProduct(tempMatrix_3,  RotationHelper::
        generateFlattenedTransformationMatrix(ZERO_VECTOR, child_3_t_1, false));


        //Recursive call
        Branch(currentLevel + 1, tempMatrix_1);
        Branch(currentLevel + 1, tempMatrix_2);
        Branch(currentLevel + 1, tempMatrix_3);
    }
    listOfNodes.push_back(*this);
}
