/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping_template.h"

// Uncomment or add any include directives that are needed

int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	ping_init();

	// YOUR CODE HERE

	while(1)
	{
      // YOUR CODE HERE
	  ping_trigger(); //trigger ping

	}

}
