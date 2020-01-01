#include "macros.h"
extern char hextostring[LCD_LEN];

void HexToString(unsigned int hexnum){
  int tmp;
  for (int i=9; i>-1; i--){
    tmp=hexnum%16;
    hexnum=hexnum/16;
    if (tmp>9)
      hextostring[i]=(tmp+55);
    else 
      hextostring[i]=(tmp+48);
  }
}