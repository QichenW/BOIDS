//
// Created by Qichen on 1/5/17.
//

#include "CameraFlattenedMatrix.h"

const GLfloat CameraFlattenedMatrix::FRONT[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -150, 1};
const GLfloat CameraFlattenedMatrix::DIAGNAL[16] =
        {0.707, 0, 0.707, 0, 0, 1, 0, 0, -0.707, 0, 0.707, 0, 0, 0, -150, 1};
const GLfloat CameraFlattenedMatrix::SIDE[16] =   {0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, -200, 1};
int CameraFlattenedMatrix::currentID = 0;

GLfloat *CameraFlattenedMatrix::getCurrentCamera() {
    switch (currentID){
        case 0:
            return (GLfloat *) FRONT;
        case 1:
            return (GLfloat *) DIAGNAL;
        case 2:
            return (GLfloat *) SIDE;
        default:
            return (GLfloat *) FRONT;
    }
}

void CameraFlattenedMatrix::switchCamera() {
    if(currentID < 2){
        currentID++;
    } else {
        currentID = 0;
    }
}



GLfloat* getCurrentCamera(){

}