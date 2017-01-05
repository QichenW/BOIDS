//
// Created by Qichen on 1/4/17.
//

#include "Cylinder.h"
vector<Cylinder> Cylinder::cylinders;
const GLfloat Cylinder::L_TRUNK_HEIGHT = 45;
const GLfloat Cylinder::M_TRUNK_HEIGHT = 30;
const GLfloat Cylinder::S_TRUNK_HEIGHT = 20;
const GLfloat Cylinder::RATIO_H= 1.22; // approximately the height of cylinder is 1.222 * trunk_height
const GLfloat Cylinder::RATIO_R= 0.8; // approximately the radius of cylinder is 0.8 * trunk_height
//const GLfloat Cylinder::RATIO_R= 0.56; // approximately the radius of cylinder is 0.56 * trunk_height
GLfloat Cylinder::bottomOfTankY = 999;
int Cylinder::amount= 0;

void Cylinder::setBottomOfTankY(GLfloat y) {
    Cylinder::bottomOfTankY = y;
}

Cylinder::Cylinder(int size, GLfloat *trunkCentroidPosition) {
    axisCoordinateX = *trunkCentroidPosition;
    axisCoordinateZ = *(trunkCentroidPosition + 2);
    switch(size) {
        case 3:
            yPosOfTopSurface = bottomOfTankY + RATIO_H * L_TRUNK_HEIGHT;
            radius = RATIO_R * L_TRUNK_HEIGHT;
            break;
        case 2:
            yPosOfTopSurface = bottomOfTankY + RATIO_H * M_TRUNK_HEIGHT;
            radius = RATIO_R * M_TRUNK_HEIGHT;
            break;
        case 1:
            yPosOfTopSurface = bottomOfTankY + RATIO_H * S_TRUNK_HEIGHT;
            radius = RATIO_R * S_TRUNK_HEIGHT;
            break;
        default:
            yPosOfTopSurface = bottomOfTankY + RATIO_H * M_TRUNK_HEIGHT;
            radius = RATIO_R * M_TRUNK_HEIGHT;
            break;
    }
    amount++;
    cylinders.push_back(*this);
}
