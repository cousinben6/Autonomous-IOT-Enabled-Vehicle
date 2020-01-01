
#include  "functions.h"
#include  "msp430.h"
#include "motor_functions.h"
#include <string.h>
#include "macros.h"
#include "ports.h"
#include "motors.h"
#include "timers.h"

extern char LCD_LINE1 [LCD_LEN];
extern char LCD_LINE2 [LCD_LEN];
extern char LCD_LINE3 [LCD_LEN];
extern char LCD_LINE4 [LCD_LEN];

extern char display_line[lcd_row][lcd_col];
extern char *display[lcd_disp];
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;

void LCD_Write(void){  
LCD_LINE1 [newline_num] = '\r';
LCD_LINE2 [newline_num] = '\r';
LCD_LINE3 [newline_num] = '\r';
LCD_LINE4 [newline_num] = '\r';
  strcpy(display_line[LCD_TOP], LCD_LINE1);
  update_string(display_line[LCD_TOP], LCD_TOP);
  strcpy(display_line[LCD_MID], LCD_LINE2);
  update_string(display_line[LCD_MID], LCD_MID);
  strcpy(display_line[LCD_BOT], LCD_LINE3);
  update_string(display_line[LCD_INST], LCD_INST);
  strcpy(display_line[LCD_LOW], LCD_LINE4);
  update_string(display_line[LCD_LOW], LCD_LOW);
}