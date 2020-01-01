//------------------------------------------------------------------------------
//
//  Description: This file contains a chart of port definitions and
//                      all the initialization functions for the msp430's ports.
//
//  Benjamin Ryle
//  Sept 11
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------

#include  "functions.h"            // lists function names for all pre-compiled files
#include  "msp430.h"               // provides names for every port
#include  <string.h>               // string functions from std library
#include  "macros.h"               // global definitions file
#include "ports.h"                 // translation of port bit numbers to port names
int IOT_COUNTDOWN=100;             // countdown until an iot process is run
//-----------------------------------------------------------------------------------------
//all values are instantiated to the safest position
//in this case, it's low output gpio
//all port values set to 0
//all ports set to outputs  
//ONLY PORTS WITH DEFINED VALUES NEED TO BE WRITTEN HERE
//a value will only be instantiated if a change from the constructor is needed.
//-----------------------------------------------------------------------------------------

//PORT 1
//left as GPIO output 0
void Init_Port_1(void){
  P1OUT=GPIO_all;
  P1SEL0=GPIO_all; 
  P1SEL1=GPIO_all;
  P1DIR=output_all;
  
  
  //P1 PIN 0
  //P1 PIN 1
  P1SEL0 |=  A1_SEEED;
  P1SEL1 |=  A1_SEEED;
  //P1 PIN 2
  P1SEL0 |=  V_DETECT_L;
  P1SEL1 |=  V_DETECT_L;
    P1REN  &=  ~V_DETECT_L;
  //P1 PIN 3     
  
  P1SEL0 |=  V_DETECT_R;
  P1SEL1 |=  V_DETECT_R;
  P1REN  &=  ~V_DETECT_R;
  //P1 PIN 4
  P1SEL0 |=  V_BAT;
  P1SEL1 |=  V_BAT;
  //P1 PIN 5
  P1SEL0 |=  V_THUMB;
  P1SEL1 |=  V_THUMB;
  //P1 PIN 6
  P1SEL0 |=  UCA0RXD;
  P1SEL1 &= ~UCA0RXD;
  //P1 PIN 7
  P1SEL0 |=  UCA0TXD;
  P1SEL1 &= ~UCA0TXD;
}

//PORT 2
//left as GPIO output 0
void Init_Port_2(void){
  P2OUT=GPIO_all;
  P2SEL0=GPIO_all;
  P2SEL1=GPIO_all;
  P2DIR=output_all;


  //P2 PIN 0
  //P2 PIN 1
  //P2 PIN 2
  //P2 PIN 3
  P2SEL0 &= ~SW2;
  P2SEL1 &= ~SW2;
  P2DIR &= ~SW2;
  P2OUT |=  SW2;
  P2REN |=  SW2; 
  P2IES |= SW2;
  P2IFG &= ~SW2;
  P2IE |= SW2;
  //P2 PIN 4
  //P2 PIN 5
  //P2 PIN 6
  P2SEL0 &= ~LFXOUT;                    // Primary (MCLK)
  P2SEL1 |= LFXOUT;                     // Primary (MCLK)
  //P2 PIN 7
  P2SEL0 &= ~LFXIN;                     // Primary (TB0CLK)
  P2SEL1 |= LFXIN;                      // Primary (TB0CLK)
}

//PORT 3
//left as GPIO output 0
void Init_Port_3(int use_smclk){
  P3OUT=GPIO_all;
  P3SEL0=GPIO_all;
  P3SEL1=GPIO_all;
  P3DIR=output_all;
  
  //P3 PIN 0
  //P3 PIN 1
  //P3 PIN 2
  //P3 PIN 3
  if (use_smclk)
    P3SEL0 |=SMCLK;
  //P3 PIN 4
  //P3 PIN 5
  //P3 PIN 6       
  P3OUT &= ~IOT_LINK;
  //P3 PIN 7

}

//PORT 4
//sets up the lcd screen to use ucb1 as a spi interface instead of gpio
void Init_Port_4(void){
  P4OUT=GPIO_all;
  P4SEL0=GPIO_all;
  P4SEL1=GPIO_all;
  P4DIR=output_all;
  
  //P4 PIN 0
  //LCD_RESET GETS RESET CORRECTLY AT TOP
  P4SEL0 &= ~RESET_LCD;
  P4SEL1 &= ~RESET_LCD;
  P4OUT  &= ~RESET_LCD;
  P4DIR  |=  RESET_LCD;
  
  //P4 PIN 1
  P4SEL0 &= ~SW1; //sw1 set as io
  P4SEL1 &= ~SW1;
  P4DIR &= ~SW1; //SW1 direction input
  P4OUT |=  SW1; // SW1 PULL UP RESISTOR CONFIG
  P4REN |=  SW1;  //SW1 PULL UP RESISTOR ENABLE
  P4IES |= SW1; //HI/LO EDGE INTERRUPT
  P4IFG &= ~SW1; //SW1 CLEARED
  P4IE |= SW1; //INTERRUPT ENABLED
  
  //P4 PIN 2 
  P4SEL0 |=  UCA1TXD;
  P4SEL1 &= ~UCA1TXD;
  
  //P4 PIN 3
  P4SEL1 &= ~UCA1RXD;
  P4SEL0 |=  UCA1RXD;
  
  //P4 PIN 4
  P4SEL1 &= ~UCB1_CS_LCD;
  P4SEL0 &= ~UCB1_CS_LCD;// sets SPI_CS_LCD off (high)
  P4OUT |= UCB1_CS_LCD;
  P4DIR |= UCB1_CS_LCD;
  
  //P4 PIN 5     
  P4SEL0 |=  UCB1CLK; // this defines it as a spi interface
  P4SEL1 &= ~UCB1CLK; //P4SELn=(0,1)
  
  //P4 PIN 6
  P4SEL0 |=  UCB1SIMO; //
  P4SEL1 &= ~UCB1SIMO; //P4SELn=(0,1)
  
  //P4 PIN 7
  P4SEL0 |=  UCB1SOMI; //
  P4SEL1 &= ~UCB1SOMI; //P4SELn=(0,1)
  
}

//PORT 5
//left as GPIO output 0
void Init_Port_5(void){
  P5OUT=GPIO_all;
  P5SEL0=GPIO_all;
  P5SEL1=GPIO_all;
  P5DIR=output_all;
  
  //P5 PIN 0
  P5OUT &= ~IOT_RESET;
  //P5 PIN 1
  P5SEL0 &= ~IR_LED;
  P5SEL1 &= ~IR_LED;
  P5OUT |= IR_LED;
  P5DIR |= IR_LED;
  //P5 PIN 2
  //P5 PIN 3
  //P5 PIN 4
  //P5 PIN 5
  //P5 PIN 6
  //P5 PIN 7
}

//PORT 6
//the lcd backlight is enabled. this a higher level non-gpio function.
void Init_Port_6(void){
  P6OUT=GPIO_all;
  P6SEL0=GPIO_all;
  P6SEL1=GPIO_all;
  P6DIR=output_all;
  
  //P6 PIN 0 R_FORWARD
  
  P6SEL0 |= R_FORWARD;
  P6SEL1 &= ~R_FORWARD;
  P6DIR |=R_FORWARD;
  P6OUT |= R_FORWARD;
  //P6 PIN 1
  P6SEL0 |= L_FORWARD;
  P6SEL1 &= ~L_FORWARD;
  P6DIR |= L_FORWARD;
  P6OUT |= L_FORWARD;
  //P6 PIN 2
  P6SEL0 |= R_REVERSE;
  P6SEL1 &= ~R_REVERSE;
  P6DIR |= R_REVERSE;
  P6OUT |= R_REVERSE;
  //P6 PIN 3
  P6SEL0 |= L_REVERSE;
  P6SEL1 &= ~L_REVERSE;
  P6DIR |= L_REVERSE;
  P6OUT |= L_REVERSE;
  //P6 PIN 4
  //P6DIR  &= ~LCD_BACKLIGHT; //backlight signal is an output
  P6OUT |= LCD_BACKLIGHT; //comment out to turn off light
  //P6 PIN 5
  //P6 PIN 6
  //P6 PIN 7
}


//this is the main function you will be calling externally
//it runs an initiation function for every port 1 through 6 sequentially.
void Init_Ports(void){
  Init_Port_1();
  Init_Port_2();
  Init_Port_3(USE_SMCLK);
  Init_Port_4();
  Init_Port_5();
  Init_Port_6();
  IOT_COUNTDOWN = 10; 
}
