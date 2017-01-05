//
// Created by Qichen on 9/23/16.
//

#include <cstdlib>
#include <setup/ScenePrefs.h>
#include <nfd.h>
#include <setup/SceneSetupFileLoader.h>
#include <Lsystem/DrawBranches.h>
#include <boid/Cylinder.h>
#include <matrix/CameraFlattenedMatrix.h>
#include "UserInputManager.h"

static int* windowID;

ScenePrefs *prefsPointer;
Object ** allObjs;
/****
 * This function describes the behavior of the right mouse button menu.
 * @param window is the identifier the opengl window
 * @param preferences is the setup for current animation
 */
UserInputManager::UserInputManager(int * window, ScenePrefs * preferences, Object ** allObjects) {
    windowID = window;
    prefsPointer = preferences;
    allObjs = allObjects;
}

void UserInputManager::setMouseMenuBehavior(int id){
    switch (id)
    {
        case 1:
            // choose the user provided text file in a "native file dialog"
            if (loadUserInputFromFileDialog()) {
                prefsPointer->setInputLoaded(true);
                // prepare walls and fish's polygon and instances
                DrawObjects::prepareObjects(prefsPointer, allObjs);
                //prepare the polygons of reefs
                DrawBranches::prepare(prefsPointer);
                //TODO test this prepare the instances of Cylinder representing the reefs
                int i;
                Cylinder::setBottomOfTankY(prefsPointer->bottomWallY);
                for(i = 0; i< prefsPointer->numberOfObstacles; i++){
                    Cylinder(prefsPointer->listOfReefSizes[i], prefsPointer->listOfObsPositions[i]);
                }
            }
            break;
        //reset the preferences
        case 2 :
            prefsPointer->resetPreferences();
            break;
        case 3 :
            if (!prefsPointer->getAreInputLoaded()){
                /* TODO  show a message to let user load file first */
                break;
            }
            prefsPointer->setIsPlaying(true);
            break;
        default :
            break;
    }
}

/****
 * the callback function for mouse event, not yet populated
 */
void UserInputManager::mouseFunc (int button, int state, int x, int y) {
//   TODO implement something here, like changing view direction; drag and drap;
}

/****
 * define the entries of the right-mouse menu and call setMouseMenuBehavior to set the behavior
 */
void UserInputManager::createMouseMenu() {

    glutCreateMenu(UserInputManager::setMouseMenuBehavior);
    glutAddMenuEntry ("Load", 1);
    glutAddMenuEntry ("Reset", 2);
    glutAddMenuEntry ("Start", 3);
    glutAttachMenu (GLUT_RIGHT_BUTTON);
}

/****
 * @param key press 'q' 'Q' or ESC to quit; press 'a' to reset
 */
void UserInputManager::keyboardFunc(unsigned char key, int x, int y) {
    switch ((char) key) {
        case 'a':
            prefsPointer->resetPreferences();
            break;
        case 'r':
            CameraFlattenedMatrix::switchCamera();
            break;
        case 'q':
        case 'Q':
        case 27:    /* ESC */
            glutDestroyWindow(*windowID);
            exit(0);
        default:
            break;
    }
    return;
}

/****
 * Get the path to the user input script
 * @return true if loaded; false if not
 */
bool UserInputManager::loadUserInputFromFileDialog() {
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );

    if ( result == NFD_OKAY ) {
        SceneSetupFileLoader::loadPreferencesFromTextFile(outPath, prefsPointer);
        puts("Success!");
        puts(outPath);
        free(outPath);
        return true;
    }
    else if ( result == NFD_CANCEL ) {
        puts("User pressed cancel.");
    }
    else {
        printf("Error: %s\n", NFD_GetError() );
    }

    return false;
}
