//
// Created by Qichen on 9/24/16.
//

#ifndef BOUNCINGBALLS_PHYSICSPREFS_H
#define BOUNCINGBALLS_PHYSICSPREFS_H
#include <iostream>
#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <string>
#include <vector>
#else
#include <GL/glut.h>
#endif
using namespace std;
class ScenePrefs {
private:
    bool areInputLoaded, isPlaying;
    static const GLfloat TRUNK_SIZE_L;
    static const GLfloat TRUNK_SIZE_M;
    static const GLfloat TRUNK_SIZE_S;
    static int counterForReefs;

public:
    ScenePrefs();

    bool getAreInputLoaded();

    void setInputLoaded(bool value);

    void resetPreferences();

    void printLoadedPreferences();

    void setIsPlaying(bool i);

    bool getIsPlaying();

    int numberOfObjects;
    int numberOfObstacles;
    GLfloat bottomWallY;
    vector<GLfloat*> listOfEulerAngle;
    vector<GLfloat*> listOfPositions;
    vector<GLfloat*> listOfVelocity;
    vector<GLfloat*> listOfAngularVelo;
    vector<GLfloat> listOfSphereRadius;
    vector<int> listOfReefSizes;
    vector<GLfloat*> listOfObsPositions;
    vector<GLfloat*> listOfObsEulerAngle;

    void setNumberOfObstacles(int numberOfObstacles);

    void setNumberOfObjects(int i);

    void addOneInitPosition(GLfloat *onePositions);

    void addOneInitOrientation(GLfloat *oneOrientation);

    void addOneVelocity(GLfloat *oneVelocity);

    void addOneAngularVelo(GLfloat *oneAngularVelo);

    void addOneSphereRadius(GLfloat m);

    void addOneObsPosition(GLfloat *p);

    void addOneObsOrientation(GLfloat *obsO);

    void addOneSizeOfReef(int s);
};


#endif //BOUNCINGBALLS_PHYSICSPREFS_H
