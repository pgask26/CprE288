/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){

    // YOUR CODE HERE
    //Timer 3B (GPTM 3, Timer B) on PB3 (Port B, Pin 3)
    SYSCTL_RCGCTIMER_R |= 0b1000; //Timer module 3 is ENABLED
    SYSCTL_RCGCGPIO_R = 0b10; //GPIO Port B is ENABLED
    GPIO_PORTB_PCTL_R |= 0x7000; //Pin 3 is ENABLED

    GPIO_PORTB_AFSEL_R |= 0x8; // alternate function for pin 3
    GPIO_PORTB_PCTL_R |= 0x7000; // choosing the alternate function to be timer 3b CCP
    GPIO_PORTB_DEN_R |= 0x8; // enabling pin 3
    GPIO_PORTB_DIR_R |= 0x8; // setting pin 3 as output

    // Configure and enable the timer

    //Input Edge Time Mode
    //GPTMCTL, clear TBEN bit
    TIMER3_CTL_R &= ~0x100;
    //GPTMCFG, register with a value of 0x0000.0004
    TIMER3_CFG_R |= 0x00000004; // splits timer3 into two 16 bit timers, we will use timer b and a prescalar
    //GPTMTBMR, TBCMR field to 0x1 and TBMR field to 0x3 and select a count direction by programming the TnCDIR bit
    TIMER3_TBMR_R |= 0x7; // set timer 3b to capture mode and edge time mode
    TIMER3_TBMR_R &= ~0x10; // sets count direction to down, check what to set (TAAMS)
    //GPTMCTL, Configure the type of event that the timer captures by writing the TnEVENT field
    //TIMER3_CTL_R
    //GPTMTnPR, If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register
    //TIMER3_TBPR_R
    //GPTMTnILR, Load the timer start value into the GPTM Timer n Interval Load
    //TIMER3_TBILR_R
    //GPTMIMR, If interrupts are required, set the CnEIM bit
    //TIMER3_IMR_R
    //GPTMCTL, Set the TnEN bit to enable the timer and start counting.
    //TIMER3_CTL_R
    //GPTMRIS, See TIVA sheet

    TIMER3_CTL_R |= 0xD00; // setting bits 11, 10 (trigger interrupt for both edges), and 8 (enable timer b)

    /*
    TIMER3_TBILR_R |= 0xFFFF; // sets value of 16 bit timer to full
    TIMER3_TBPR_R |= 0xFF; // sets value of 8 bit pre-scalar to full
    TIMER3_IMR_R |= 0x400;
     */


    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();
}

void ping_trigger (void){
    STATE = LOW;
    TIMER3_CTL_R &= ~0x100; //disable timer
    TIMER3_IMR_R &= ~0x400; //disable timer interrupt
    GPIO_PORTB_AFSEL_R &= ~0x8; //disables pin 3 alternate function

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    // set the PB3 pin 3 data to 1 to start pulse
    GPIO_PORTB_DATA_R &= ~0x8;
    timer_waitMicros(2);
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(2); //2 microsecond pulse, can be 2-5
    GPIO_PORTB_DATA_R &= ~0x8;

    TIMER3_ICR_R = 0x400;// Clear an interrupt that may have been erroneously triggered

    GPIO_PORTB_AFSEL_R |= 0x8; // Re-enable alternate function, timer interrupt, and timer
    TIMER3_IMR_R |= 0x400; // Re-enable timer interrupt
    TIMER3_CTL_R |= 0xD00; // Re-enable timer

}

void TIMER3B_Handler(void){

    // YOUR CODE HERE
    // As needed, go back to review your interrupt handler code for the UART lab.
    // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
    // includes checking the source of the interrupt and clearing the interrupt status bit.

    // Checking the source: test the MIS bit in the MIS register (is the ISR executing
    // because the input capture event happened and interrupts were enabled for that event?

    // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
    // The rest of the code in the ISR depends on actions needed when the event happens.

    // interrupt means that either the rising or falling edge has been detected, use STATE variable
    // to determine which it is

    TIMER3_ICR_R = 0x400;

    char byte_received;
    //check if handler called due to GPTM Timer B Capture Mode Event Masked Interrupt event
    if (TIMER3_MIS_R & 0x400)
    {
        //byte was received in the TIMER3 data register
        //clear the GPTM Timer B Capture Mode Event Masked Interrupt flag (clear by writing 1 to ICR)
        TIMER3_ICR_R |= 0b010000000000;

        //if rising edge

        //if falling edge
    }
}

float ping_getDistance (void){

    // YOUR CODE HERE
    //speed of sound is 343m/s, sound needs to do a round trip to get back to the sonar sensor

    return 0;
}
