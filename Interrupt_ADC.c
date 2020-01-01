//------------------------------------------------------------------------------
//
//  Description: This file contains every ADC interrupt programmed so far
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

void motors_off(void);          //ability to cut off motors for safety reasons

int Left_Color;                 // whether the left IR detector sees black or white
int Right_Color;                // whether the right IR detector sees black or white

extern unsigned int followtheline;              // whether we are following the black line
extern volatile int motor_next_direction_L;     // what the next direction for the left motor will be
extern volatile int motor_direction_L;          // what the current direction for the left motor is.
extern volatile int motor_direction_R;          // what the current direction for the right motor is.
extern volatile int motor_next_direction_R;     // what the next direction for the right motor will be.
volatile unsigned int ADC_Thumb;                // adc value of thumb wheel

int ADC_Channel;                                // next device to be judged by analog to digital converter
unsigned int ADC_Left_Detect;                   // Left Infared Detector ADC value
unsigned int ADC_Right_Detect;                  // Right Infared Detector ADC value

extern int IOT_COUNTDOWN;                       // Countdown before starting next IOT process
extern char LCD_LINE1 [LCD_LEN];                // String outputting to line 1 of the lcd
extern char LCD_LINE2 [LCD_LEN];                // String outputting to line 2 of the lcd
extern char LCD_LINE3 [LCD_LEN];                // String outputting to line 3 of the lcd
extern char LCD_LINE4 [LCD_LEN];                // String outputting to line 4 of the lcd


extern volatile unsigned char display_changed;  // whether the LCD should update
unsigned int Black_Detect=160;                  // ADC threshold (larger is black. smaller is white.)


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

extern char ip_line1   [LCD_LEN];               // what will be printed to the LCD when the Menu state machine wants to print the ip address.
extern char ip_line2   [LCD_LEN];               // what will be printed to the LCD when the Menu state machine wants to print the ip address.
extern char command_string [11];                // what command has been recieved from the IOT module via TCP


#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
  switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
  case ADCIV_NONE:
    break;
  case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
    // before its previous conversion result was read.
    break;
  case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
    break;
  case ADCIV_ADCHIIFG: // Window comparator interrupt flags
    break;
  case ADCIV_ADCLOIFG: // Window comparator interrupt flag
    break;
  case ADCIV_ADCINIFG: // Window comparator interrupt flag
    break;
  case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
    ADCCTL0 &= ~ADCENC; // Disable ENC bit.
    switch (ADC_Channel++){
    case left_detect: // Channel A2 Interrupt
      
      ADC_Left_Detect = ADCMEM0; // Move result into Global
      ADC_Left_Detect = ADC_Left_Detect >> 4; // Divide the result by 8
      if (ADC_Left_Detect-60<Black_Detect)
        Left_Color=WHITE;
      else Left_Color=BLACK;
      ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
      ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
      break;
      
    case right_detect:
      
      ADC_Right_Detect = ADCMEM0; // Move result into Global
      ADC_Right_Detect = ADC_Right_Detect >> 4; // Divide the result by 8
      if (ADC_Right_Detect<Black_Detect)
        Right_Color=WHITE;
      else Right_Color=BLACK;
      ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A3
      ADCMCTL0 |= ADCINCH_5; // Enable Next channel A4
      break;
      
    case thumb_wheel:
      ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A4
      ADCMCTL0 |= ADCINCH_2;
      ADC_Thumb = ADCMEM0;
      ADC_Channel=rst_time;
      ADCCTL0 &= ~ADCENC;
      //     IR_Timer=LED_Refresh_Time;
      
      break;
    default:
      break;
    }
    ADCCTL0 |= ADCENC; // Enable Conversions
    ADCCTL0 |= ADCSC; // Start next sample
    break;
  default:
    break;
  }
}
