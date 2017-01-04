//
// Created by Qichen on 9/23/16.
//

#ifndef BOIDS_USERINPUTMANAGER_H
#define BOIDS_USERINPUTMANAGER_H

// glut
#if defined(__APPLE__)
#include <GLUT/glut.h>
#include "setup/ScenePrefs.h"
#include "boid/DrawObjects.h"

#else
#include <GL/glut.h>
#endif

class UserInputManager {
private:
    static void setMouseMenuBehavior(int id);

public:
    UserInputManager(int * window, ScenePrefs * preferences, Object ** allObjects);
    static void createMouseMenu();
    static void mouseFunc (int button, int state, int x, int y);
    static void keyboardFunc(unsigned char, int, int);

    static bool loadUserInputFromFileDialog();
};


#endif //BOIDS_USERINPUTMANAGER_H
