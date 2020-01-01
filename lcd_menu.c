///////////////////////////////////////////////////////////////////////////////////////////////////
//Benjamin Ryle, August 12th 2019
//
// LCD Menu 
// this includes 3 types of scrolling windows
//
// This code is markably not well documented. It's here to show growth from starting to finishing the project
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "macros.h"
#include "StateMachine.h"
void LCD_Write(void);
extern volatile unsigned int ADC_Thumb;
void lcd_BIG_mid(void);
extern char LCD_LINE1 [LCD_LEN];
extern char LCD_LINE2 [LCD_LEN];
extern char LCD_LINE3 [LCD_LEN];
extern char LCD_LINE4 [LCD_LEN];
extern char command_string [lcd_col];
extern int ip_log_state;
extern unsigned int state;
char SSID       [LCD_LEN] = "  ncsu     ";
char IP_address [LCD_LEN] = "IPaddress  ";
char ip_line1   [LCD_LEN] = "    ...    ";
char ip_line2   [LCD_LEN] = "    ...    ";
char Intercept [] =      "Intercept  ";
char BL_Start []  =      "BL Start   ";
char BL_Travel []  =     "BL Travel  ";
char BL_Circle []  =     "BL Circle  ";
char BL_Exit []  =       "BL Exit    ";
char BL_Stop []  =       "BL Stop    ";

int lcd_state=show_ip;
char timer_string [20];
char for_input [LCD_LEN] =              "for input  ";
extern char comand [comand_len];
extern unsigned int disp_time;
extern volatile unsigned int pos_count;
/////////////////////////////////////////////////
// call this to write the menu to the lcd display
//  you still need to regularly update the display
/////////////////////////////////////////////////
void define_timer_string(void){
  int j=3;
  int l=(disp_time/40);
  int k;
  timer_string[11]=' ';
  timer_string[12]=' ';
  timer_string[18]=' ';
  timer_string[19]=' ';
  while (j<10 & l!=0){
    k=l%10;
    timer_string[20-j]=(k+48);
    j++;
    l=l/10;
  }
}
void Menu(void){
  int k=pos_count;
  switch(lcd_state){
  case show_ip:
    for (int i=LCD_START; i<LCD_OFFSET; i++){
      LCD_LINE1 [i] = ' ';
      LCD_LINE2 [i] = ' ';
      LCD_LINE3 [i] = ' ';
      LCD_LINE4 [i] = ' ';
    }
    for (int i=0; i<7; i++)
      LCD_LINE1 [i] = timer_string[i+13];
    for (int i=LCD_START; i<LCD_OFFSET; i++)
      LCD_LINE2 [i] = IP_address[i];
    for (int i=LCD_START; i<LCD_OFFSET; i++)
      LCD_LINE3 [i] = ip_line1[i];
    for (int i=LCD_START; i<LCD_OFFSET; i++)
      LCD_LINE4 [i] = ip_line2[i];
    break;
    
  case show_command:
    define_timer_string();
    for (int i=LCD_START; i<LCD_OFFSET; i++){
      LCD_LINE1 [i] = ' ';
      LCD_LINE2 [i] = ' ';
      LCD_LINE3 [i] = ' ';
      LCD_LINE4 [i] = ' ';
      LCD_LINE2 [i] = command_string[i];
    }
    for (int i=0; i<7; i++)
      if (timer_string[i+13]!= '\0')
        LCD_LINE3 [i] = timer_string[i+13];
    
    for (int j=6; j>4; j--){
      LCD_LINE1[j]=k%10+48;
      k=k/10;
    }
    
    LCD_LINE3[5]=LCD_LINE1[4];
    LCD_LINE3[4]='.';
    LCD_LINE1 [0] = 'A';
    LCD_LINE1 [1] = 'R';
    LCD_LINE1 [2] = 'V';
    LCD_LINE1 [3] = 'E';
    LCD_LINE1 [4] = 'D';
    LCD_LINE1 [5] = '?';
    break;
    //BL Start. Intercept, BL Travel, BL Circle, BL Exit, BL Stop
  case BL_lcd:
    define_timer_string();
    for (int i=LCD_START; i<LCD_OFFSET; i++){
      LCD_LINE1 [i] = ' ';
      LCD_LINE2 [i] = ' ';
      LCD_LINE3 [i] = ' ';
      LCD_LINE4 [i] = ' ';
    }
    for (int i=0; i<7; i++)
      LCD_LINE1 [i] = timer_string[i+13];
    
    switch (state){
      
    case pause://BL Start
    case Find_White:
      for (int i=LCD_START; i<LCD_OFFSET; i++)
        LCD_LINE2 [i] = BL_Start [i];
      break;
      
    case find_line://Intercept
    case start_face_line:
    case end_face_line:
    case rev_pulse:
      for (int i=LCD_START; i<LCD_OFFSET; i++)
        LCD_LINE2 [i] = Intercept [i];
      break;
      
    case BL_Travel_pause://BL Travel
    case follow_line:
      for (int i=LCD_START; i<LCD_OFFSET; i++)
        LCD_LINE2 [i] = BL_Travel [i];
      break;
      
    case BL_Circle_Pause://BL Circle
    case follow_circle:
    case white_timer:  
      for (int i=LCD_START; i<LCD_OFFSET; i++)
        LCD_LINE2 [i] = BL_Circle [i];
      break;
      
    case exit_pause://BL Exit
    case turn_right:
    case to_center:
      for (int i=LCD_START; i<LCD_OFFSET; i++)
        LCD_LINE2 [i] = BL_Exit [i];
      break;
      
    case done://BL Stop
      for (int i=LCD_START; i<LCD_OFFSET; i++)
        LCD_LINE2 [i] = BL_Stop [i];
      break;
    }
  }
}