//------------------------------------------------------------------------------
//
//  Description: This file contains the display initation, and general
//                      initiations needed before beginning the core logic loop
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


  // Global Variables
extern volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern unsigned int test_value;
extern char chosen_direction;
extern char change;

//------------------------------------------------------------------------------
// this function initializes the displays update, update count, mode, and line flags.
//there are no alternative options, but it may need to be fixed later on if instantiation of the extern variables needs to exist elsewhere.
//------------------------------------------------------------------------------
void Init_Conditions(void){
//------------------------------------------------------------------------------
  int i;

  for(i=0;i<11;i++){
    display_line[0][i] = RESET_STATE;
    display_line[1][i] = RESET_STATE;
    display_line[2][i] = RESET_STATE;
    display_line[3][i] = RESET_STATE;
  }
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  display[0] = &display_line[0][0];
  display[1] = &display_line[1][0];
  display[2] = &display_line[2][0];
  display[3] = &display_line[3][0];
  update_display = 0;
  update_display_count = 0;
  
// Interrupts are disabled by default, enable them.
  enable_interrupts();
//------------------------------------------------------------------------------
}
