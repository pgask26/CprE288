#include "custom.h"
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "cyBot_uart.h"
#include <stdio.h>
#include <string.h>

struct detectedObject {
    int startingAngle;
    int endingAngle;
    int midpointAngle;
    float distanceToObject;
};
