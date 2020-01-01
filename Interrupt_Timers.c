//------------------------------------------------------------------------------
//
//  Description: This file contains every Timer interrupt programmed
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


int startup=TRUE;                               //whether the module is first starting up, used to start IOT setup state machine
extern volatile int motor_next_direction_L;     //next direction for the left motor (forward/reverse)
extern volatile int motor_direction_L;          //current motor direction for the left motor
extern volatile int  motor_direction_R;         //current motor direction for the right motor
extern volatile int motor_next_direction_R;     //next direction for the right motor (forward/reverse)
extern int MotorTimer;                          // a countdown to when motors will be able to work again after changing directions

volatile unsigned int Time_Sequence;            // a count to activate certain activities after N number of times, or 1 in every n numbers
unsigned int disp_time;                         // count of how many seconds have passed to print a timer to the lcd display

char display_timer;                             // count of how many seconds have passed to print a timer to the lcd display
extern int IOT_COUNTDOWN;                       // timer based countdown for IOT state machine
extern int sw1_debounce;                        // disables the left switch so 1 button press isn't registered as 2 or more
extern int sw2_debounce;                        //disables the right switch so 1 button press isn't registered as 2 or more
extern int Motor_timer;                         // countdown until motors are automatically turned off without input

extern volatile unsigned char display_changed;
volatile int State_Timer;                       // Counts down on a timer and then sets State_Trigger to true
char State_Trigger;                             // nonblocking delay in line following statemacahine
volatile int State_Timer2;                      // Counts down on a timer and then sets State_Trigger2 to true
char State_Trigger2;                            // noblocking delay in line following statemachine
void enable_display_update(void);               // enables the lcd screen to be updated
void out_string(void);                          // outputs a string to the IOT module

char diss_int []= "AT+WSYNCINTRL=2000";                 // changes dissociation timer so the wifi doesn't disconnect
char start_tcp [] ="AT+NSTCP=11100,1";                  // opens TCP port 11100
//usb ring buffer
extern volatile char USB_Char_Rx[SMALL_RING_SIZE] ;    // usb  receive  ring buffer array
extern volatile unsigned int usb_rx_ring_wr;           // usb  receive  ring buffer write position
extern unsigned int usb_rx_ring_rd;                    // usb  recieve  ring buffer read  position
extern volatile char USB_Char_Tx[LARGE_RING_SIZE] ;    // usb  transmit ring buffer array
extern volatile unsigned int usb_tx_ring_wr;           // usb  transmit ring buffer write position
extern unsigned int usb_tx_ring_rd;                    // usb  transmit ring buffer read  position

//wifi ring buffer
extern char USB_Char_Rx1[SMALL_RING_SIZE] ;            // wifi receive  ring buffer array
extern volatile unsigned int usb_rx_ring_wr1;          // wifi recieve  ring buffer write position
extern unsigned int usb_rx_ring_rd1;                   // wifi recieve  ring buffer read  position
extern volatile char USB_Char_Tx1[LARGE_RING_SIZE] ;   // wifi transmit ring buffer array
extern volatile unsigned int usb_tx_ring_wr1;          // wifi transmit ring buffer write position
extern unsigned int usb_tx_ring_rd1;                   // wifi transmit ring buffer read  position
extern char command_string [11];                       // string reflecting the current IOT command output to LCD screen
extern char ip_line1   [LCD_LEN];                       // line 1 of string storing ip address to print to lcd
extern char ip_line2   [LCD_LEN];                       // line 2 of string storing ip address to print to lcd
//char quick_array [128];
int gather_ip=TRUE;                                     //whether the ip address is being gathered to output to the LCD screen
void motors_off(void);                                  //turns off motors
//
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){//every 1.25 ms
  TB0CCR0 +=TB0CCR0_INTERVAL;                           //adds offset to TBCCR0
  display_timer++;
  if (display_timer==display_update_time){              //if it's time to update the display
    enable_display_update();
    Display_Update(LCD_INST,LCD_MID,LCD_TOP,LCD_TOP);   //run the display update function
    display_timer=rst_time;
    display_changed=rst_time;                           //disable display changed flag
  }
  if (MotorTimer==rst_time){                            //if the motors have been off long enough to deplete any built up charge
    motor_direction_R=motor_next_direction_R;           //set the motor direction from disabled to the next direction
    motor_direction_L=motor_next_direction_L;
    MotorTimer--;                                       //disable the countdown
  }
  else if (MotorTimer>rst_time)
    MotorTimer--;
  if (State_Timer>rst_time)
    State_Timer--;
  else if (State_Timer==rst_time){
    State_Trigger=TRUE;
    State_Timer--;
  }
  if (State_Timer2>rst_time)
    State_Timer2--;
  else if (State_Timer2==rst_time){
    State_Trigger2=TRUE;
    State_Timer2--;
  }  
}


//this is the interrupt responsible for re-enabling the buttons about 500ms after it's pressed.
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
  //timerB0 1-2, Overflow interrupt vector (TBV1 handler)
  
  switch(__even_in_range(TB0IV,overflow)){
  case disabled: break;
  case ccr1: //timer debounce every 62.5 ms
    if (Time_Sequence%4==BEGINNING & ADC_ENABLE){
      ADCCTL0 |= ADCENC;
    }
    Time_Sequence++;
    if (sw1_debounce>disabled) //we are counting our disabled timer down to 0
      sw1_debounce--;
    if (sw2_debounce>disabled)//at 0, we can press our button again in it's respective interrupt
      sw2_debounce--;
    
    TB0CCR2 +=TB0CCR1_INTERVAL;
    break;
  case ccr2:       //ccr2 every 25 ms
    TB0CCR2 +=TB0CCR2_INTERVAL;
    disp_time+=disp_increment;
    if (Motor_timer>BEGINNING)
      Motor_timer--;
    else if (Motor_timer==BEGINNING){                           //if the motors are turned off
      motors_off();
      Motor_timer--;
      command_string[2]='S';
      command_string[3]='T';                                    //set the LCD display to say STOP
      command_string[4]='O';                                    //(depending on the menu state machine)
      command_string[5]='P';
      command_string[6]=' ';
      command_string[7]=' ';
      command_string[8]=' ';
    }
    if(IOT_COUNTDOWN>BEGINNING)
      IOT_COUNTDOWN--;
    else
    switch (startup){                                           //state machine for IOT startup (wifi startup)
  case expire:                                                  
      for (int i=BEGINNING; i<(sizeof(diss_int)+1); i++){       // set dissociation timer high enough to not disconnect from the network
        USB_Char_Tx[usb_tx_ring_wr]=diss_int[i];
        usb_tx_ring_wr++;
        if (usb_tx_ring_wr>sizeof(USB_Char_Tx))
          usb_tx_ring_wr=BEGINNING;
      }
      //startup=3;
      startup++;
      IOT_COUNTDOWN=expire_delay;
      USB_Char_Tx[usb_tx_ring_wr-1]= '\r';                      // newline tells the IOT module to run the previous command
      out_string();
    break;
    
  case turnon:
      P5OUT |= IOT_RESET;
      startup++;
      IOT_COUNTDOWN=turnon_delay;                               //delay to display on screen first
    break;
  
    case tcp_setting:
      startup=FALSE;
      for (int i=BEGINNING; i<sizeof(start_tcp); i++){          // tell IOT module to set up a TCP server
        USB_Char_Tx[usb_tx_ring_wr]=start_tcp[i];
        usb_tx_ring_wr++;
        if (usb_tx_ring_wr>sizeof(USB_Char_Tx))
          usb_tx_ring_wr=BEGINNING;
      }
      IOT_COUNTDOWN=tcp_setting_delay;//EVERY 5 SECONDS         
      USB_Char_Tx[usb_tx_ring_wr-1]= '\r';                      
      out_string();
       ip_line2[8]='O';                                         // print OK to the display to show the state machien is done
       ip_line2[9]='K';
  break;
 
    }
    break;
  case  overflow: //overflow
    break;
  default: break;
  }
}