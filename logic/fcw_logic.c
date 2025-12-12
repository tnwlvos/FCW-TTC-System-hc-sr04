#include "fcw_logic.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h> 
#include "systemstate.h"


void fcw_update(void){
    float ttc = fcw_get_ttc();  
    if(ttc < 0.0){
        sdv_sys.fcw_state = FCW_ERROR;
    }
    else if(ttc < 1.0 && ttc >= 0.0){
        sdv_sys.fcw_state = FCW_DANGER;
    }
    
    else if(ttc >= 1.0 && ttc < 3.0){
        sdv_sys.fcw_state = FCW_WARNING;
    }
    else{
        sdv_sys.fcw_state = FCW_SAFE;
    }
    sdv_sys.ttc = ttc;
}

const char* fcw_state_to_string(void){
    switch(sdv_sys.fcw_state){
        case FCW_SAFE:
            return "SAFE";
        case FCW_WARNING:
            return "WARNING";
        case FCW_DANGER:
            return "DANGER";
        case FCW_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

void fcw_get_relative_speed(void){
    static unsigned int t_prev = 0;
    unsigned int t_now = micros();

    if (t_prev == 0) {  // 첫 호출
        t_prev = t_now;
        sdv_sys.last_distance_cm = sdv_sys.distance_cm;
        sdv_sys.last_speed_cms = 0.0f;
        sdv_sys.speed_cms = 0.0f;
        return;
    }

    float dt = (t_now - t_prev) / 1000000.0f;  // seconds
    t_prev = t_now;

    if (dt < 0.02f) dt = 0.02f;  // 안전장치(너무 작으면 폭주)
    if (dt > 0.30f) dt = 0.30f;  // 너무 크면 이상치
    float delta_distance = sdv_sys.distance_cm - sdv_sys.last_distance_cm;
    float v_cms = delta_distance / dt; // cm/s
    const float beta = 0.4; // 필터 계수 (0 < beta < 1)
    sdv_sys.speed_cms= beta * v_cms + (1 - beta) *sdv_sys.last_speed_cms;
    sdv_sys.last_speed_cms = sdv_sys.speed_cms;
    sdv_sys.last_distance_cm = sdv_sys.distance_cm;
    

}

float fcw_get_ttc(void){
    fcw_get_relative_speed();
    // sdv_sys.speed_cms = (float)relative_speed; // Convert cm/ss
    int v_min=0;
    if (sdv_sys.speed_cms >= -v_min){
        return 9999.0; // Object is moving away
    }
    float ttc = sdv_sys.distance_cm / (-sdv_sys.speed_cms);
    return ttc;
}