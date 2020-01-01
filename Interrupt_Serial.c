//------------------------------------------------------------------------------
//
//  Description: This file contains every serial communication 
//      interrupt programmed so far
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

extern int startup;             // set to 1 on startup. used to start running wifi setup state machine
volatile int cmd_state;         // state of state machine to setup wifi module

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

extern int IOT_COUNTDOWN;               // delay countdown for wifi setup state machine
extern char ip_line1   [LCD_LEN];       // line 1 of string storing ip address to print to lcd
extern char ip_line2   [LCD_LEN];       // line 2 of string storing ip address to print to lcd
extern char command_string [11];        // recieved commands are placed in this string
void motors_off(void);                  // turns off motors.
// global variables
extern char comand [comand_len];        // stores commmands recieved from wifi module
int dump_cnt=BEGINNING;                 // address/index/count used to store ip address quickly
int ip_log_state;                       // state of state machine used to store ip address
int rec_command=FALSE;                  // whether we are ready to recieve a command
    unsigned int cmd_index= offset;     // index of command being stored to buffer to process in background
//------------------------------------------------------------------------------
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
  unsigned int temp;

 // int index=0;
  switch(__even_in_range(UCA0IV,0x08)){
  case 0: // Vector 0 - no interrupt
    break;
  case 2: // Vector 2 - RXIFG
    temp = usb_rx_ring_wr++;
    USB_Char_Rx[temp] = UCA0RXBUF; // RX -> USB_Char_Rx character
    if (usb_rx_ring_wr >= (sizeof(USB_Char_Rx))){
      usb_rx_ring_wr = BEGINNING; // Circular buffer back to beginning
    }
    
    //motor control state machine.
     if (USB_Char_Rx[temp]=='~')
    {//this just counts the tildas and changes states accordingly
      if (cmd_state==gurantee) rec_command=TRUE;//~#1
      if (cmd_state==hold || cmd_state==process_iot_command) cmd_state=gurantee;//~ #2
    }
        if (USB_Char_Rx[temp] && cmd_state==hold) //no tilda sent
      cmd_state=process_iot_command;
    if (rec_command){
    comand[cmd_index]=USB_Char_Rx[temp];
    cmd_index++;
    if (USB_Char_Rx[temp]=='d'){
      cmd_state=process_fram_command;
    cmd_index=1;
    }
    }

    
    //ip logging state machine
    if (ip_log_state!=ip_done)
    switch (ip_log_state){
    case wait:
      if (USB_Char_Rx[temp]=='G')
        ip_log_state=confirm;
      break;
    case confirm:
            if (USB_Char_Rx[temp]=='y')
        ip_log_state=find;
        break;
    case find:
      if (USB_Char_Rx[temp]=='\r')
        ip_log_state=line1pt1;
      break;
    case line1pt1:
      if (USB_Char_Rx[temp]=='.'){
        ip_log_state=line1pt2;
      }
      if (USB_Char_Rx[temp]!=' '){
      ip_line1[dump_cnt]=USB_Char_Rx[temp];
      dump_cnt++;
      }
      break;
    case line1pt2:
      if (USB_Char_Rx[temp]=='.'){
        ip_log_state=line2;
        dump_cnt=BEGINNING;
      }
      ip_line1[dump_cnt]=USB_Char_Rx[temp];
      dump_cnt++;
      break;
    case line2:      
      ip_line2[dump_cnt]=USB_Char_Rx[temp];
      dump_cnt++;
      if (USB_Char_Rx[temp]==':'){
        ip_log_state=ip_done;
      IOT_COUNTDOWN=found_ip_delay;
      startup=expire;
      }
      ///////////////////////////////////////////////////////////////////
      break;
    }
    //IM JUST GONNA BIND RECIEVED STUFF FROM THE IOT TO USB OUT
    //on startup, i toss all input into an array.
    temp=UCA0RXBUF;
    UCA1TXBUF=temp;
    break;
  case 4: // Vector 4 – TXIFG
    temp = usb_tx_ring_wr++;
    UCA0TXBUF = USB_Char_Tx[temp];
    if (usb_tx_ring_wr >= (sizeof(USB_Char_Tx))){
      usb_tx_ring_wr = BEGINNING; // Circular buffer back to beginning
      break;
    default: break;
    }
  }
}

// global variables

//------------------------------------------------------------------------------
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
  unsigned int temp;

  switch(__even_in_range(UCA1IV,0x08)){
  case 0: // Vector 0 - no interrupt
    break;
  
  case 2: // Vector 2 - RXIFG UCA1RXBUF_
    temp = usb_rx_ring_wr1++;
    USB_Char_Rx1[temp] = UCA1RXBUF; // RX -> USB_Char_Rx character
    if (usb_rx_ring_wr1 >= (sizeof(USB_Char_Rx1))){
      usb_rx_ring_wr1 = BEGINNING; // Circular buffer back to beginning
    }
    break;
    
  case 4: // Vector 4 – TXIFG UCA1TXBUF_
    temp = usb_tx_ring_wr1++;
    UCA1TXBUF = USB_Char_Rx1[temp];
        if (usb_tx_ring_wr >= (sizeof(USB_Char_Tx)))
      usb_tx_ring_wr = BEGINNING; // Circular buffer back to beginning
      break;
    default: break;
    }
  }