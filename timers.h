#define MC__CONTINUTOUS (0x20)
#define TB0CCR0_INTERVAL (625)          //625 every 1.25ms
#define TB0CCR1_INTERVAL (31250)        //31250 every 62.5ms
#define TB0CCR2_INTERVAL (12500)        //12500 every 25ms.
#define debounce_delay (010)            //10 intervals of B0B1vector??

        //the following are bit selections for TB0IV
#define disabled (0x00)                 // bit number for first timer
#define ccr1 (2)                        // bit number for first timer
#define ccr2 (4)                        // bit number for second timer
#define overflow (14)                   // bit number for timer overflow
#define refresh_display (5)             // timer countdown to refresh LCD display

#define TRUE (1)                        
#define WHEEL_PERIOD (20000)            // Max pwm value
#define WHEEL_OFF (0)                   // value to turn pwm off
#define RIGHT_FORWARD_SPEED (TB3CCR1)   // Translation from pwm to port listing
#define LEFT_FORWARD_SPEED (TB3CCR2)    // Translation from pwm to port listing
#define RIGHT_REVERSE_SPEED (TB3CCR3)   // Translation from pwm to port listing
#define LEFT_REVERSE_SPEED (TB3CCR4)    // Translation from pwm to port listing

#define reset (0) 