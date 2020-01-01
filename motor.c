//------------------------------------------------------------------------------
//
//  Description: This file what port pins to turn on or off based off of what motor should be on
//
//  Benjamin Ryle
//  Oct 2019
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------

#include  "functions.h"         // lists function names for all pre-compiled files
#include  "msp430.h"            // provides names for every port
#include <string.h>             // string functions from std library
#include "macros.h"             // global definitions file
#include "motors.h"             // motor definitions file
#include "timers.h"             // timer definitions file
#include "ports.h"              // port definition translation file


int MotorTimer;                         // a countdown until motors automatically shut off without commands
volatile int motor_direction_L=forward; // left motors direction
volatile int motor_next_direction_L;    // the next direction of the left motor for when motors are temporarily disabled
volatile int motor_direction_R=forward; // right motors direction
volatile int motor_next_direction_R;    // the next direction of the right motor for when motors are temporarily disabled


//set battery level from 1 to 5. (1 low 5 high)
unsigned int battery_level=0;//normally this would be in a .h, but i figure ill need to define it in startup later on.
int SPEED_L[7][3] = {{slowL1,medL1,fastL1}, {slowL2,medL2,fastL2},{slowL3,medL3,fastL3}, {slowL4,medL4,fastL4}, {slowL5,medL5,fastL5}, {slowL6,medL6,fastL6}, {slowL7,medL7,fastL7}}; 
int SPEED_R[7][3] = {{slowR1,medR1,fastR1},{slowR2,medR2,fastR2}, {slowR3,medR3,fastR3}, {slowR4,medR4,fastR4}, {slowR5,medR5,fastR5}, {slowR6,medR6,fastR6}, {slowR7,medR7,fastR7}};
//battery level 0 attempts to do NOTHING except counteract basic motor differences.
//battery levels 1 through 5 are litterally just there to make different sized circles.
//

///////
//this function turns off the motors, and is often called for the sake of safety before making important changes
//NOTE: this car uses an H bridge circuit for the motors.
// IF THE MOTORS ARE BOTH RUNNING IN FORWARD AND REVERSE, THE CAR WILL CATCH FIRE.
///////
void motors_off(void){//there isn't a check here, because you can always turn off motors
  TB3CCR1 = WHEEL_OFF;
  TB3CCR2 = WHEEL_OFF;
  TB3CCR3 = WHEEL_OFF;
  TB3CCR4 = WHEEL_OFF;
}

////
//this function turns the right motors on
//it has many redundancy checks due to the risk of fire
////
void right_motor_on(int speed){
  if (motor_direction_R==1){
    TB3CCR3 = WHEEL_OFF;
    TB3CCR1 = SPEED_R[battery_level][speed];
  }
  else if (motor_direction_R==0){
    TB3CCR1 = WHEEL_OFF;
    TB3CCR3 = SPEED_R[battery_level][speed];
  }
}

////
//this function turns the left motors on
//it has many redundancy checks due to the risk of fire
////
void left_motor_on(int speed){
  if (motor_direction_L==1){
    TB3CCR4 = WHEEL_OFF;
    TB3CCR2 = SPEED_L[battery_level][speed];
  } else if (motor_direction_L==0){
    TB3CCR2 = WHEEL_OFF;
    TB3CCR4 = SPEED_R[battery_level][speed];
  }
}

//this is simply a higher order function that runs both motor turn ons
void motors_on(int speed) {//make sure im allowed to change the ports a second time
  right_motor_on(speed);
  left_motor_on(speed);
}
//turns off left motor
void left_motor_off(void){
  TB3CCR2 = WHEEL_OFF;
  TB3CCR3 = WHEEL_OFF;
}
//turns off right motor
void right_motor_off(void){
  TB3CCR1 = WHEEL_OFF;
  TB3CCR3 = WHEEL_OFF;
}

//////
//You can not instantly go from powering one side of an H bridge to powering the other
//You need a period of turn off between the two extremes
// Our motors are an h bridge circuit
// This function finds out if we need to turn off the motors, and sets up a timer interrupt to
//      change the direction as soon as it is safe to do so.
//////
void motor_set_directions(int left, int right){
  if (motor_direction_L>3)
    motor_direction_L=3;
  //the below is the logic involved in the proccess broken down into as many steps as possible to make debugging easier
  int given_L_different        = motor_direction_L != left;
  int current_L_not_off        = motor_direction_L != MOTOR_OFF; 
  int given_R_different        = motor_direction_R != right;
  int current_R_not_off        = motor_direction_R != MOTOR_OFF;
  int left_is_changing         = given_L_different & current_L_not_off;
  int right_is_changing        = given_R_different & current_R_not_off;
  int run_motor_change         = left_is_changing | right_is_changing; 
  if (run_motor_change){ //if we didnt do anything, than dont change anything.
    if (left_is_changing)
      left_motor_off();
    if (right_is_changing)
      right_motor_off();
    motor_direction_R=MOTOR_OFF;        //turn off the motors
    motor_direction_L=MOTOR_OFF;        
    motor_next_direction_R=right;       //set the next direction
    motor_next_direction_L=left;
    MotorTimer=1;                       //set our countdown timer (n*25ms+the skew of the clock compared to when this process is done)
  }
}