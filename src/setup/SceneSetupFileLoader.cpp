//
// Created by Qichen on 9/25/16. Last modified on 1/4/2017
//

#include <cstdio>
#include <iostream>
#include "SceneSetupFileLoader.h"
using namespace std;
/****
 * This is called inside UserInputManager::loadUserInputFromFileDialog()
 * @param path is provided by UserInputManager::loadUserInputFromFileDialog()
 * @param pPreferences
 */
void SceneSetupFileLoader::loadPreferencesFromTextFile(char *path, ScenePrefs *prefs) {
    FILE *setupFilePointer;
    GLfloat tempFloat[3] = {};
    GLfloat tempRadius = 0;
    int tempSize= 0;
    int numberOfWalls = 6;
    int numberOfObstacles= 0;
    int numberOfObjects = 6;
    int count;
    char firstWord[16];
    setupFilePointer = fopen(path, "r");
    if (!setupFilePointer) {
        printf("can't open file %s\n", path);
        exit(1);
    }

    while (1) {
        if (fscanf(setupFilePointer, "%s", firstWord) == EOF) {
            // read the next word, if returns EOF then break
            break;
        } else if (firstWord[0] == '#') {
            // if reach comment break
            break;
        }
        //The following chuck of code interpret user input from the file, how to write a user input script
        // is shown in the .txt files in /data
        if (strcmp(firstWord, "n") == 0) {
            fscanf(setupFilePointer, "%d", &numberOfObjects);
            prefs->setNumberOfObjects(numberOfObjects);
        } else if (strcmp(firstWord, "x") == 0) {
            fscanf(setupFilePointer, "%d", &numberOfObstacles);
            prefs->setNumberOfObstacles(numberOfObstacles);
        }  else if (strcmp(firstWord, "p") == 0) {
            fscanf(setupFilePointer, "%f %f %f", &tempFloat[0], &tempFloat[1], &tempFloat[2]);
            // save positions in preferences
            prefs->addOneInitPosition(tempFloat);
        } else if (strcmp(firstWord, "k") == 0) {
            fscanf(setupFilePointer, "%f %f %f", &tempFloat[0], &tempFloat[1], &tempFloat[2]);
            // save positions in preferences
            prefs->addOneObsPosition(tempFloat);
        } else if (strcmp(firstWord, "e") == 0) {
            fscanf(setupFilePointer, "%f %f %f", &tempFloat[0], &tempFloat[1], &tempFloat[2]);
            //save euler angle in preferences
            prefs->addOneInitOrientation(tempFloat);
        } else if (strcmp(firstWord, "j") == 0) {
            fscanf(setupFilePointer, "%f %f %f", &tempFloat[0], &tempFloat[1], &tempFloat[2]);
            //save euler angle in preferences
            prefs->addOneObsOrientation(tempFloat);
        } else if(strcmp(firstWord, "v") == 0) {
            fscanf(setupFilePointer, "%f %f %f", &tempFloat[0], &tempFloat[1], &tempFloat[2]);
            prefs->addOneVelocity(tempFloat);
        }else if (strcmp(firstWord, "r") == 0){
            for(count = 0; count < numberOfObjects - numberOfWalls; count++){
                fscanf(setupFilePointer, "%f", &tempRadius);
                prefs->addOneSphereRadius(tempRadius);
            }
        }else if (strcmp(firstWord, "s") == 0){
            for(count = 0; count < numberOfObstacles; count++){
                fscanf(setupFilePointer, "%d", &tempSize);
                prefs->addOneSizeOfReef(tempSize);
            }
        }
    }
    fclose(setupFilePointer);
}
