//------------------------------------------------------------------------------
//
//  Description: This File contains the State Machine that follows the black line.
//               
//  Benjamin Ryle
//  Nov 2019
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

#include "StateMachine.h"       // definitions for the autonomous white line following state machine
#include "macros.h"             // global definitions file
#include "motors.h"             // motor definitions file

//Function Prototypes
void motor_set_directions(int left, int right); // rewrites the direction of the motors in a safe way
void motors_on(int speed);                      // turns on the motors in the set direction
void motors_off(void);                          // turns off motors
void right_motor_on(int speed);                 // turns the right motor on at slow, med, or fast
void left_motor_on(int speed);                  // turns on left motor at slow, med, or fast
void left_motor_off(void);                      // turns off left motor
void right_motor_off(void);                     // turns off right motor
void lcd_4line(void);                           // switches display to run with 4 lines
void Init_ADC(void);                            // initializes analog to digital converter

//Global Variables
extern unsigned int followtheline;              // Whether im running this state machine
extern unsigned int disp_time;                  // Counts up in real time, and displays to LCD screen depending on Menu
extern volatile int State_Timer;                // Counts down to 0 in interrupts, and pulls state_Trigger high when it reaches 0.
extern char State_Trigger;                      // True when State_Timer counts down to 0
extern int State_Timer2;                        // Counts down to 0 in a timer interrupt and pulls State_Trigger2 high when it's done
extern char State_Trigger2;                     // true when State_Timer2 counts down to 0
extern unsigned int battery_level;              // selects between preset PWM values
extern int Left_Color;                          // interpreted left infared detector value (WHITE/BLACK)
extern int Right_Color;                         // interpreted right infared detector value (WHITE/BLACK)

unsigned int state=done;                        // where the state machine is in progress (see StateMachine.h)
char once=TRUE;                                 // ensures that secific events only trigger when entering a state in the state machine.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// State Machine for autonomously following a black line
//
// In order, the states are:
// pause, find_white, find_line, rev_pulse_line, start_face_line, end_face_line, rev_pulse, BL_Travel_pause, follow_circle, white_timer, exit_pause, turn_right, to_center
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateMachine(void){
  switch(state){
    
    //placing the car down and having it instantly drive is inconvennient
    // this state will wait 2 seconds before starting searching.
  case pause:
    if (once){
      lcd_4line();
      State_Timer=two_sec;
      once=FALSE;
      disp_time=rst_time;
      Init_ADC();
    }
    else if (State_Trigger==TRUE){
      state=Find_White;
      State_Trigger=FALSE;
      once=TRUE;
    } 
    break;
    
    //the car needs to now find the white background that it will be following
    // it does this via a timer
  case Find_White:
    motor_set_directions (forward, forward);
    motors_on(med);
    if (once){
      State_Timer=find_white;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state=find_line;
      State_Trigger=FALSE;
      once=TRUE;
    }    
    break;
    
    //the car will now use its 2 infared detectors to find where the black line starts
  case find_line:
    motor_set_directions(forward, forward);
    right_motor_on(med);
    left_motor_on(med);
    once=TRUE;
    if((Left_Color==BLACK) & (Right_Color==BLACK)){
      motors_off();
      state=rev_pulse_line;
    }
    break;
    
    //in order to not pass the line after detection, we pulse the motors in reverse on a short timer
  case rev_pulse_line:
    motor_set_directions(reverse, reverse);
    motors_on(fast);
    if (once){
      State_Timer=rev_pulse_time;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state=start_face_line;
      State_Trigger=FALSE;
      once=TRUE;
    }
    break;
    
    //we now start turning to face the line, starting a timer where we ignore the black under us.
  case start_face_line:
    motor_set_directions( reverse, forward);
    motors_on(fast);
    if (once){
      State_Timer=face_delay;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state++;
      State_Trigger=FALSE;
      once=TRUE;
    }
    break;
    
    //at the end of this timer, we stop the next time we see the black line
  case end_face_line: 
    motor_set_directions( reverse, forward);
    motors_on(fast);
    once=1;
    if ((Left_Color==BLACK) & (Right_Color==BLACK)){
      motors_off();
      state++;
    }
    break;
    
    //another reverse pulse, this time to stop the turning momentum of the car
  case rev_pulse:
    motor_set_directions(forward, reverse);
    motors_on(fast);
    if (once){
      State_Timer=rev_pulse_time;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state=BL_Travel_pause;
      State_Trigger=FALSE;
      once=TRUE;
    }
    break;
    
    //we now stop to show that we have detected and intercepted the black line successfully.
  case BL_Travel_pause:
    motors_off();
    if (once){
      State_Timer=two_sec;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state=follow_circle;
      State_Trigger=FALSE;
      once=TRUE;
    }
    break;
    
    /////
    // This is the bulk of the statemachine
    // it goes forward, turns right, turns left, or reverses
    //    depending on the position of the car
    ////
  case follow_circle:
    battery_level=6;
    if (Left_Color==WHITE){
      if (Right_Color==WHITE){//reverse if they areboth white
        motor_set_directions(reverse, reverse);
        right_motor_on(fast);
        left_motor_on(fast); //with directionality
        state=white_timer;
      }      
      else{ 
        motor_set_directions(forward, reverse);
        motors_off();
        left_motor_on(med);
        right_motor_on(fast);       
      }}
    else if (Right_Color==WHITE){
      motor_set_directions(reverse, forward);
      motors_off();
      left_motor_on(fast);
      right_motor_on(med);
    }  
    else{
      motor_set_directions(forward, forward);
      motors_on(med); //go forward if both black
    }
    break;
    
    //in order to prevent the car from sticking on the line and stopping dead between 2 states
    // detecting white reverses on a timer instead of until it detects black.
  case white_timer:
    //motor_set_directions(reverse, reverse);
    right_motor_on(fast);
    left_motor_on(med); //with directionality
    if (once){
      State_Timer=rev_delay;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state=follow_circle;
      State_Trigger=FALSE;
      once=TRUE;
    }
    break;
    
    
    //after the user gives a command, we can stop driving to show we are going to exit the line
  case exit_pause:
    motors_on(fast);
    if (once){
      State_Timer=two_sec;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state++;
      State_Trigger=FALSE;
      once=TRUE;
    }
    break;
    
    // we turn right on a timer
  case turn_right:
    motor_set_directions( forward, reverse);
    motors_on(slow);
    if (once){
      State_Timer=right_turn_time;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state++;
      State_Trigger=FALSE;
      once=TRUE;
    } 
    break;
    
    //we then drive towards the center of the test circle i judged this off of.
  case to_center:
    motor_set_directions( forward, forward);
    motors_on(fast);
    if (once){
      State_Timer=two_sec;
      once=FALSE;
    }
    else if (State_Trigger==TRUE){
      state++;
      State_Trigger=FALSE;
      once=TRUE;
    } 
    break;
    
    //idle state.
  case done:
    followtheline=FALSE;
    motors_off();
    break;
  }
}