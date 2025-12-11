#ifndef BUZZER_H
#define BUZZER_H

#include <wiringPi.h>

#define BUZZER_PIN 21
extern volatile int buzzer_mode; 
void Buzzer_Init(void);
void Buzzer_beep_warning(void);
void Buzzer_beep_danger(void);
void Buzzer_off(void);
void Buzzer_Update(void);

#endif // BUZZER_H 