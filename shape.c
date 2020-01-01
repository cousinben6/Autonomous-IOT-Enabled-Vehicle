
//------------------------------------------------------------------------------
//
//  Description: This file decides a motor angle based off of a handful of preset shapes
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
extern char triangle_part;

//motor control global variables
extern volatile unsigned int motor_counter;
extern char motor_direction_R; //0 is backwards 1 is forwards
extern char motor_direction_L;
extern char angle; // value 0 to 6 and 7
//0 is hard left 6 is hard right, 3 is even power to both motors; 
//7 is off.
void motor_set_directions(int left, int right);
void motor_set_direction_R(int dir);
void motor_set_direction_L(int dir);
void motors_on(int speed);
void motors_off(void);
void right_motor_off(void);
void right_motor_on(int speed);
void left_motor_on(int speed);
void left_motor_off(void);
void motor_change_direction_L(void);
void motor_change_direction_R(void);
extern char display_line[lcd_row][lcd_col];
extern char *display[lcd_disp];

//this function finds out which shape is enabled, and assigns an angle to drive in accordingly
void decide_shape(void){
  switch (shape){
  case done:
    angle= off;
    strcpy(display_line[LCD_BOT], " put down ");
    update_string(display_line[LCD_INST], LCD_INST);
    break;
    
  case forward0:
    angle= Straight;
    strcpy(display_line[LCD_BOT], "  forward ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=FULL_SPEED;
    LEFT_FORWARD_SPEED=FULL_SPEED;
    break;
    
  case pause0:
    strcpy(display_line[LCD_BOT], "   pause   ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=WHEEL_OFF;
    LEFT_FORWARD_SPEED=WHEEL_OFF;    
    RIGHT_REVERSE_SPEED=WHEEL_OFF;
    LEFT_REVERSE_SPEED=WHEEL_OFF;  
    break;
    
  case reverse0:
    strcpy(display_line[LCD_BOT], " reverse ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_REVERSE_SPEED=FULL_SPEED;
    LEFT_REVERSE_SPEED=FULL_SPEED;  
        break;
    
  case reverse1:
    strcpy(display_line[LCD_BOT], " reverse ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_REVERSE_SPEED=FULL_SPEED;
    LEFT_REVERSE_SPEED=FULL_SPEED; 
    break;
    
  case pause1:
    strcpy(display_line[LCD_BOT], "   pause   ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=WHEEL_OFF;
    LEFT_FORWARD_SPEED=WHEEL_OFF;  
    RIGHT_REVERSE_SPEED=WHEEL_OFF;
    LEFT_REVERSE_SPEED=WHEEL_OFF;  
    break;
    
  case forward1:
    strcpy(display_line[LCD_BOT], "  forward ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=FULL_SPEED;
    LEFT_FORWARD_SPEED=FULL_SPEED;
    break;
    
  case pause2:
    strcpy(display_line[LCD_BOT], "   pause   ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=WHEEL_OFF;
    LEFT_FORWARD_SPEED=WHEEL_OFF;  
        RIGHT_REVERSE_SPEED=WHEEL_OFF;
    LEFT_REVERSE_SPEED=WHEEL_OFF;  
    break;
    
  case clock0:
    strcpy(display_line[LCD_BOT], "clockwise ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=FULL_SPEED;
    LEFT_FORWARD_SPEED=WHEEL_OFF;  
    RIGHT_REVERSE_SPEED=WHEEL_OFF;
    LEFT_REVERSE_SPEED=FULL_SPEED;  
    break;
    
  case clock1:
    strcpy(display_line[LCD_BOT], "clockwise ");
    update_string(display_line[LCD_INST], LCD_INST);
    break;
    
  case clock2:
    strcpy(display_line[LCD_BOT], "clockwise ");
    update_string(display_line[LCD_INST], LCD_INST);
    break;
    
  case pause3:
    strcpy(display_line[LCD_BOT], "   pause   ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=WHEEL_OFF;
    LEFT_FORWARD_SPEED=WHEEL_OFF;  
        RIGHT_REVERSE_SPEED=WHEEL_OFF;
    LEFT_REVERSE_SPEED=WHEEL_OFF;  
    break;
    
  case pause4:
    strcpy(display_line[LCD_BOT], "   pause   ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=WHEEL_OFF;
    LEFT_FORWARD_SPEED=WHEEL_OFF;  
        RIGHT_REVERSE_SPEED=WHEEL_OFF;
    LEFT_REVERSE_SPEED=WHEEL_OFF;  
    break;
    
  case counterclock0:
    strcpy(display_line[LCD_BOT], "countclock");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=WHEEL_OFF;
    LEFT_FORWARD_SPEED=FULL_SPEED;  
    RIGHT_REVERSE_SPEED=FULL_SPEED;
    LEFT_REVERSE_SPEED=WHEEL_OFF;  
    break;
    
  case counterclock1:
    strcpy(display_line[LCD_BOT], "countclock");
    update_string(display_line[LCD_INST], LCD_INST);  
    break;
    
  case counterclock2:
    angle= Straight;
    strcpy(display_line[LCD_BOT], "countclock");
    update_string(display_line[LCD_INST], LCD_INST);
    break;
    
  case pause5:
    strcpy(display_line[LCD_BOT], "   pause   ");
    update_string(display_line[LCD_INST], LCD_INST);
    RIGHT_FORWARD_SPEED=WHEEL_OFF;
    LEFT_FORWARD_SPEED=WHEEL_OFF;  
        RIGHT_REVERSE_SPEED=WHEEL_OFF;
    LEFT_REVERSE_SPEED=WHEEL_OFF;  
    break;
  }
}