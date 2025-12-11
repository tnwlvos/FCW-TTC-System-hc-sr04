#ifndef ULTRASONIC_H
#define ULTRASONIC_H



void Ultrasonic_Init(void);
float Ultrasonic_GetDistance(void);
float Ultrasonic_filtered(void);
void *Ultrasonic_thread(void* arg);
#endif // ULTRASONIC_H