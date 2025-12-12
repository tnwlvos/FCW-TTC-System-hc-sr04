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
#include "output/led.h"
#include "output/io_select.h"
#include "output/tm1637.h"
#include "../OutputCtrl.h"


#define TM_CLK_BCM  12 // GPIO pin for TM1637 CLK
#define TM_DIO_BCM  6 // GPIO pin for TM1637 DIO
#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line
pthread_mutex_t io_sel_mutex = PTHREAD_MUTEX_INITIALIZER;


// static void* TM_display_thread(void* arg) {
//     (void)arg;

//     while (1) {
//         // 거리값 스냅샷 (락 필요 없음: float 1개 읽기 정도는 대체로 OK)
//         float ttc = sdv_sys.ttc;

//         // 너무 큰 값은 TMshowFloat 범위에 맞춰 적당히 클램프(너 TMshowFloat이 99.99까지)
//         if (ttc > 99.99f) ttc = 99.99f;
//         if (ttc < -99.99f) ttc = -99.99f;

//         // 표시(내부에서 io_sel_mutex + IOSEL_TM1637 걸어둔 safe wrapper 사용 가정)
//         TMshowFloat_safe(ttc);

//         delay(100); // 100ms마다 갱신
//     }
//     return NULL;
// }

int main(void)
    {
    if (wiringPiSetup() == -1) { printf("A1 FAIL\n"); fflush(stdout); exit(1); }  
    SystemState_Init();
    Ultrasonic_Init();
    TMsetup(TM_CLK_BCM, TM_DIO_BCM);
    TMsetBrightness(1);   // 0~7
    Buzzer_Init();
    lcd_init(); 
    led_init();
    ClrLcd(); // clear LCD
    lcdLoc(LINE1);
    typeln("BOOT OK");
    pthread_t pth_us;

    if (pthread_create(&pth_us, NULL, Ultrasonic_thread, NULL) != 0) {
        printf("Failed to create ultrasonic thread\n");
        exit(1);
    }
    

    
    static unsigned int tm_prev = 0;
    while (1)  
     {
        if(sdv_sys.distance_flag == false){
            delay(5);
            continue; // Wait for new distance value
        }
        fcw_update();
        Buzzer_Update();
        led_set_state();
        lcdLoc(LINE1);
        ClrLcd();
        typeln("Dist(cm):");
        typeFloat(sdv_sys.distance_cm);
        lcdLoc(LINE2);
        typeln("vel(cm/s):");
        // typeln(fcw_state_to_string());
        
        typeFloat(sdv_sys.speed_cms);
        // unsigned int now = millis();
        // if (now - tm_prev >= 500) {
        //     tm_prev = now;
        TMshowFloat_safe(sdv_sys.ttc);
        // }
        delay(10);
       
        sdv_sys.distance_flag = false; // Reset flag
        
    }
    return 0;
}