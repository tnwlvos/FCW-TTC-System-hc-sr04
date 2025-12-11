#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include <pthread.h>

#include "sensors/ultrasonic.h"
#include "logic/systemstate.h"
#include "output/lcd.h"
#include "output/buzzer.h"
#include "logic/fcw_logic.h"

#include "../OutputCtrl.h"



#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line


int main(void)
    {printf("A\n"); fflush(stdout);
    if (wiringPiSetup() == -1) { printf("A1 FAIL\n"); fflush(stdout); exit(1); }
    
    printf("B\n"); fflush(stdout);
    SystemState_Init();
    printf("C\n"); fflush(stdout);

    Ultrasonic_Init();
    printf("D\n"); fflush(stdout);

    Buzzer_Init();
    printf("E\n"); fflush(stdout);

    lcd_init();
    printf("F\n"); fflush(stdout);

    ClrLcd(); // clear LCD
    pthread_t pth;

    if (pthread_create(&pth, NULL, Ultrasonic_thread, NULL) != 0) {
        printf("Failed to create ultrasonic thread\n");
        exit(1);
    }

    pthread_detach(pth);
    while (1)  
     {
        if(sdv_sys.distance_flag == false){
            delay(5);
            continue; // Wait for new distance value
        }
        fcw_update();
        lcdLoc(LINE1);
        ClrLcd();
        typeln("Dist(cm):");
        typeFloat(sdv_sys.distance_cm);
        lcdLoc(LINE2);
        typeln("FCW:");
        typeln(fcw_state_to_string());
        
        typeFloat(sdv_sys.speed_cms);

        sdv_sys.last_distance_cm = sdv_sys.distance_cm;
        sdv_sys.distance_flag = false; // Reset flag
        Buzzer_Update();
    }
    return 0;
}