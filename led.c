//------------------------------------------------------------------------------
//
//  Description: This file contains the LED initiation function
//
//
//  Benjamin Ryle
//  Sept 11
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include "motor_functions.h"
#include <string.h>
#include "macros.h"

void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  GREEN_LED_OFF;
  RED_LED_OFF;
//------------------------------------------------------------------------------
}
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;

void decide_led(void){
    switch(Time_Sequence){
    case change_display:                        //
      if(one_time){
        Init_LEDs();
        lcd_BIG_mid();
        display_changed = changed;
        one_time = rst_time;
      }
      Time_Sequence = rst_time;             //
      break;
    case just_grn_state:                        //
      if(one_time){
        GREEN_LED_ON;            // Change State of LED 5
        one_time = rst_time;
      }
      break;
    case red_state:                         //
      if(one_time){
        RED_LED_ON;            // Change State of LED 4
        GREEN_LED_OFF;           // Change State of LED 5
        one_time = rst_time;
      }
      break;
    case green_state:                         //
      if(one_time){
        lcd_4line();
        GREEN_LED_ON;            // Change State of LED 5
        display_changed = changed;
        one_time = rst_time;
      }
      break;
    case  off_state:                        //
      if(one_time){
        RED_LED_OFF;           // Change State of LED 4
        GREEN_LED_OFF;           // Change State of LED 5
        one_time = rst_time;
      }
      break;                         //
    default: break;
    }
}

