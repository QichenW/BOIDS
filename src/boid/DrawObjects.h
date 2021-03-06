//
// Created by Qichen on 11/12/16.
//

#ifndef BOIDS_DRAWOBJECTS_H
#define BOIDS_DRAWOBJECTS_H
#include "Fish.h"
#include "Object.h"
#include "setup/SimpleObjLoader.h"
#include "matrix/RotationHelper.h"
#if defined(__APPLE__)
#include <GLUT/glut.h>
#include "setup/ScenePrefs.h"
#include <vector>


#else
#include <GL/glut.h>
#endif

class DrawObjects {
public:
    static void draw(Object **objects);

    static int numberOfObjects;
    
    static const int NUMBER_OF_WALLS;


    static void prepareObjects(ScenePrefs *pPrefs, Object **pObjects);

private:
    static const char* HEAD_OBJ_MAME;
    static const char* BODY_OBJ_MAME;
    static const char* TAIL_OBJ_MAME;

    static const char *WALL_OBJ_NAME;

    static const char *WALL_2_OBJ_NAME;
};


#endif //BOIDS_DRAWOBJECTS_H
