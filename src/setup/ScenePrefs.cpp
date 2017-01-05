//
// Created by Qichen on 9/24/16.
//

#include "ScenePrefs.h"

const GLfloat ScenePrefs::TRUNK_SIZE_L = 45;
const GLfloat ScenePrefs::TRUNK_SIZE_M = 30;
const GLfloat ScenePrefs::TRUNK_SIZE_S = 20;
int ScenePrefs::counterForReefs= 0;
/****
 * This class contains the setup for the animation.
 * The fields except the list of quaternions are interpreted from the user input script.
 * It also contains the coefficient matrices for the current time instance.
 */
ScenePrefs::ScenePrefs() {
    resetPreferences();
}

// setters
void ScenePrefs::setInputLoaded(bool value) {
    areInputLoaded = value;
}

// getters
bool ScenePrefs::getAreInputLoaded() {
    return areInputLoaded;
}


void ScenePrefs::resetPreferences() {
    areInputLoaded = false;
    isPlaying = false;
    numberOfObjects = 0;
    numberOfObstacles = 0;
    bottomWallY = 1000;
}

/****
 * print out the infomation for debug purposes
 */
void ScenePrefs::printLoadedPreferences() {
    cout<< "number of objects: "<< numberOfObjects<<endl;
    int i ,j ;
    float l;
    for(i = 0; i < numberOfObjects; i++) {
        for(j = 0; j < 3; j++) {
        l =listOfPositions[i][j];
            cout<< l << '\t';
        }
        cout<< '\n';
    }
    cout<< '\n';
    for(i = 0; i < numberOfObjects; i++) {
        for(j = 0; j < 3; j++) {
            l =listOfEulerAngle[i][j];
            cout<< l << '\t';
        }
        cout<< '\n';
    }
}

void ScenePrefs::setIsPlaying(bool i) {
    isPlaying = i;
}

bool ScenePrefs::getIsPlaying() {
    return isPlaying;
}

void ScenePrefs::setNumberOfObjects(int i) {
    numberOfObjects = i;

}

void ScenePrefs::addOneInitPosition(GLfloat *onePositions) {
    int j;
    GLfloat *pv = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    for (j = 0; j < 3; j++){
        pv[j] = *(onePositions + j);
    }
    listOfPositions.push_back(pv);

    //TODO there is a bit of redundancy; may want to fix it later
    if (*(onePositions + 1) < bottomWallY){
        bottomWallY = *(onePositions + 1);
    }
}

void ScenePrefs::addOneInitOrientation(GLfloat *oneOrientation) {
    int j;
    GLfloat *ov = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    for (j = 0; j < 3; j++){
        ov[j] = *(oneOrientation + j);
    }
    listOfEulerAngle.push_back(ov);
}

void ScenePrefs::addOneObsPosition(GLfloat *p) {
    int j;
    GLfloat *pv = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    for (j = 0; j < 3; j++){
        pv[j] = *(p + j);
    }
    listOfObsPositions.push_back(pv);
    // note that for now the y position is just a placeholder
    // it will be adjusted when the size of reef is given in addOneSizeOfReef(int size)
}

void ScenePrefs::addOneObsOrientation(GLfloat *obsO) {
    int j;
    GLfloat *ov = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    for (j = 0; j < 3; j++){
        ov[j] = *(obsO + j);
    }
    listOfObsEulerAngle.push_back(ov);
}

void ScenePrefs::addOneVelocity(GLfloat *oneVelocity) {
    int j;
    GLfloat *vv = (GLfloat *) malloc(sizeof(GLfloat) * 3);
    for (j = 0; j < 3; j++){
        vv[j] = *(oneVelocity + j);
    }
    listOfVelocity.push_back(vv);
}

void ScenePrefs::addOneSphereRadius(GLfloat m) {
    listOfSphereRadius.push_back(m);
}

void ScenePrefs::setNumberOfObstacles(int n) {
    ScenePrefs::numberOfObstacles = n;
}

void ScenePrefs::addOneSizeOfReef(int s) {
    listOfReefSizes.push_back(s);
    //adjust the y post of the centroid of the trunk (main branch) of the reef
    GLfloat trunkSize = 30;
    switch (s) {
        case 3:
            trunkSize = TRUNK_SIZE_L;
            break;
        case 2:
            trunkSize = TRUNK_SIZE_M;
            break;
        case 1:
            trunkSize = TRUNK_SIZE_S;
            break;
        default:
            break;
    }
    *(listOfObsPositions[counterForReefs] + 1) = (GLfloat) (bottomWallY + 0.5 * trunkSize);

    counterForReefs++;
}
