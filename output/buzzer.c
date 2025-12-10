#include <wiringPi.h>
#include <stdlib.h>

#include <stdio.h>
#include "../../OutputCtrl.h"
#include <softTone.h>
#define BUZZER_PIN 21


volatile int buzzer_mode = 0; 



void Buzzer_Update(void)
{
    if (buzzer_mode == 2)          // DANGER → 연속 톤
    {
        softToneWrite(BUZZER_PIN, 1200);  // 1.2kHz (자동차 느낌)
    }
    else if (buzzer_mode == 1)     // WARNING → 삐삐 패턴
    {
        softToneWrite(BUZZER_PIN, 1000);
        delay(120);
        softToneWrite(BUZZER_PIN, 0);
        delay(180);
    }
    else                            // SAFE
    {
        softToneWrite(BUZZER_PIN, 0);
    }
}


void Buzzer_Init(void)
{
    // wiringPiSetupGpio();
    // OC_Init();
    // OC_SelectIO(1); // Use GPIO pins
    pinMode(BUZZER_PIN, OUTPUT);
    softToneCreate(BUZZER_PIN);   // softTone 시작
    softToneWrite(BUZZER_PIN, 0); // 초기 무음


}
void Buzzer_beep_warning(void)  {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100); // Beep for 100 ms
    digitalWrite(BUZZER_PIN, LOW);
}
void Buzzer_beep_danger(void)  {
    digitalWrite(BUZZER_PIN, HIGH);  
    delay(10);  
 }
void Buzzer_off(void)  {
    digitalWrite(BUZZER_PIN, LOW);
}
