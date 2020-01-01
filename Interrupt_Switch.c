//------------------------------------------------------------------------------
//
//  Description: This file contains interrupts programmed for both buttons
//
//  Benjamin Ryle
//  Sept 11
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------
#include "StateMachine.h"       // definitions for the autonomous white line following state machine
#include  "functions.h"         // lists function names for all pre-compiled files
#include  "msp430.h"            // provides names for every port
#include <string.h>             // string functions from std library
#include "macros.h"             // global definitions file
#include "motors.h"             // motor definitions file
#include "timers.h"             // timer definitions file
#include "ports.h"              // port definition translation file

//extern volatile int lcd_state;
extern unsigned int followtheline;                     // whether the vehicle is following the white line
int sw1_debounce=disabled;                             // whether the left  button will accept a second input
int sw2_debounce=disabled;                             // whether the right button will accept a second input
int sw2_pressed;                                       // whether the right button is pressed
int sw1_pressed;                                       // whether the left  button is pressed

extern unsigned int state;                             // state of line follow state machine


//usb ring buffer
extern volatile char USB_Char_Rx[SMALL_RING_SIZE] ;    // usb  receive  ring buffer array
extern volatile unsigned int usb_rx_ring_wr;           // usb  receive  ring buffer write position
extern unsigned int usb_rx_ring_rd;                    // usb  recieve  ring buffer read  position
extern volatile char USB_Char_Tx[LARGE_RING_SIZE] ;    // usb  transmit ring buffer array
extern volatile unsigned int usb_tx_ring_wr;           // usb  transmit ring buffer write position
extern unsigned int usb_tx_ring_rd;                    // usb  transmit ring buffer read  position

void motors_off(void);                                 // turns off motors

//switch 2 interrupt (button on the right relative to LCD screen text)
// current functionality:
// if following the line, return to idle state of line following.
#pragma vector=PORT2_VECTOR
__interrupt void switch_interrupt(void) {
  sw2_pressed=P2IFG &SW2;               
  if (sw2_pressed){                     
    P2IFG &= !SW2;                      // disable interrupt flag
    if (sw2_debounce==disabled){                
      sw2_debounce=debounce_delay;      // set debounce delay
      
      if (followtheline==TRUE)          // if we are following the line
        state=done;                     // return to idle state
    }
    else P2IFG=disabled;
  }
  else P2IFG = disabled;
}

//switch 1 interrupt (left button)
//no functionality programmed.
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
  sw1_pressed=P4IFG & SW1;
  if (sw1_pressed){
    P4IFG &= !SW1;                      //disable interrupt flag
    if (sw1_debounce==disabled){        
      sw1_debounce=debounce_delay;      //set debounce
    }                                   //no other functionality programmed
    else P4IFG=disabled;
  }
  else P4IFG=disabled;
}