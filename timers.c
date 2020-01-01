//------------------------------------------------------------------------------
//
//  Description: This file defines how to initiate the timer B0
//                      
//
//  Benjamin Ryle
//  Sept 11
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------

//include files
#include  "functions.h"           // lists function names for all pre-compiled files         
#include  "msp430.h"              // provides names for every port
#include "motor_functions.h"      // functions to interact with motors
#include <string.h>               // string functions from std library
#include "macros.h"               // global definitions file
#include "ports.h"                // port definitions
#include "timers.h"               // numbers used in instantiating timers
#include "motors.h"               // motor definitions file

volatile extern char motor_direction_R;         // the direction the right motor is spinning
volatile extern char motor_direction_L;         // the direction the left motor is spinning                              
extern volatile unsigned int Time_Sequence;
extern int sw1_debounce;                        // whether the left button can be pressed
extern int sw2_debounce;                        // whether the right button can be pressed                      
//#define CCIE (0X10)

//timer b0 initialization here sets up b0_0, b0_1-b0_2 and overflow
void Init_Timer_B0(void){
  TB0CTL=TBSSEL__SMCLK;		//smclk source
  TB0CTL |= TBCLR;		// resets tb0r, clk divider, count direction
  TB0CTL |= MC__CONTINUTOUS;	// continuous up
  TB0CTL |=ID_2; 			//divide clock by 8
  
  TB0EX0 = TBIDEX_7; 		//Divide clock by an aditional 8
  
  TB0CCR0 = TB0CCR0_INTERVAL;	// CCR0
  TB0CCTL0 |= CCIE;		// CCR0 enable interrupt
  
  TB0CCR1 = TB0CCR1_INTERVAL;	// CCR1
  TB0CCTL1 |= CCIE;		// CCR1 enable interrupt
  
  TB0CCR2 = TB0CCR2_INTERVAL;	// CCR2
  TB0CCTL2 |= CCIE;		// CCR2 enable interrupt
  
  TB0CTL &= ~TBIE;		// Disable overflow interrupt
  TB0CTL &= ~TBIFG;		//Clear Overflow interrupt flag
}

void Init_Timer_B3(void){
  TB3CTL = TBSSEL__SMCLK;
  TB3CTL |= MC__UP;
  TB3CTL |= TBCLR;
  
  TB3CCR0 = WHEEL_PERIOD;
  
  TB3CCTL1 = OUTMOD_7;
  RIGHT_FORWARD_SPEED = WHEEL_OFF;
  
  TB3CCTL2 = OUTMOD_7;
  LEFT_FORWARD_SPEED = WHEEL_OFF;
  
  TB3CCTL3 = OUTMOD_7;
  RIGHT_REVERSE_SPEED = WHEEL_OFF;
  
  TB3CCTL4 = OUTMOD_7;
  LEFT_REVERSE_SPEED = WHEEL_OFF;
}

/////////////////////////////////////////////////////
//Initializes All required default values for timers
//              Runs All Timer port initializations
////////////////////////////////////////////////////
void Init_Timers(void){
  Time_Sequence=reset;
  sw1_debounce=reset;
  sw2_debounce=reset;
  //one_time=reset;
  Init_Timer_B0();
  Init_Timer_B3();
}
