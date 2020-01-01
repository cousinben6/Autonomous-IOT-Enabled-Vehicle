//when defining motor_direction
//we will either be in forward, or reverse:
#define forward         (1)     // motors drive forward
#define reverse         (0)     // motors drive in reverse

//This is every state in the line following state machine.
#define pause           (1)     // give user time to place car down
#define find_line       (2)     // drive at a selected angle until we find the line
#define start_face_line (3)     // ignore black line, turn
#define end_face_line   (4)     // turn, stop when we see black line
#define rev_pulse       (5)     // reverse motors to cut momentum
#define follow_line     (6)     // follow the line
#define turn_right      (7)     // turn 90 degrees away from line
#define to_center       (8)     // drive to center of 16 inch diameter circle
#define done            (9)     // idle state
#define BL_Travel_pause (10)    // pause while traveling black line for 2 seconds
#define white_timer     (11)    // reverse for a standard amoumt of time
#define BL_Circle_Pause (12)    // pause while following a circel in the black line
#define follow_circle   (13)    // black line traveling around a tight circle
#define exit_pause      (14)    // pause before driving away from black line
#define Find_White      (15)    // ignore ir sensors until we find we are on white background
#define rev_pulse_line  (16)    // a pulse in reverse to kill momentum

//this is the delay values associated with each delay found in the line-following state machine.
#define find_white      (700)   // time to find the white backdrop the line following machine runs on
#define rev_delay       (25)    // time to reverse motors in order to stop all momentum
#define face_delay      (50)    // time to turn before we leave the black line
#define rev_pulse_time  (15)    // time to reverse to kill forward momentum
#define two_rotation_time (13000)//time to run in order to make 2 full rotations around an 16 inch diameter circle
#define right_turn_time (200)   // time to make a 90 degree right turn
#define to_center_time  (350)   // time to run to drive 8 inches
#define two_sec         (800)   // about 2 seconds (used for pausing before starting state machine)

#define LED_Refresh_Time (5)    // 1.25ms*40 is every 50 ms
#define BLACK (0)               // Left_Color and Right_Color will display this when they detect the line
#define WHITE (1)               // Left_Color and Right_color will display this when no line is detected
#define disp_increment (50)     // how often to increment a display timer

//these are definitions of the switch case that decides
// what ADC line to process found in Interrupt_ADC.c
#define left_detect (0)         // Left  Infared Detector
#define right_detect (1)        // Right Infared Detector
#define thumb_wheel (2)         // thumb wheel position