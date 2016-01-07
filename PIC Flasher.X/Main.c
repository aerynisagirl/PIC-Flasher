/*******************************************************************************************************
 *  PIC Flasher - A pointless flashy blinky thingy which has no purpose but to experiment with delays  *
 *  Created by mikemadealarms on January 7, 2016 at 4:57 PM                                            *
 * --------------------------------------------------------------------------------------------------- *
 *  Last modified by mikemadealarms on January 7, 2016 at 5:56 PM                                      *
 *  Last modification made was: Finish up code and run final test                                      *
 *******************************************************************************************************/

#include <xc.h>

/***********************
 *  MCU Configuration  *
 ***********************/

//CONFIG1 Register
#pragma config FOSC = INTOSC   //Configure the MCU to use the internal oscillator
#pragma config WDTE = OFF      //Disable the Watchdog Timer as it is not needed
#pragma config PWRTE = OFF     //Disable the Power-up Timer as it is not needed
#pragma config MCLRE = ON      //Enable the use of the MCLRE function on Pin 4 of the MCU
#pragma config CP = OFF        //Disable Code Protection as it is completely unecessary
#pragma config CPD = OFF       //Disable Data Code Protection as it is completely unecessary
#pragma config BOREN = ON      //Enable the Brown-out Reset function
#pragma config CLKOUTEN = OFF  //Disable the CLKOUT function on Pin 3 of the MCU
#pragma config IESO = ON       //Allow the use of Internal/External Clock Switch Over feature
#pragma config FCMEN = ON      //Enable the Fail-Safe Clock monitor

//CONFIG2 Register
#pragma config WRT = OFF     //Disable the Flash-memory Re-Write Protection feature as it is completely unecessary
#pragma config PLLEN = ON    //Enable the 4xPLL function on the internal oscillator to so the oscillator runs at 32MHz instead of 8MHz
#pragma config STVREN = OFF  //Disable the Stack Overflow Reset function as it is not needed
#pragma config BORV = LO     //Set the voltage at which the Brown-out Reset function resets the MCU at to (I don't know as of right now, im assuming its the low voltage setting)
#pragma config LVP = OFF     //Disable the Low Voltage Program function linked to the Pin that doesn't exist on the MCU

/***************************
 *  Flasher Configuration  *
 ***************************/

//Flasher Configuration
#define DELAY_MINIMUM 250   //The minimum speed at which the flasher will flash the output at
#define DELAY_MAXIMUM 2000  //The maximum speed at which the flasher will flash the output at
#define DELAY_INCREMENT 10  //The amount of time that the flasher will increase or decrease the flashing speed of each cycle

/***************
 *  Variables  *
 ***************/

//Flasher Variables
unsigned int flasherSpeed;  //The current speed of the cycle that is being flashed
int flasherIncrement;       //The increment value that determines how big the speed increase will be and which direction it will go

/***********
 *  Setup  *
 ***********/

//Setup Function, configures MCU modules, initializes variables, and sets IO registers
void setup() {
    //Timing Related Registers
    OSCCON = 0x70;  //Turn on the internal oscillator and set it to run at 8MHz with PLL enabled making it 32MHz
    
    //IO Related Registers
    TRISA = 0x0F;  //Set PORTA4 and PORTA5 to outputs and set the rest to inputs
    
    //Initialize Variables
    flasherSpeed = DELAY_MINIMUM;        //Set the speed of the flasher to begin at the shortest delay the flasher can run at set in the configuration settings
    flasherIncrement = DELAY_INCREMENT;  //Set the increment variable so that the speed of the flasher will increment at the speed set in the configuration settings
}

/****************
 *  Processing  *
 ****************/

//Pause Function, pauses the program for a specified amount of time in milliseconds
void pause(unsigned long delay) {
    //Loop for the amount of clock cycles required to achieve the specified delay
    for (unsigned long cycles = 0; cycles < 32000000 / (8000000 / delay); cycles++);
}

//Main Function, called upon reset of the MCU
void main() {
    setup();  //Setup MCU by configuring MCU modules, initializing variables, and setting IO registers
    
    //Run in a continuous loop to prevent the MCU from going into sleep mode
    while (1) {
        //Flash the output of the flasher
        PORTA = 0x30;         //Set PORTA4 and PORTA5 to logic HIGH
        pause(flasherSpeed);  //Wait for the current delay of the flasher and then move on
        PORTA = 0x00;         //Set PORTA4 and PORTA5 to logic LOW
        pause(flasherSpeed);  //Wait for the current delay of the flasher and then move on
        
        //Update the speed of the flasher
        flasherSpeed += flasherIncrement;  //Increment the speed by the value of the increment variable
        
        //Check to see if the delay has reached the limit
        if (flasherSpeed == DELAY_MINIMUM) {
            flasherIncrement += DELAY_INCREMENT * 2;  //Set the increment variable to be the negative equivalent to the increment value in the configuration settings
        } else if (flasherSpeed == DELAY_MAXIMUM) {
            flasherIncrement -= DELAY_INCREMENT * 2;  //Set the increment variable to be equal to the increment value in the configuration settings
        }
    }
}
