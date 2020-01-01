////////////////////////////////////////////////////////////////////////////
//Description: Contains the uart state machine
//  Benjamin Ryle
//  Nov 2019
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//      
////////////////////////////////////////////////////////////////////////////

//include files
#include  "functions.h"            // lists function names for all pre-compiled files
#include  "msp430.h"               // provides names for every port
#include  <string.h>               // string functions from std library
#include  "macros.h"               // global definitions file
#include  "motors.h"               // motor definitions file
#include  "ports.h"                // port definition translation file
#include  "StateMachine.h"         // definitions for the autonomous white line following state machine
// global variables

extern char LCD_LINE1 [LCD_LEN];                // String outputting to line 1 of the lcd
extern char LCD_LINE2 [LCD_LEN];                // String outputting to line 2 of the lcd
extern char LCD_LINE3 [LCD_LEN];                // String outputting to line 3 of the lcd
extern char LCD_LINE4 [LCD_LEN];                // String outputting to line 4 of the lcd

//usb ring buffer
extern volatile char USB_Char_Rx[SMALL_RING_SIZE] ;    // usb  receive  ring buffer array
extern volatile unsigned int usb_rx_ring_wr;           // usb  receive  ring buffer write position
extern unsigned int usb_rx_ring_rd;                    // usb  recieve  ring buffer read  position
extern volatile char USB_Char_Tx[LARGE_RING_SIZE] ;    // usb  transmit ring buffer array
extern volatile unsigned int usb_tx_ring_wr;           // usb  transmit ring buffer write position
extern unsigned int usb_tx_ring_rd;                    // usb  transmit ring buffer read  position

//wifi ring buffer
extern volatile char USB_Char_Rx1[SMALL_RING_SIZE] ;   // wifi receive  ring buffer array
extern volatile unsigned int usb_rx_ring_wr1;          // wifi recieve  ring buffer write position
extern unsigned int usb_rx_ring_rd1;                   // wifi recieve  ring buffer read  position
extern volatile char USB_Char_Tx1[LARGE_RING_SIZE] ;   // wifi transmit ring buffer array
extern volatile unsigned int usb_tx_ring_wr1;          // wifi transmit ring buffer write position
extern unsigned int usb_tx_ring_rd1;                   // wifi transmit ring buffer read  position

//function calls
void out_string(void);                                  // outputs the contents of the circular buffer to UCA0
void motor_set_directions(int left, int right);         // safely sets the direction of both motors
void motors_on(int speed);                              // turns on the motors when it's next safe to do so
void right_motor_on(int speed);                         // turns on the right motor when it's safe to do so
void left_motor_on(int speed);                          // turns on the left  motor when it's safe to do so
void motors_off(void);                                  // turns off all motors

extern unsigned int disp_time;                          // count of how many seconds have passed to print a timer to the lcd display
extern unsigned int state;                              // state of autonomous line following state machine
extern unsigned int battery_level;                      // pwm value of line following state machine
extern volatile int cmd_state;                          // whether there is an IOT or rc car command to process or not
extern unsigned int followtheline;                      // whether we are following the line or not


char command_string [lcd_col] = "          ";           // stores commmands respective string to be printed by menu state machine
char comand [comand_len];                               // stores commmands recieved from wifi module
int Motor_timer=timer_disabled;                         // auto motor cutoff on timer
volatile unsigned int pos_count=0;                      // what part of the obstacale course the car is at.

void uart_statemachine(void){
  if (cmd_state==process_iot_command){
    lcd_4line();
    while (usb_rx_ring_wr1!=usb_rx_ring_rd1){
      char temp=USB_Char_Rx1[usb_rx_ring_rd1];
      USB_Char_Tx[usb_tx_ring_wr]=temp;
      usb_rx_ring_rd1++;
      usb_tx_ring_wr++;
      if (usb_tx_ring_wr>LARGE_RING_SIZE)
        usb_tx_ring_wr=BEGINNING;
      if (usb_rx_ring_rd1>SMALL_RING_SIZE)
        usb_tx_ring_rd1=BEGINNING;
      cmd_state=hold;
    }
    out_string();
  }
  //void motor_set_directions(int left, int right);
  //void motors_on(int speed);
  //void motors_off(void);
  extern int lcd_state;
  if (cmd_state==process_fram_command){
    lcd_state=show_command;
    lcd_BIG_mid();
    //ensure they have the right key
    cmd_state=hold;
    
    switch (comand[parse_direction]){
    case 'L':   //turn left
      Motor_timer= time_constant*(comand[parse_time]-'0');
      motor_set_directions(reverse, forward);
      motors_on(med);
      
      command_string[2]='L';
      command_string[3]='E';
      command_string[4]='F';
      command_string[5]='T';
      command_string[6]=' ';
      command_string[7]=' ';
      command_string[8]=' ';
      right_motor_on(med);
      left_motor_on(med);
      break;
    case 'R':           // turn right
      Motor_timer=time_constant*(comand[parse_time]-'0');
      motor_set_directions(forward, reverse);
      motors_on(med);
      
      command_string[2]='R';
      command_string[3]='I';
      command_string[4]='G';
      command_string[5]='H';
      command_string[6]='T';
      command_string[7]=' ';
      command_string[8]=' ';
      right_motor_on(med);
      left_motor_on(med);
      break;
    case 'B':   //go backwards
      Motor_timer= time_constant*(comand[parse_time]-'0');
      motor_set_directions(reverse, reverse);
      motors_on(med);
      
      command_string[2]='B';
      command_string[3]='A';
      command_string[4]='C';
      command_string[5]='K';
      command_string[6]=' ';
      command_string[7]=' ';
      command_string[8]=' ';
      right_motor_on(fast);
      left_motor_on(med);
      
      break;
    case 'F': //go forwards
      Motor_timer= time_constant*(comand[parse_time]-'0');
      motor_set_directions(forward, forward);
      motors_on(fast);
      
      command_string[2]='F';
      command_string[3]='O';
      command_string[4]='R';
      command_string[5]='W';
      command_string[6]='A';
      command_string[7]='R';
      command_string[8]='D';
      motors_on(fast);
      break;
    case 'H': //HALT
      motors_off();
      Motor_timer= -1;
      command_string[2]='W';
      command_string[3]='A';
      command_string[4]='I';
      command_string[5]='T';
      command_string[6]='I';
      command_string[7]='N';
      command_string[8]='G';
      break;
    case 'S':   // find line angle #1
      lcd_state=BL_lcd;
      followtheline=TRUE;
      battery_level=1;
      state=pause;
      break;
    case 'T':// find line angle #2
      lcd_state=BL_lcd;
      followtheline=TRUE;
      battery_level=2;
      state=pause;
      break;
    case 'U'://find line angle #3
      lcd_state=BL_lcd;
      followtheline=TRUE;
      battery_level=3;
      state=pause;
      break;
    case 'V'://find line angle #4
      lcd_state=BL_lcd;
      followtheline=TRUE;
      battery_level=4;
      state=pause;
      break;
    case 'W'://find line angle #5
      lcd_state=BL_lcd;
      followtheline=TRUE;
      battery_level=5;
      state=pause;
      break;
    case 'X':   //Xit (exit) the circle
      lcd_state=BL_lcd;
      state=turn_right;
      break;
      
    case '0': // reset timer on display
      disp_time=FALSE;
      break;
      
    case 'A': // increments counter showing what part of the obstacle course we're on
      pos_count++;
      break;
      
    case 'Y': // starts leaving the circle
      state =turn_right;
      break;
    }
  }
}
