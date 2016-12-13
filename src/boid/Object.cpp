//
// Created by Qichen on 11/12/16.
//

#include "Object.h"


const GLfloat Object::BOTTOM_WALL_Y = -50;
const GLfloat Object::TOP_WALL_Y = 50;
const GLfloat Object::LEFT_WALL_X = -50;
const GLfloat Object::RIGHT_WALL_X = 50;
const GLfloat Object::NEAR_WALL_Z = 50;
const GLfloat Object::REAR_WALL_Z = -50;

/***
 * Object is the base class of Ball; a wall is an instance of Object
 */
Object::Object(int id, GLuint listId, GLfloat mass, bool isFixed, GLfloat *orientation, GLfloat *translation) {
    Object::id = id;
    Object::listId = listId;
    Object::mass = mass;
    Object::isFixed = isFixed;

    int i;
    for (i = 0 ; i < 3; i++) {
        *(Object::translation + i) = *(translation + i);
        *(Object::orientation + i) = *(orientation + i);
    }

    setFlattenedTransformationMatrix(RotationHelper::
                                     generateFlattenedTransformationMatrix(orientation,translation,false));

}

/***
 * The following functions are all getters and setters
 */
void Object::setFlattenedTransformationMatrix(GLfloat *flattenedTransformationMatrix) {
    int i;
    for (i = 0 ; i < 16; i++) {
        *(Object::flattenedTransformationMatrix + i) = *(flattenedTransformationMatrix + i);
    }
}

GLuint Object::getListId() {
    return listId;
}

GLfloat *Object::getFlattenedTransformationMatrix() {
    return flattenedTransformationMatrix;
}

GLfloat *Object::getTranslation() {
    return translation;
}

GLfloat Object::getX() {
    return *translation;
}

GLfloat Object::getY() {
    return *(translation + 1);
}

GLfloat Object::getZ() {
    return *(translation + 2);
}
