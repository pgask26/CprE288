///////////////////////////////////////////////////////////////////////
// Name: Simple-CyBot-echo.c                                         //
// Author: Phillip Jones (phjones@iastate.edu)                       //
// Date: 09/26/2023                                                  //
// Description: Simple Cybot program that is an Echo'ish program for //
//              communicating with the Starter Client Socket code.   //
///////////////////////////////////////////////////////////////////////

#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "movement.h"

int main (void) {

    timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called
    // and enables time functions (e.g. timer_waitMillis)
    lcd_init();   // Initialize the the LCD screen.  This also clears the screen.
    cyBot_uart_init();  // Initialize UART

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    char my_data;       // Variable to get bytes from Client
    char command[3];  // Buffer to store command from Client

    // Write to LCD so that we know the program is running
    lcd_printf("Running");

    while(1)
    {

        my_data = cyBot_getByte(); // Get first byte of the command from the Client

        command[0] = my_data;
        command[1] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
        command[2] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

        if (command[0] == 'w') {
            move_forward(sensor_data, 5); //Calibrate TBD

        } else if (command[0] == 's') {
            move_backwards(sensor_data, 5); //Calibrate TBD

        } else if (command[0] == 'a') {
            turn_left(sensor_data, 5); //Calibrate TBD

        } else if (command[0] == 'd') {
            turn_right(sensor_data, 5); //Calibrate TBD

        }

        lcd_printf("Got: %s", command);  // Print received command to the LCD screen

        // Send a response to the Client (Starter Client expects the response to end with \n)
        // In this case I am just sending back the first byte of the command received and a '\n'
        cyBot_sendByte(command[0]);

        // Only send a '\n' if the first byte of the command is not a '\n',
        // to avoid sending back-to-back '\n' to the client
        if(command[0] != '\n')
        {
            cyBot_sendByte('\n');
        }

    }

    return 0;
}
