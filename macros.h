//------------------------------------------------------------------------------
//
//  Description: This file contains definitions for names objects that failed to meet any rhyme or reason during development
//				To be blunt, it's a product of time crunch, and is an example of a mistake i made during development.
//				
//  Benjamin Ryle
//  Sept 11
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.12.4)
//------------------------------------------------------------------------------

#define RED_LED_ON      (P1OUT |= RED_LED)
#define RED_LED_OFF    (P1OUT &= ~RED_LED)
#define GREEN_LED_ON    (P6OUT |= GRN_LED)
#define GREEN_LED_OFF  (P6OUT &= ~GRN_LED)
#define ALWAYS                         (1)
#define RESET_STATE                    (0)
#define RED_LED                     (0x01) // RED LED 0
#define GRN_LED                     (0x40) // GREEN LED 1
#define just_grn_state (200)
#define red_state (150)
#define green_state (100)
#define off_state (50)
#define change_display (250)
#define rst_time (0)
#define changed (1)
#define LCD_TOP (0)
#define LCD_MID (1)
#define LCD_BOT (2)
#define LCD_LOW (3)
#define LCD_INST (3)
#define lcd_row (4)
#define lcd_col (11)
#define lcd_disp (4)
#define LCD_LEN (11)
#define display_update_time (160)
#define LCD_OFFSET (10)
#define LCD_START (0)
// MACROS========================================================================
#define MCLK_FREQ_MHZ 8                     // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)
#define USE_GPIO        (0x00)
#define USE_SMCLK       (0x01)

#define ADC_ENABLE (0)
#define BEGINNING (0)
#define SMALL_RING_SIZE (128)
#define LARGE_RING_SIZE (128)
//UART macros.
#define transmitting (1)
#define recieving (0)
#define FALSE (0)
#define TRUE (1)
#define one_fifteen (1)
#define four_sixty (2)

//menu macros.
#define show_ip (0)
#define show_command (1)
#define BL_lcd (2)
//found_ip
#define wait (0)
#define confirm (1)
#define find (2)
#define line1pt1 (3)
#define line1pt2 (4)
#define line2 (5)
#define ip_done (6)

#define hold (0)
#define gurantee (1)
#define recieve_command (2)
#define process_iot_command (3)
#define process_fram_command (4)
#define finish_command (5)

//state machine
#define turnon (1)
#define turnon_delay (50)
#define found_ip_delay (10)
#define expire (2)
#define expire_delay (20)
#define tcp_setting (4)
#define tcp_setting_delay (100)
#define ping_google_state (3)
#define ping_google_delay (50)
#define comand_len (15)
#define dump_len (30)
#define offset (7)

#define timer_disabled (-1)

//parsing wheel commands
#define parse_direction (12)
#define parse_time (13)
#define time_constant (5)
#define newline_num (10)

#define TRUE (1)
#define FALSE (0)