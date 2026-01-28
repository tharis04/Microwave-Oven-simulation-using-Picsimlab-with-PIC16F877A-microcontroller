#include <xc.h>
#include "microoven.h"
#include "clcd.h"
#include "matrix_keypad.h" 
#include "main.h"

unsigned char sec, min, pre_heat_time;
extern unsigned char screen_flag;

void power_on_screen(void) 
{
    {
        for (unsigned char i=0;i<16;i++) // creating a bar we use for loop here
        {
            clcd_putch(BAR, LINE1(i)); // bar is printed on the line 1 of the clcd
            __delay_ms(100); // providing a delay of 100microseconds
        }
        clcd_print("Powering ON", LINE2(2)); // dplys in line 2
        clcd_print("Microwave oven", LINE3(2));
        for (unsigned char i=0;i<16;i++) // creating bar
        {
            clcd_putch(BAR, LINE4(i)); // bar in printed on line 4 in clcd
            __delay_ms(100); // providing a delay of 100 microseconds
        }
        __delay_ms(1000); // keeps all the messages for 1 second to hold on the screen
    } 
}

void clear_screen(void) {
    
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
    
}

void display_menu_screen(void) 
{
    
        clcd_print("1. Micro", LINE1(1));
        clcd_print("2. Grill", LINE2(1));
        clcd_print("3. Convention", LINE3(1));
        clcd_print("4. Start", LINE4(1));
    // displaying all the modes here.
     
    
}
void set_time(unsigned char key, unsigned char reset_flag) {
    
    static unsigned char blink_pos = 0, key_count = 0;
    
    if(reset_flag == RESET_MODE) {
        
        blink_pos = 0; 
        key_count = 0;
        key = ALL_RELEASED; 
        min = 0;
        sec = 0;
        reset_flag = RESET_NOTHING;
        
    }
    
    clcd_print("SET TIME (MM:SS)", LINE1(0));
    clcd_print("TIME:", LINE2(0));
    clcd_print("*:CLEAR #:ENTER", LINE4(0));
    
    // The Secs field should blink indicating the field to be changed
    // to convert min and sec into ASCII value to display on clcd
    if (key != '*' && key!= '#' && key != ALL_RELEASED) {
        
        key_count ++;
        if(key_count <= 2) { // update sec
            
            sec = sec * 10 + key ;
            blink_pos = 0;
            
        }
        else if ( key_count <= 4) {
            
            min = min * 10 + key ;
            blink_pos = 1;
        }// update min 
        
    }
    else if(key == '*') {
        
        if(blink_pos) {
            
            min = 0;
            key_count = 2;
            
        }
        else {
            
            sec = 0;
            key_count = 0;
            
        }

    }
    else if (key == '#') { // Start the microwave with entered time
    
    FAN = 1; //turn on the FAN
    
    TMR2ON = 1; //turn on the timer
    
    screen_flag = DISPLAY_TIME;
    clear_screen();
    
}

    if(blink_pos == 0) {
        
        clcd_putch( ' ', LINE2(9));
        clcd_putch( ' ', LINE2(10));
               
    }
    else {
        
        clcd_putch( ' ', LINE2(6));
        clcd_putch( ' ', LINE2(7));
        
    }
    for (unsigned char wait = 50; wait--;);
    
    
    clcd_putch( (sec/10 + '0'), LINE2(9));
    clcd_putch( (sec%10 + '0'), LINE2 (10));
    clcd_putch( ':' , LINE2(8));
    clcd_putch( (min/10 + '0'), LINE2(6));
    clcd_putch( (min%10 + '0'), LINE2(7)); 
   
}

void time_display(void) {
    
    //start stop pause function & time display
    clcd_print("TIME = ", LINE1(1));
    clcd_putch( (sec/10 + '0'), LINE1(12));
    clcd_putch( (sec%10 + '0'), LINE1(13));
    clcd_putch( ':' , LINE1(11));
    clcd_putch( (min/10 + '0'), LINE1(9));
    clcd_putch( (min%10 + '0'), LINE1(10)); 
    clcd_print("4.START/RESUME", LINE2(1));
    clcd_print("5.PAUSE", LINE3(1));
    clcd_print("6.STOP", LINE4(1));
            
    if(min == 0 && sec ==0) {
        
        clear_screen();
        
        TMR2ON = 0;
        FAN = 0;
        clcd_print("TIME UP!!!", LINE2(4));
        clcd_print("Enjoy your meal", LINE3(0));
        __delay_ms(2000);
        clear_screen();
        screen_flag = MENU_SCREEN;
        
    }
}

void set_temp(unsigned char key, unsigned char reset_flag) {
    
    static unsigned char key_count ;
    static unsigned int temp ;
    
    if(reset_flag == RESET_MODE) {
        temp = 0;
        key_count = 0;
        key = ALL_RELEASED;
    }
    
    clcd_print("SET TEMP. <*C>", LINE1(0));
    clcd_print("TEMP:", LINE2(0));
    clcd_print("*:CLEAR #:ENTER", LINE4(0));
    
    // blinking the temperature to indicate reading mode
    clcd_putch( (' '), LINE2(8));
    clcd_putch( (' '), LINE2(9)); 
    clcd_putch( (' '), LINE2(10));
    
    for (unsigned char wait = 50; wait--;);
    
    // convert decimal into ASCII
    clcd_putch( (temp/100 + '0'), LINE2(8));
    clcd_putch( ((temp /10) % 10 + '0'), LINE2(9)); 
    clcd_putch( (temp %10 + '0'), LINE2(10));
    
    //read the temp from User from 0 to 9
    if (key != '*' && key!= '#' && key != ALL_RELEASED) {
        
        if(key_count < 3) {
            
            temp = temp *10 + key;
            key_count++;
        }
    }
    else if (key == '*') { // Clear the entered temperature
        
        temp = 0;
        key_count = 0;
    }
    else if(key == '#') { // Confirm entered temperature
        
        if(temp > 180)
            temp = 180;
        
        //implement pre-heat screen
        clear_screen();
        clcd_print("Pre-Heating", LINE1(2));
        clcd_print("Time Rem.=", LINE3(0));
        clcd_print("s", LINE3(13));
        pre_heat_time = 60;
        TMR2ON = 1;
        
        // display pre_heat_screen till pre_heat_time = 0
        while (pre_heat_time != 0) {
            
            // decimal to ASCII to display it on ASCII
            clcd_putch( (pre_heat_time /100 + '0'), LINE3(10));
            clcd_putch( ((pre_heat_time /10) % 10 + '0'), LINE3(11)); 
            clcd_putch( (pre_heat_time %10 + '0'), LINE3(12));
            
        }
        // turn off the timer and then change the operational flag
        TMR2ON = 0;
        clear_screen();
        screen_flag = MICRO_MODE;
        
        
    }
    
    
}
void heat_food(void) 
{
        
        //start heating food for 30 sec
        sec = 30;
        min = 0;
        FAN = 1;
        TMR2ON = 1;
        screen_flag = DISPLAY_TIME;
    }
