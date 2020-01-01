//------------------------------------------------------------------------------------------
//this is a list of every port definition followed by it's subsequent value.
//if a suggested use case exists for a variable, it is listed here.
//not all defined variables are used.
//blank values are undefined as of this moment (GPIO OUTPUT 0)
//------------------------------------------------------------------------------------------
#define output_all (0xFF)
#define GPIO_all (0x00)


//~~~port 1~~~
//1.0
#define RedLED (0x01);
//1.1
#define A1_SEEED (0x02);
//1.2     
#define V_DETECT_L (0x04);
//1.3
#define V_DETECT_R (0x08);
//1.4   
#define V_BAT (0x10);
//1.5   
#define V_THUMB (0x20);
//1.6   
#define UCA0RXD (0x40);
//1.7   
#define UCA0TXD (0x80);

//~~~port 2~~~
//2.0
#define P2_0 (0x01);
//2.1
#define P2_1 (0x02);
//2.2
#define P2_2 (0x04);
//2.3
#define SW2 (0x08);
//2.4
#define P2_3 (0x10);
//2.5
#define P2_4 (0x20);
//2.6
#define LFXOUT (0x40);
//2.7
#define LFXIN (0x80);


//~~~port 3~~~
//3.0   
#define TEST_PROBE (0x01);
//3.1
//3.2
//3.3
//3.4   
#define SMCLK (0x10);
//3.5
//3.6   
#define IOT_LINK (0x40);
//3.7


//~~~port 4~~~
//4.0   
#define RESET_LCD (0x01);
//4.1
#define SW1 (0x02);
//4.2
#define UCA1RXD (0x04);
//4.3
#define UCA1TXD (0x08);
//4.4   
#define UCB1_CS_LCD (0x10);
//4.5   
#define UCB1CLK (0x20);
//4.6   
#define UCB1SIMO (0x40);
//4.7   
#define UCB1SOMI (0x80);


//~~~port 5~~~
//5.0   
#define IOT_RESET (0x01);
//5.1   
#define IR_LED (0x02);
//5.2   
#define IOT_PROGRAM_SELECT (0x04);
//5.3   
#define IOT_PROGRAM_MODE (0x08);
//5.4   
#define CHECK_BAT (0x10);
//5.5
//5.6
//5.7


//~~PORT 6~~
//6.0   
#define R_FORWARD (0x01);
//6.1   
#define L_FORWARD (0x02);
//6.2   
#define R_REVERSE (0x04);
//6.3   
#define L_REVERSE (0x08);
//6.4   
#define LCD_BACKLIGHT (0x10);
//6.5
//6.6
//6.7