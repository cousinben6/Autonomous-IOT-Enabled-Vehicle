//------------------------------------------------------------------------------
//
//  Description: This file defines the method, and values to make UCA0
//                              work at 115200 baud.
//
//  Benjamin Ryle
//  Sept 11
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------
#include  "functions.h"            // lists function names for all pre-compiled files
#include  "msp430.h"               // provides names for every port
#include  <string.h>               // string functions from std library
#include  "macros.h"               // global definitions file
#include "ports.h"                 // definitions of port listings
#include "motors.h"                // motor definitions file
#include "timers.h"                // definitions for timer instantiation
#include "StateMachine.h"          // definitions for line following state machine

// global variables
//usb ring buffer
volatile char USB_Char_Rx[SMALL_RING_SIZE] ;    // usb  receive  ring buffer array
volatile unsigned int usb_rx_ring_wr;           // usb  receive  ring buffer write position
unsigned int usb_rx_ring_rd;                    // usb  recieve  ring buffer read  position
volatile char USB_Char_Tx[LARGE_RING_SIZE] ;    // usb  transmit ring buffer array
volatile unsigned int usb_tx_ring_wr;           // usb  transmit ring buffer write position
unsigned int usb_tx_ring_rd;                    // usb  transmit ring buffer read  position

//wifi ring buffer
volatile char USB_Char_Rx1[SMALL_RING_SIZE] ;   // wifi receive  ring buffer array
volatile unsigned int usb_rx_ring_wr1;          // wifi recieve  ring buffer write position
unsigned int usb_rx_ring_rd1;                   // wifi recieve  ring buffer read  position
volatile char USB_Char_Tx1[LARGE_RING_SIZE] ;   // wifi transmit ring buffer array
volatile unsigned int usb_tx_ring_wr1;          // wifi transmit ring buffer write position
unsigned int usb_tx_ring_rd1;                   // wifi transmit ring buffer read  position


int rate = one_fifteen;                         // instantiate to 115,200 baud (also supports 460,800 baud, just write four_sixty)
int UCA1_ON;                                    

///////////////////////////////////////////////
// this function defines serial transmission to the IOT module speed as 115200 baud
//      or as 460,800 baud depending on whether the rate variable is set as one_fifteen, or four_sixty
//
///////////////////////////////////////////////
void Init_Serial_UCA1(void){//115200 baud
  int i;
  UCA1_ON=0;
  for(i=0; i<SMALL_RING_SIZE; i++){
    USB_Char_Rx1[i] = 0x00; // USB Rx Buffer
  }
  usb_rx_ring_wr1 = BEGINNING;
  usb_rx_ring_rd1 = BEGINNING;
  for(i=0; i<LARGE_RING_SIZE; i++){ // May not use this
    USB_Char_Tx1[i] = 0x00; // USB Tx Buffer
  }
  usb_tx_ring_wr1 = BEGINNING;
  usb_tx_ring_rd1 = BEGINNING;
  // Configure UART 1
  UCA1CTLW0 = 0; // Use word register
  UCA1CTLW0 |= UCSWRST; // Set Software reset enable
  UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
  /*1. Calculate N=fBRCLK / Baudrate
  a. N=SMCLK/115,200 =>8,000,000/115,200 = 69.444444
  b. N=SMCLK/460,800 =>8,000,000/460,800 = 17.361111
  N>16 so skip 2.
  2. OS16=0 UCBRx = INT(N) 
  3. OS16=1
  UCx = INT (N/16)
  a. UCx = INT(4.340)
  b. UCx = INT(1.085)
  
  UCFx = INT([(N/16) - INT(N/16)]*16)
  a. UCFx = 5
  b. UCFx = 1
  4. UCSx is found by looking up thte fractional part of N = N-INT(N) in table 18-4
  a. UCSx = 0x55
  b. UCSx = 0x4A
  5. if OS16=0 was chosen, a detailed error calc is reccomended to be performed.
  // BRCLK 	Baudrate 	UCOS16 	UCBRx 	UCFx 	UCSx 	
  // 8000000 	115200 		1 	4 	5 	0x55 
  // 8000000    460800          1       1       1       0x4A                                    */
  UCA1BRW = 4; // 115,200 Baud
  //    UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
  //a.  UCA0MCTLW = 0x55 concatenate 5 concatenate 1;
  //b.  UCA0MCTLW = 0x4A concatenate 1 concatenate 1;
  UCA1MCTLW = 0x5551 ;
  UCA1CTLW0 &= ~ UCSWRST; // Set Software reset enable
  UCA1IE |= UCRXIE; // Enable RX interrupt
}

///////////////////////////////////////////////
// this function defines USB serial transmission speed as 115200 baud
//      or as 460,800 baud depending on whether rate is set as one_fifteen, or four_sixty
//
///////////////////////////////////////////////
void Init_Serial_UCA0(void){
  if (rate==one_fifteen){
    int i;
    for(i=0; i<SMALL_RING_SIZE; i++){
      USB_Char_Rx[i] = 0x00; // USB Rx Buffer
    }
    usb_rx_ring_wr = BEGINNING;
    usb_rx_ring_rd = BEGINNING;
    for(i=0; i<LARGE_RING_SIZE; i++){ // May not use this
      USB_Char_Tx[i] = 0x00; // USB Tx Buffer
    }
    usb_tx_ring_wr = BEGINNING;
    usb_tx_ring_rd = BEGINNING;
    // Configure UART 0
    UCA0CTLW0 = 0; // Use word register
    UCA0CTLW0 |= UCSWRST; // Set Software reset enable
    UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
    /*1. Calculate N=fBRCLK / Baudrate
    a. N=SMCLK/115,200 =>8,000,000/115,200 = 69.444444
    b. N=SMCLK/460,800 =>8,000,000/460,800 = 17.361111
    N>16 so skip 2.
    2. OS16=0 UCBRx = INT(N) 
    3. OS16=1
    UCx = INT (N/16)
    a. UCx = INT(4.340)
    b. UCx = INT(1.085)
    
    UCFx = INT([(N/16) - INT(N/16)]*16)
    a. UCFx = 5
    b. UCFx = 1
    4. UCSx is found by looking up thte fractional part of N = N-INT(N) in table 18-4
    a. UCSx = 0x55
    b. UCSx = 0x4A
    5. if OS16=0 was chosen, a detailed error calc is reccomended to be performed.
    // BRCLK 	Baudrate 	UCOS16 	UCBRx 	UCFx 	UCSx 	
    // 8000000 	115200 		1 	4 	5 	0x55 
    // 8000000    460800          1       1       1       0x4A                                    */
    UCA0BRW = 4; // 115,200 Baud
    //    UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
    //a.  UCA0MCTLW = 0x55 concatenate 5 concatenate 1;
    //b.  UCA0MCTLW = 0x4A concatenate 1 concatenate 1;
    UCA0MCTLW = 0x5551 ;
    UCA0CTLW0 &= ~ UCSWRST; // Set Software reset enable
    UCA0IE |= UCRXIE; // Enable RX interrupt
  }
  else if (rate==four_sixty)
  {
    int i;
    for(i=0; i<SMALL_RING_SIZE; i++){
      USB_Char_Rx[i] = 0x00; // USB Rx Buffer
    }
    usb_rx_ring_wr = BEGINNING;
    usb_rx_ring_rd = BEGINNING;
    for(i=0; i<LARGE_RING_SIZE; i++){ // May not use this
      USB_Char_Tx[i] = 0x00; // USB Tx Buffer
    }
    usb_tx_ring_wr = BEGINNING;
    usb_tx_ring_rd = BEGINNING;
    // Configure UART 0
    UCA0CTLW0 = 0; // Use word register
    UCA0CTLW0 |= UCSWRST; // Set Software reset enable
    UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
    /*1. Calculate N=fBRCLK / Baudrate
    a. N=SMCLK/115,200 =>8,000,000/115,200 = 69.444444
    b. N=SMCLK/460,800 =>8,000,000/460,800 = 17.361111
    N>16 so skip 2.
    2. OS16=0 UCBRx = INT(N) 
    3. OS16=1
    UCx = INT (N/16)
    a. UCx = INT(4.340)
    b. UCx = INT(1.085)
    
    UCFx = INT([(N/16) - INT(N/16)]*16)
    a. UCFx = 5
    b. UCFx = 1
    4. UCSx is found by looking up thte fractional part of N = N-INT(N) in table 18-4
    a. UCSx = 0x55
    b. UCSx = 0x4A
    5. if OS16=0 was chosen, a detailed error calc is reccomended to be performed.
    // BRCLK 	Baudrate 	UCOS16 	UCBRx 	UCFx 	UCSx 	
    // 8000000 	115200 		1 	4 	5 	0x55 
    // 8000000    460800          1       1       1       0x4A                                    */
    UCA0BRW = 1;
    //    UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
    //a.  UCA0MCTLW = 0x55 concatenate 5 concatenate 1;
    //b.  UCA0MCTLW = 0x4A concatenate 1 concatenate 1;
    UCA0MCTLW = 0x4A11 ;
    UCA0CTLW0 &= ~ UCSWRST; // Set Software reset enable
    UCA0IE |= UCRXIE; // Enable RX interrupt
  }
}

//outputs a character to UCA0
void out_character(char character){
  //------------------------------------------------------------------------------
  // The while loop will stall as long as the Flag is not set [port is busy]
  while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
  UCA0TXBUF = character;
  //------------------------------------------------------------------------------
}
//outputs a character to UCA1
void out_character1(char character){
  //------------------------------------------------------------------------------
  // The while loop will stall as long as the Flag is not set [port is busy]
  while (!(UCA1IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
  UCA1TXBUF = character;
  //------------------------------------------------------------------------------
}

//ouptputs a string to UCA1 (from UCA1's transmit buffer)
//volatile char USB_Char_Tx1[LARGE_RING_SIZE]

void out_string1(void){
  while (usb_tx_ring_rd1!=usb_tx_ring_wr1){
    out_character1(USB_Char_Tx1[usb_tx_ring_rd1]);
    usb_tx_ring_rd1++;
    if (usb_tx_ring_rd1>sizeof(USB_Char_Tx1))
      usb_tx_ring_rd1=BEGINNING;
  }
}

//ouptputs a string to UCA0 (from UCA0's transmit buffer)
//volatile char USB_Char_Tx[LARGE_RING_SIZE]

void out_string(void){
  while (usb_tx_ring_rd!=usb_tx_ring_wr){
    out_character(USB_Char_Tx[usb_tx_ring_rd]);
    usb_tx_ring_rd++;
    if (usb_tx_ring_rd>sizeof(USB_Char_Tx))
      usb_tx_ring_rd=BEGINNING;
  }
}