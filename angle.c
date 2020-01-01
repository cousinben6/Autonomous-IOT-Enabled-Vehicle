
//------------------------------------------------------------------------------
//
//  Description: This file decides which combination of motor functions to call for each angle
//
//  Benjamin Ryle
//  Oct 2019
//
//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "motor_functions.h"
#include <string.h>
#include "macros.h"
#include "ports.h"
#include "motors.h"
#include "timers.h"
//
extern char chosen_direction;
extern char shape;
extern char previous_button;
extern unsigned long shape_timer;
extern char shape_progress;

//motor control global variables
extern volatile unsigned int motor_counter;
extern char motor_direction; //0 is backwards 1 is forwards

extern char angle; // value 0 to 6 and 7
//0 is hard left 6 is hard right, 3 is even power to both motors; 
//7 is off.

//motor controls:
//0 off 1 on 2 reverse
void motors_on(int);
void motors_off(void);
void right_motor_off(void);
void right_motor_on(int);
void left_motor_on(int);
void left_motor_off(void);


//------------------------------------------------------------------------------
//this function translates a given angle value into a motor.c function
//this function DOES NOT handle forward to reverse switching
//this function DOES NOT understand directionality in any way. this is handed off to motors.c
//this function DOES NOT check if motors are safe to use.
//-----------------------------------------------------------------------------
void decide_angle(void){
    switch (angle){
    case off:
      motors_off();
      break;
      
    case U_L:
      right_motor_on(FULL_SPEED);
      left_motor_off();        
      break;
    
    case Hard_L:
      right_motor_on(FULL_SPEED);
      left_motor_on(FITH_SPEED);
      break;
    
    case Soft_L:
      right_motor_on(FULL_SPEED);
      left_motor_on(FOUR_FITHS_SPEED);
      break;
    
    case Straight:
      motors_on(FULL_SPEED);
      break;
    
    case Soft_R:
      left_motor_on(FULL_SPEED);
      right_motor_on(FOUR_FITHS_SPEED);
      break;
    
    case Hard_R:
      left_motor_on(FULL_SPEED);
      right_motor_on(FITH_SPEED);
      break;
    
    case U_R:
      right_motor_off();
      left_motor_on(FULL_SPEED);
      break;
    }
	}