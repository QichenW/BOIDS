//
// Created by Qichen on 9/24/16.
//

#ifndef BOIDS_STRINGUTILS_H
#define BOIDS_STRINGUTILS_H

// glut
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class UserInterfaceManager {
public:
    static void printMessageForFishTank(int numberOfFish, int numberOfReefs, bool isPlaying);

private:
    static char* buildString(int nb, int nr, const char **pointer, int numberOfSegments);

    static void printInWindow(char *strInfo, bool isStatusInfo);

    static char *buildString(const char **pointer, int numberOfSegments);
};


#endif //BOIDS_STRINGUTILS_H
