/* Project created by: THARIS .S
   Emebedded Systems Internship @Emertxe Information Technologies.
 
   Microwave oven simulation using PicsimLab with PIC16F877A microcontroller. 
 
 */
#include <xc.h>
#include "clcd.h"
#include "microoven.h"
#include "matrix_keypad.h"
#include "timers.h"
#include "main.h"
#pragma config WDTE = OFF

unsigned char screen_flag = POWER_ON_SCREEN ;
unsigned char reset_flag = RESET_NOTHING;
extern unsigned char sec, min;
 
 

void init_config(void) {
    
    init_clcd(); //config clcd
    init_matrix_keypad(); // config keypad
    
    FAN_DDR = 0; //config FAN
    FAN = 0; // initial status of FAN
    init_timer2();
    PEIE = 1;
    GIE = 1;
    
}

void main(void) {
    
    unsigned char key;
    unsigned char start_mode = 0;
    init_config();
    while(1) {
        
        key = read_matrix_keypad(STATE);
        // select the menu based on keypress (operational flag)
        if(screen_flag == MENU_SCREEN) {
             
        if (key == 1) {
            clear_screen();
            screen_flag = MICRO_MODE;
            start_mode = 0;
            clcd_print("Power = 900W", LINE2(2));
            __delay_ms(3000);
            clear_screen();
            reset_flag = RESET_MODE; 
        }
        else if (key == 2) {
            
            clear_screen();
            start_mode = 0;
            screen_flag = GRILL_MODE;
            reset_flag = RESET_MODE;
            
        }
        else if (key == 3) {
            
            clear_screen();
            start_mode = 0;
            screen_flag = CONVENTION_MODE;
            reset_flag = RESET_MODE;
        }
        else if (key == 4) {
            
            clear_screen();
            screen_flag = START_MODE;
            start_mode = 1;
            reset_flag = RESET_MODE;
        }
      }
      if(screen_flag == DISPLAY_TIME) {
          
          if (key == 4) {
             //screen_flag = START; (not using anywhere)
              // Increment the cooking time by 30 sec
              if(start_mode) {
                  
                  sec = sec+30;
                  if(sec > 59) {
                      
                      min++;
                      sec = sec - 60;
                  }
              }
                  
        }
        else if (key == 5) {
            
            screen_flag = PAUSE;
          
        }
        else if (key == 6) {
            
            screen_flag = STOP;
        }
        
          
      } 
      else if (screen_flag == PAUSE) {
          if (key == 4){ // resume the oven
              
              FAN = 1;
              TMR2ON = 1;
              screen_flag = DISPLAY_TIME;
              
          }
      } 
        
        switch(screen_flag) 
        {
            
            case POWER_ON_SCREEN :
                power_on_screen();
           
                screen_flag = MENU_SCREEN;
                clear_screen(); //to clear the clcd
            break;
            case MENU_SCREEN :
                                
                display_menu_screen();
                break;
            case MICRO_MODE :
                set_time(key, reset_flag);
                
                break;
            case DISPLAY_TIME :
                time_display();
                break;
            
            case PAUSE :
                FAN = 0;
                TMR2ON = 0;
                break;
                
            case STOP : 
                FAN = 0;
                TMR2ON = 0;
                clear_screen();
                screen_flag = MENU_SCREEN;
                break;
                
            case GRILL_MODE :
                set_time(key, reset_flag);            
                break;
                
            case CONVENTION_MODE :
                set_temp(key, reset_flag);
                break;
            case START_MODE :
                //display time as 30sec for cooking time
                heat_food(); 
                break;
                                
             
           
        } // Switch
        reset_flag = RESET_NOTHING; 
    } // while (1)
    
} // main
