//------------------------------------------------------------------------------
//
//  Description: This file contains the Intitialization of the lcd software
//
//
//  Benjamin Ryle
//  Dec 2019
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------

#include  "functions.h"            // lists function names for all pre-compiled files
#include  "msp430.h"               // provides names for every port
#include  <string.h>               // string functions from std library
#include  "macros.h"               // global definitions file

// Global Variables
extern char display_line[lcd_row][lcd_col];
extern char *display[lcd_disp];
unsigned char display_mode;

//These are the four strings that will be used to define our display
//it defaults to displaying the mac address, just for convenience sake.
char LCD_LINE1 [LCD_LEN]= "  MAC      ";
char LCD_LINE2 [LCD_LEN]= "  address  ";
char LCD_LINE3 [LCD_LEN]= "20F85EFDF6F";
char LCD_LINE4 [LCD_LEN]= "4D         ";

void Init_Display(void){
  strcpy(display_line[LCD_TOP], LCD_LINE1);
  update_string(display_line[LCD_TOP], LCD_TOP);
  strcpy(display_line[LCD_MID], LCD_LINE2);
  update_string(display_line[LCD_MID], LCD_MID);
  strcpy(display_line[LCD_BOT], LCD_LINE3);
  update_string(display_line[LCD_LOW], LCD_LOW);
  enable_display_update();
  Display_Update(LCD_LOW,LCD_MID,LCD_TOP,LCD_TOP);
}