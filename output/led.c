#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../OutputCtrl.h"
#include "../logic/systemstate.h"
#define RED_LED_PIN 18 // GPIO pin for LED
#define GREEN_LED_PIN 4 // GPIO pin for LED
#define YELLOW_LED_PIN 17 // GPIO pin for LED


void led_init(void){
    
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
}

#define LED_GREEN (1<<0)
#define LED_YELLOW (1<<1)
#define LED_RED (1<<2)

void led_set_state(void){
    uint8_t out = 0;

    switch(sdv_sys.fcw_state){
        case FCW_SAFE:    out = LED_GREEN; break;
        case FCW_WARNING: out = LED_YELLOW; break;
        case FCW_DANGER:  out = LED_RED; break;
        case FCW_ERROR:   out = LED_RED; break;
        default:          out = 0; break;
    }
    OC_WriteLED(out);
}