//-----------------------------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Benjamin Ryle
//  Mar 2019
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//-----------------------------------------------------------------------------------------------------

#include  "functions.h"            // lists function names for all pre-compiled files
#include  "msp430.h"               // provides names for every port
#include  <string.h>               // string functions from std library
#include  "macros.h"               // global definitions file
#include  "motors.h"               // motor definitions file

// Function Prototypes
void Menu(void);                   // State machine to process what goes in each lcd line
void LCD_Write(void);              // Refreshes LCD screen
void StateMachine(void);           // Autonomous Line following state machine
void uart_statemachine(void);      // turn uart commands passed from ISR into variable changes
void Init_Serial_UCA0(void);       // Sets up ports to run 
void Init_Serial_UCA1(void);       // Sets up ports to run USB
void Init_Display(void);           // Sets up ports to run display

// Global Variables
unsigned int followtheline=0;      // don't run the line follow state machine by default

void main(void){
  //-----------------------------------------------------------------------------------------------------
  // Main Program
  // This is the main routine for the program. Execution of code starts here.
  // The operating system is Back Ground Fore Ground.
  //
  //-----------------------------------------------------------------------------------------------------
  
PM5CTL0 &= ~LOCKLPM5;
  Init_Ports();                   // Initialize Ports
  Init_Clocks();                  // Initialize Clock System
  Init_Conditions();              // Initialize Variables and Initial Conditions
  Init_Timers();                  // Initialize Timers
  Init_LCD();                     // Initialize LCD
  Init_Serial_UCA0();             // Initialize Wifi module Serial
  Init_Serial_UCA1();             // Initialize USB Serial
  Init_Display();                 // Initialize LCD Menu abstraction variables
  
  //-----------------------------------------------------------------------------------------------------
  // Begining of the "While" Operating System
  //-----------------------------------------------------------------------------------------------------
  while(ALWAYS) {
    if (followtheline)           //if i'm currently following the line...
      StateMachine();            //run the autonomous line following state machine.
    
    uart_statemachine();         //look for and process any incoming or outgoing UART communications
    Menu();                      //refresh all menu aspects
    LCD_Write();                 //write to the LCD Screen.
  }
  
}