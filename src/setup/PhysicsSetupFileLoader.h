//
// Created by Qichen on 11/13/16.
//

#ifndef BOIDS_PHYSICSSETUPFILELOADER_H
#define BOIDS_PHYSICSSETUPFILELOADER_H

#include "boid/Object.h"
#include "PhysicsPrefs.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <string>

#else
#include <GL/glut.h>
#endif

class PhysicsSetupFileLoader {
public:
    static void loadPreferencesFromTextFile(char *path, PhysicsPrefs *prefs);
};


#endif //BOIDS_PHYSICSSETUPFILELOADER_H
