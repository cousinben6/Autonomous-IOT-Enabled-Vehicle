//------------------------------------------------------------------------------
//
//  Description: This file contains every interrupt programmed so far
//                      every second, the backlight will swap from on to off
//                      pressing a button disables the button shortly, and turns
//                              the backlight off.
//  Benjamin Ryle
//  Sept 11
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------
#include "StateMachine.h"
#include  "functions.h"
#include  "msp430.h"
#include "motor_functions.h"
#include <string.h>
#include "macros.h"
#include "motors.h"
#include "timers.h"
#include "ports.h"
//extern volatile int lcd_state;
int Left_Color;
int Right_Color;
int chan_done=TRUE;
extern unsigned int followtheline;
extern volatile int motor_next_direction_L;
extern volatile int motor_direction_L;
extern volatile int  motor_direction_R;
extern volatile int motor_next_direction_R;
extern volatile unsigned int motor_counter;
extern char previous_button;
extern unsigned long shape_timer;
extern int MotorTimer;
volatile unsigned int ADC_Thumb;
volatile unsigned int Time_Sequence;
int sw1_debounce=disabled;
int sw2_debounce=disabled;
int sw2_pressed;
int sw1_pressed;
int debounce_count;
unsigned int disp_time;
int ADC_Channel;
int ADC_Left_Detect;
char display_timer;
extern int IOT_COUNTDOWN;
extern char LCD_LINE1 [LCD_LEN];
extern char LCD_LINE2 [LCD_LEN];
extern char LCD_LINE3 [LCD_LEN];
extern char LCD_LINE4 [LCD_LEN];
extern int Motor_timer;
extern unsigned int IR_Timer;
extern unsigned int state;
unsigned int ADC_Right_Detect;
char subsample_time=rst_time;
extern volatile unsigned char display_changed;
volatile int State_Timer;
char State_Trigger;
volatile int State_Timer2;
char State_Trigger2;
void enable_display_update(void);
void out_string(void);
unsigned int Black_Detect=160;
volatile int cmd_state;
char ping_google[] = "AT+PING=www.google.com,2";
char diss_int []= "AT+WSYNCINTRL=2000";
char start_tcp [] ="AT+NSTCP=11100,1";
int startup=TRUE;
extern volatile unsigned int usb_rx_ring_wr;
extern  unsigned int usb_rx_ring_rd;
extern volatile unsigned int usb_tx_ring_wr;
extern  unsigned int usb_tx_ring_rd;
extern volatile char USB_Char_Rx[SMALL_RING_SIZE] ;
extern volatile char USB_Char_Tx[LARGE_RING_SIZE] ;
extern volatile unsigned int usb_rx_ring_wr1;
extern unsigned int usb_rx_ring_rd1;
extern volatile unsigned int usb_tx_ring_wr1;
extern unsigned int usb_tx_ring_rd1;
extern volatile char USB_Char_Rx1[SMALL_RING_SIZE] ;
extern volatile char USB_Char_Tx1[LARGE_RING_SIZE] ;
unsigned int start_char;
extern char ip_line1   [LCD_LEN];
extern char ip_line2   [LCD_LEN];
extern char command_string [11];
//char quick_array [128];
int gather_ip=TRUE;
void motors_off(void);
//switch 2 interrupt (button on the right)
#pragma vector=PORT2_VECTOR
__interrupt void switch_interrupt(void) {
  sw2_pressed=P2IFG &SW2;
  if (sw2_pressed){//SW2 is pressed
    if (sw2_debounce==disabled){
      P2IFG &= !SW2;
      sw2_debounce=debounce_delay;
      if (followtheline==TRUE)
        state=pause;
      //Black_Detect=ADC_Thumb>>4;
      
      //deselect menu item
   //   if (lcd_state==song)
   //     lcd_4line();
   //   lcd_state=select;
    }
    else P2IFG=disabled;
  }
  else P2IFG = disabled;
}
//switch 1 interrupt is the only thing implemented (left button)
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
  sw1_pressed=P4IFG & SW1;
  if (sw1_pressed){
    if (sw1_debounce==disabled){
      P4IFG &= !SW1;
      sw1_debounce=debounce_delay;
    /*  if ((ADC_Thumb>>10) >0)
        lcd_BIG_mid();
      lcd_state=(ADC_Thumb>>10);
      if (lcd_state>2)
        lcd_state=2;*/
    }
    else P4IFG=disabled;
  }
  else P4IFG=disabled;
}

//
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){//every 1.25 ms
  //TB0CCR0 Is my timer for ApplyTimer(Int N);
  TB0CCR0 +=TB0CCR0_INTERVAL; //adds offset to TBCCR0
  display_timer++;
  if (display_timer==display_update_time){
    enable_display_update();
    Display_Update(LCD_INST,LCD_MID,LCD_TOP,LCD_TOP);
    display_timer=rst_time;
    display_changed=rst_time;
  }
  if (MotorTimer==rst_time){
    motor_direction_R=motor_next_direction_R;
    motor_direction_L=motor_next_direction_L;
    MotorTimer--;
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
  /*  if (IR_Timer==rst_time){
  ADCMCTL0 |= ADCINCH_2; // Enable channel A2   
  IR_Timer--;
}
  if(IR_Timer>rst_time)
  IR_Timer--;*/
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
    else if (Motor_timer==BEGINNING){
      motors_off();
      Motor_timer--;
      command_string[2]='S';
      command_string[3]='T';
      command_string[4]='O';
      command_string[5]='P';
      command_string[6]=' ';
      command_string[7]=' ';
      command_string[8]=' ';
    }
    if(IOT_COUNTDOWN>BEGINNING)
      IOT_COUNTDOWN--;
    else
    switch (startup){
  case expire:
      for (int i=BEGINNING; i<(sizeof(diss_int)+1); i++){
        USB_Char_Tx[usb_tx_ring_wr]=diss_int[i];
        usb_tx_ring_wr++;
        if (usb_tx_ring_wr>sizeof(USB_Char_Tx))
          usb_tx_ring_wr=BEGINNING;
      }
      //startup=3;
      startup++;
      IOT_COUNTDOWN=expire_delay;
      USB_Char_Tx[usb_tx_ring_wr-1]= '\r';
      out_string();
    break;
  case turnon:
      P5OUT |= IOT_RESET;
      startup++;
      IOT_COUNTDOWN=turnon_delay;//delay to display on screen first
    break;
  case tcp_setting:
      startup=FALSE;
      for (int i=BEGINNING; i<sizeof(start_tcp); i++){
        USB_Char_Tx[usb_tx_ring_wr]=start_tcp[i];
        usb_tx_ring_wr++;
        if (usb_tx_ring_wr>sizeof(USB_Char_Tx))
          usb_tx_ring_wr=BEGINNING;
      }
      IOT_COUNTDOWN=tcp_setting_delay;//EVERY 5 SECONDS
      USB_Char_Tx[usb_tx_ring_wr-1]= '\r';
      out_string();
       ip_line2[8]='O';
       ip_line2[9]='K';
  break;
  case ping_google_state:
      for (int i=BEGINNING; i<sizeof(ping_google); i++){
        USB_Char_Tx[usb_tx_ring_wr]=ping_google[i];
        usb_tx_ring_wr++;
    }
    USB_Char_Tx[usb_tx_ring_wr-1]= '\r';
    USB_Char_Tx[usb_tx_ring_wr]= '\r';
    USB_Char_Tx[usb_tx_ring_wr+1]= '\r';
    out_string();
    startup++;
    IOT_COUNTDOWN=ping_google_delay;
    break;
    }
    break;
  case  overflow: //overflow
    break;
  default: break;
  }
}

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
// global variables
extern char comand [comand_len];
char dump_array [dump_len];
int dump_cnt=BEGINNING;
int ip_log_state;
int rec_command=BEGINNING;
    unsigned int cmd_index= offset;
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
