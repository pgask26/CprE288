/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include <string.h>
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "cyBot_uart.h"
#include "custom.h"
#include "cyBot_Scan.h"
#include <detectedObject.h>


void main (void) {

    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    cyBot_uart_init();

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    //PART1 CYBOT COMMUNICATION USING PUTTY
    cyBOT_Scan_t scanner;
    calloc(1, sizeof(cyBOT_Scan_t));

    //PART2 COLLECT SENSOR DATA AND IDENTIFY OBJECTS IN THE DATA
    //TWO OR MORE TALL OBJECTS SPACED APART IN FIELD
    cyBOT_init_Scan(0b0111);

    int degreeValues[90];
    float DistenceValues[90];


    //custom struct for detected objects
    struct detectedObject objects[5]; //5 Possible objects

    while (1) {
        char strDistance[20] = {0};
        char strDegree[20] = {0};
        int i;
        bool detectingObject = false;
        for (i = 0; i<180; i+=2) { //scans every 2 degrees from 0 to 178 degrees (inclusive)
            cyBOT_Scan(i, &scanner);

            DistenceValues[i] = scanner.sound_dist;

            if (DistenceValues[i] == 0.000000f || DistenceValues[i] < 0.000000f) { //Might be weird if 0 detected at first index position but check for phantom values not working???
                DistenceValues[i] = DistenceValues[i-1];
            }

            degreeValues[i] = i;
            //Prints sensor values to Putty
            float distance = DistenceValues[i];
            sprintf(strDistance, "%f", distance);
            int u = degreeValues[i];
            sprintf(strDegree, "%d", u);
            PuttyString(strDistance);
            PuttyString("mm @ ");
            PuttyString(strDegree);
            PuttyString(" degrees\n\r");

        }

        float difference2 = 0;
        int count = 0;

        for (i = 0; i < 90; i+=1) {//Check values to find objects
            float difference;

            if (DistenceValues[i] > DistenceValues[i+1] ) {
                difference = DistenceValues[i] - DistenceValues[i+1];
            } else {
                difference = DistenceValues[i+1] - DistenceValues[i];
            }

            if (difference > 30.0 && difference > difference2 && i != 0) { //Might need to change constant being compared to difference
                if (detectingObject == false) { //Start scanning an object
                    objects[count].startingAngle = degreeValues[i];
                    detectingObject = true;
                } else { //Finished scanning an object
                    objects[count].endingAngle = degreeValues[i];
                    objects[count].midpointAngle = objects[count].endingAngle - objects[count].startingAngle;

                    int distance = objects[count].midpointAngle;
                    sprintf(strDistance, "%d", distance);
                    PuttyString(strDistance);
                    PuttyString(" Midpoint angle\n\r");

                    detectingObject = false;
                    count++;
                }
            }

            difference2 = difference;
        }
    }

    //PART3 DETECT THE SMALLEST WIDTH OBJECT


    //PART4 DRIVE TO THE SMALLEST WIDTH OBJECT (extra credit)


    oi_free(sensor_data); // do this once at end of main()

    return;
}


