/*
 * custom.h
 *
 *  Created on: Feb 9, 2024
 *      @author Spencer Opitz
 */

#ifndef DETECTEDOBJECT_H_
#define DETECTEDOBJECT_H_

struct detectedObject {
    int startingAngle;
    int endingAngle;
    int midpointAngle;
    float distanceToObject;
};

#endif
