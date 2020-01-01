#include "macros.h"
#include "StateMachine.h"
extern int Left_Color;
extern int Right_Color;
void HexToString(unsigned int hexnum);
extern unsigned int ADC_Right_Detect;
extern volatile unsigned int ADC_Thumb;
unsigned int tmp_disp_time;
extern unsigned int disp_time;
extern char LCD_LINE1 [LCD_LEN];
extern char LCD_LINE2 [LCD_LEN];
extern char LCD_LINE3 [LCD_LEN];
extern char LCD_LINE4 [LCD_LEN];

extern char adc_char [LCD_LEN];
extern char hextostring[LCD_LEN];

extern int ADC_Channel;
extern unsigned int ADC_Left_Detect;
void LCD_Write(void);

void     ADC_To_Display(void){
  HexToString(ADC_Thumb);
  for (int i=0; i<2; i++)
    LCD_LINE1[i+5]=hextostring[i+7];
  
  HexToString(ADC_Left_Detect);
  for (int i=0; i<3; i++)
    LCD_LINE2[i+1]=hextostring[i+7];
  
  HexToString(ADC_Right_Detect);
  for (int i=0; i<3; i++)
    LCD_LINE2[i+6]=hextostring[i+7];
  
  tmp_disp_time=disp_time;
  tmp_disp_time=tmp_disp_time/100;
  for (int i=0; i<4; i++){
    LCD_LINE1[4-i]=tmp_disp_time%10+48;
    tmp_disp_time=tmp_disp_time/10;
  }
  if (Left_Color==BLACK && Right_Color==WHITE){
  LCD_LINE3[0]='L';
  LCD_LINE3[1]='E';
  LCD_LINE3[2]='F';
  LCD_LINE3[3]='T';
  LCD_LINE3[4]=' ';
  }
  else if(Left_Color==WHITE && Right_Color==BLACK){
    LCD_LINE3[0]='R';
  LCD_LINE3[1]='I';
  LCD_LINE3[2]='G';
  LCD_LINE3[3]='H';
  LCD_LINE3[4]='T';
  }
}