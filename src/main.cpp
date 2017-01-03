//  Created by Qichen on Nov 12, 2016
// As required opengl toolkit's api about any matrix and vector related
// calculations are not used. The author implemented them.
//  Copyright © 2016 Qichen Wang. All rights reserved.
//

#include <Lsystem/Branch.h>
#include <Lsystem/DrawBranches.h>
#include "boid/BoundaryDetector.h"
#include "UserInputManager.h"
#include "StringUtils.h"

using namespace std;

Object *objects[50];
PhysicsPrefs prefs;
GLfloat increment = 0.03;
//TODO add this to script
GLfloat flattenedForReef[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -35, 0, 1};
GLfloat flattenedForReef_2[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 20, -35, -40, 1};
int window;

void drawFrame();

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat) w / (GLfloat) h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

/***
 * display the figure when idle
 */
void displayObject() {

    glLoadIdentity();
    glColor3f(0.1, 0.45, 0.1);
    glMatrixMode(GL_MODELVIEW);
    //move the model view away from the camera, so that we are not inside the object1
    glMultMatrixf((GLfloat []){1,0,0,0,0,1,0,0,0,0,1,0,0,0,-150,1});

    //TODO insert real local rotation and translation
    if(objects[0]!= nullptr) {
        DrawObjects::draw(objects);
    }
    //TODO test this
    DrawBranches::draw(Branch::listOfNodes);
}

/****
 * This function is for drawing the frames in the interpolated animation.
 */
void drawFrame() {
    glLoadIdentity();
    glPushMatrix();
    //move the model view away from the camera, so that we are not inside the object
//    glMultMatrixf((GLfloat []){0.707,0,0.707,0,0,1,0,0,-0.707,0,0.707,0,0,0,-180,1});
   glMultMatrixf((GLfloat []){1,0,0,0,0,1,0,0,0,0,1,0,0,0,-150,1});
    glColor3f(0.1, 0.45, 0.1);
    int i;
    GLfloat tempSum[3] = {};
    //check if 1. two boids are close enough, then they mark each other as neighbours
    // or 2. a boids is too close to the walls, the eliminate the velocity in the corresponding directions
    BoundaryDetector::detectAll(objects, prefs.numberOfObjects);
    // if one goal is met, generate another goal
    Fish::updateGoal();
    for(i = 6; i <prefs.numberOfObjects; i++){
        ((Fish *) objects[i])->getCombinedDesires();
        //move the boid
        ((Fish *) objects[i])->updateFlattenedTransformationMatrix(increment);
        ((Fish *) objects[i])->rotateBodyAndTail();
        tempSum[0] += objects[i]->getTranslation()[0];
        tempSum[1] += objects[i]->getTranslation()[1];
        tempSum[2] += objects[i]->getTranslation()[2];
    }
    Fish::updateFlockCentroid(tempSum);
    DrawObjects::draw(objects);
    //TODO test this
    DrawBranches::draw(Branch::listOfNodes);
    glPopMatrix();
}

void display(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UserInterfaceManager::printMessageForBouncingBalls(
            prefs.numberOfObjects, DrawObjects::NUMBER_OF_WALLS, prefs.getIsPlaying());
    if(!prefs.getIsPlaying()){
        // if there is no user input, show an walking in straight line animation
        displayObject();
    } else {
        drawFrame();
    }
    glutSwapBuffers(); //swap the buffers

}

/**
 *  Note that openGL works in a right-handed coordinate system by default
 * **/
int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    window = glutCreateWindow("CSCI 6555 project 4 : BOIDS");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(UserInputManager::keyboardFunc);
    glutMouseFunc(UserInputManager::mouseFunc);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // initiate an instance of prefs to store the user preference
    UserInputManager(&window, &prefs, objects);

    UserInputManager::createMouseMenu();
    //TODO fix this, this function is also called in userInputManager
    DrawObjects::prepareObjects(&prefs,objects);
    //prepare the reef
    Branch(0, flattenedForReef);
    Branch(0, flattenedForReef_2);
    DrawBranches::prepare();
    glutMainLoop();
    return 0;
}