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

    float degreeValues[180];
    float DistenceValues[180];
    float AveragedegreeValues[90];
    float AverageValues[90];
    float AveragedegreeValues2[45];
    float AverageValues2[45];


    //custom struct for detected objects
    struct detectedObject objects[5]; //5 Possible objects

    while (1) {
        int i;
        bool detectingObject = false;
        for (i = 0; i<180; i+=1) { //scans every degree from 0 to 180 degrees (inclusive)
            cyBOT_Scan(i, &scanner);

            degreeValues[i] = i;
            DistenceValues[i] = scanner.sound_dist;

            //Prints sensor values to Putty
            char strDistance[20] = {0};
            float distance = DistenceValues[i];
            sprintf(strDistance, "%f", distance);
            char strDegree[20] = {0};
            int u = degreeValues[i];
            sprintf(strDegree, "%d", u);
            PuttyString(strDistance);
            PuttyString("mm @ ");
            PuttyString(strDegree);
            PuttyString(" degrees\n\r");

        }

        for (i = 0; i<90; i+=1) {//Populate with averages

            AverageValues[i] = (DistenceValues[i*2] + DistenceValues[(i*2)+1])/2.0;
            AveragedegreeValues[i] = (degreeValues[i*2] + degreeValues[(i*2)+1])/2.0;
        }

        for (i = 0; i<45; i+=1) {//Averages of the averages

            AverageValues2[i] = (AverageValues[i*2] + AverageValues[(i*2)+1])/2.0;
            AveragedegreeValues2[i] = (AveragedegreeValues[i*2] + AveragedegreeValues[(i*2)+1])/2.0;
        }

        float difference2 = 0.0;

        for (i = 0; i < 44; i+=1) {//Check values to find objects
            float difference;
            int objectCount = 0;
            if (AverageValues2[i] > AverageValues2[i+1]) {
                difference = AverageValues2[i] - AverageValues2[i+1];
            } else {
                difference = AverageValues2[i+1] - AverageValues2[i];
            }

            if (difference > 10.0 && difference > difference2 && i!=0) { //Might need to change constant being compared to difference
                if (detectingObject == false) { //already started scanning an object
                    objects[objectCount].startingAngle = AveragedegreeValues2[i];
                } else { //just started scanning an object
                    objects[objectCount].endingAngle = AveragedegreeValues2[i];
                    objects[objectCount].midpointAngle = objects[objectCount].endingAngle - objects[objectCount].startingAngle;
                    char strDegree[20] = {0};
                    sprintf(strDegree, "%d", objects[objectCount].midpointAngle);
                    PuttyString(strDegree);
                    PuttyString("KILL ME\n\r");
                    objectCount++;
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


