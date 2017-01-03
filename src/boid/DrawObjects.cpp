//
// Created by Qichen on 11/12/16.
//



#include <iostream>
#include "DrawObjects.h"

const char * DrawObjects::HEAD_OBJ_MAME = "fish_head.obj";
const char * DrawObjects::BODY_OBJ_MAME = "fish_body.obj";
const char * DrawObjects::TAIL_OBJ_MAME = "fish_tail.obj";
//const char * DrawObjects::BODY_OBJ_MAME = "fishBody.obj";
const char * DrawObjects::WALL_OBJ_NAME= "wall.obj";
const char * DrawObjects::WALL_2_OBJ_NAME= "wall2.obj";
int DrawObjects::numberOfObjects;
const int DrawObjects::NUMBER_OF_WALLS = 6;



void DrawObjects::draw(Object **objects) {
    int i;
    Fish * fish;
    for (i = 0; i < NUMBER_OF_WALLS; i++) {
            glPushMatrix();
            glMultMatrixf(objects[i]->getFlattenedTransformationMatrix());
            glCallList(objects[i]->getListId());
            glPopMatrix();
    }
    for (i = NUMBER_OF_WALLS; i < numberOfObjects; i++){
        GLfloat combinedFlatTransformation[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        //Draw fish head
        fish = (Fish *) objects[i];
        glPushMatrix();
        glMultMatrixf(fish->getFlattenedTransformationMatrix());
        glCallList(fish->getListId());
        glPopMatrix();
        //Draw fish body
        glPushMatrix();
        RotationHelper::rightDotProduct(combinedFlatTransformation, fish->getFlattenedTransformationMatrix());
        RotationHelper::rightDotProduct(combinedFlatTransformation, fish->bodyAlignFlat2);
        RotationHelper::rightDotProduct(combinedFlatTransformation,
                                        RotationHelper::generateFlattenedTransformationMatrix(
                                                fish->bodyLocalRotation, nullptr, false));
        RotationHelper::rightDotProduct(combinedFlatTransformation, fish->bodyAlignFlat1);
        glMultMatrixf(combinedFlatTransformation);
        glCallList(fish->bodyListId);
        glPopMatrix();
        //set individual centroid as the centroid of body here
        fish->setIndividualCentroid(combinedFlatTransformation[12], combinedFlatTransformation[13],
                                    combinedFlatTransformation[14]);
        //Draw fish tail
        glPushMatrix();
        RotationHelper::rightDotProduct(combinedFlatTransformation, fish->tailAlignFlat2);
        RotationHelper::rightDotProduct(combinedFlatTransformation,
                                        RotationHelper::generateFlattenedTransformationMatrix(
                                                fish->tailLocalRotation, nullptr, false));
        RotationHelper::rightDotProduct(combinedFlatTransformation, fish->tailAlignFlat1);
        glMultMatrixf(combinedFlatTransformation);
        glCallList(fish->tailListId);
        glPopMatrix();

    }
}

//TODO test this
/***
 *  called in upserInputManager, when input file is loaded in to prefs, initiate **object
 * @param pPrefs
 * @param pObjects
 */
void DrawObjects::prepareObjects(PhysicsPrefs *pPrefs, Object **pObjects) {
    DrawObjects::numberOfObjects = pPrefs->numberOfObjects;
    unsigned long i;
    int j,k;
    GLfloat defaultScale = 2.6, tempRadius, realRadius;
    vector<GLfloat *> orientations, positions, velocities, angularVelos;

    GLuint wallObjID = SimpleObjLoader::loadObj((char *) WALL_OBJ_NAME, 1, 1.0, true, false, false);
    GLuint wall2ObjID = SimpleObjLoader::loadObj((char *) WALL_2_OBJ_NAME, 1, 1.0, true, false, false);

    GLuint sHeadID = SimpleObjLoader::loadObj((char *) HEAD_OBJ_MAME, 1, 1, true, false, true);
    GLuint sBodyID = SimpleObjLoader::loadObj((char *) BODY_OBJ_MAME, 1, 1, true, false, true);
    GLuint sTailID = SimpleObjLoader::loadObj((char *) TAIL_OBJ_MAME, 1, 1, true, false, true);

    GLuint mHeadID = SimpleObjLoader::loadObj((char *) HEAD_OBJ_MAME, 1, 1.5, true, false, true);
    GLuint mBodyID = SimpleObjLoader::loadObj((char *) BODY_OBJ_MAME, 1, 1.5, true, false, true);
    GLuint mTailID = SimpleObjLoader::loadObj((char *) TAIL_OBJ_MAME, 1, 1.5, true, false, true);

    GLuint lHeadID = SimpleObjLoader::loadObj((char *) HEAD_OBJ_MAME, 1, 2.0, true, false, true);
    GLuint lBodyID = SimpleObjLoader::loadObj((char *) BODY_OBJ_MAME, 1, 2.0, true, false, true);
    GLuint lTailID = SimpleObjLoader::loadObj((char *) TAIL_OBJ_MAME, 1, 2.0, true, false, true);

    orientations = pPrefs->listOfEulerAngle;
    positions = pPrefs->listOfPositions;
    velocities = pPrefs->listOfVelocity;
    angularVelos = pPrefs->listOfAngularVelo;
//TODO test only
    for(i = 0; i < numberOfObjects; i++){
        for(j = 0; j < 3; j++){
            if( i < NUMBER_OF_WALLS){
                cout<<orientations.at(i)[j] << "/" << positions.at(i)[j] << "\t";
            } else {
                cout<<orientations.at(i)[j] << "/" << positions.at(i)[j] << "/"
                    << velocities.at(i - NUMBER_OF_WALLS)[j]<< "/" <<angularVelos.at(i - NUMBER_OF_WALLS)[j]<< "\t";
            }
        }
        cout<<"\n";
    }

    for (k = 0; k < numberOfObjects; k++){
        if (k < 2) {
            *(pObjects + k) = new Object(k,wallObjID, true, orientations.at(k), positions.at(k));
        } else if(k < NUMBER_OF_WALLS){
            *(pObjects + k) = new Object(k,wall2ObjID, true, orientations.at(k), positions.at(k));
        } else {
            Fish::amount++;
            tempRadius = pPrefs->listOfSphereRadius.at(k - NUMBER_OF_WALLS);
            realRadius = tempRadius * defaultScale;
            //record biggest radius of the boids' enclosing sphere
            if(realRadius > Fish::biggestRadius){
                Fish::biggestRadius = realRadius;
            }
            if(tempRadius<1.48){
                *(pObjects + k) = new Fish(k, sHeadID, false, orientations.at(k), positions.at(k),
                                           velocities .at(k - NUMBER_OF_WALLS),
                                          realRadius, sBodyID, sTailID, tempRadius); //the radius of the ball in .obj file is 2.4
            } else if(tempRadius < 1.98){
                *(pObjects + k) = new Fish(k, mHeadID, false, orientations.at(k), positions.at(k),
                                           velocities .at(k - NUMBER_OF_WALLS),
                                           realRadius, mBodyID, mTailID, tempRadius);
            } else {
                *(pObjects + k) = new Fish(k, lHeadID, false, orientations.at(k), positions.at(k),
                                           velocities .at(k - NUMBER_OF_WALLS),
                                           realRadius, lBodyID, lTailID, tempRadius);
            }

        }
    }

}
