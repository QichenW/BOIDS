//
// Created by Qichen on 1/5/17.
//

#ifndef BOIDS_CAMERAFLATTENEDMATRIX_H
#define BOIDS_CAMERAFLATTENEDMATRIX_H

#if defined(__APPLE__)
#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif

class CameraFlattenedMatrix {
private:
    static const GLfloat FRONT[16];
    static const GLfloat DIAGNAL[16];
    static const GLfloat SIDE[16];
public:
    static int currentID;
    static GLfloat * getCurrentCamera();
    static void switchCamera();
};


#endif //BOIDS_CAMERAFLATTENEDMATRIX_H
