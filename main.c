#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "sensors/ultrasonic.h"
#include "output/lcd.h"
#include "output/buzzer.h"
#include "logic/fcw_logic.h"
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include "../OutputCtrl.h"



#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line


int main(void)
{
    if (wiringPiSetup () == -1) exit (1);

    Ultrasonic_Init();
    Buzzer_Init();
    lcd_init(); // setup LCD
    ClrLcd(); // clear LCD
    float distance;
    bool state = FCW_SAFE;
    bool last_state = FCW_SAFE;
   
    while (1)  
     {
        distance = Ultrasonic_filtered();
        lcdLoc(LINE1);
        ClrLcd();
        typeln("Dist(cm):");
        lcdLoc(LINE2);
        typeFloat(distance);

        // fcw_state state = fcw_update(distance);

        if(distance < 10) {
            buzzer_mode = 2;            // Danger 연속음
            lcdLoc(LINE1);
            ClrLcd();
            typeln("DANGER!");
        }
        else if(distance < 20) {
            buzzer_mode = 1;             // Warning 삐삐
            lcdLoc(LINE1);
            ClrLcd();
            typeln("WARNING!");
        }
        else {
            buzzer_mode = 0;             // Safe 무음
        }

    Buzzer_Update();
    }
    return 0;
}