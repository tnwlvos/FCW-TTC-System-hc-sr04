#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	
#include "../../OutputCtrl.h"
#include "../logic/systemstate.h"
#define UC_TRIG_PIN	12
#define UC_ECHO_PIN	6

void Ultrasonic_Init(void)
{
	// WiringPi setup
	// wiringPiSetupGpio();
	wiringPiSetupGpio();
    OC_Init();
	OC_SelectIO(1); // Use GPIO pins

	// Set pin modes
	pinMode(UC_TRIG_PIN, OUTPUT);
	pinMode(UC_ECHO_PIN, INPUT);
}


float Ultrasonic_GetDistance(void){
	float distance;
	
	unsigned int timeout;
	// Trigger the ultrasonic pulse
	// Trigger the ultrasonic pulse
	digitalWrite(UC_TRIG_PIN, LOW);
	delayMicroseconds(5);
	digitalWrite(UC_TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(UC_TRIG_PIN, LOW);

	// Wait for echo to go HIGH
	while(digitalRead(UC_ECHO_PIN) == LOW);

	float startTime = micros();

	// Wait for echo to go LOW
	while(digitalRead(UC_ECHO_PIN) == HIGH);

	float endTime = micros();

	// Calculate distance in cm
	distance = (endTime - startTime) / 58.0;

	

	return distance;
}
float Ultrasonic_filtered(void){
	#define SAMPLE_SIZE 3
	float samples[SAMPLE_SIZE];
	float sum = 0.0;
	float average;
	
	// Collect samples
	for(int i = 0; i < SAMPLE_SIZE; i++) {
		samples[i] = Ultrasonic_GetDistance();
		delay(10); // Small delay between samples
	}
	
	// Calculate average
	for(int i = 0; i < SAMPLE_SIZE; i++) {
		sum += samples[i];
	}
	average = sum / SAMPLE_SIZE;
	
	return average;
}

void *Ultrasonic_thread(void* arg){
	while(1){
		float cm = Ultrasonic_filtered();

        if (cm < 0) {               // 센서 오류
            sdv_sys.distance_cm = sdv_sys.last_distance_cm;  // 이전 값 유지
        } else {
            sdv_sys.distance_cm = (uint16_t)cm;
        }

        sdv_sys.distance_flag = true;
		delay(100); // 100ms 간격
		}
}