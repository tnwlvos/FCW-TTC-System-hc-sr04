#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	

#include "../../OutputCtrl.h"
#include "../logic/systemstate.h"
#include "ultrasonic.h"
#include "../output/io_select.h"
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
	pinMode(UC_TRIG_PIN, OUTPUT);
	pinMode(UC_ECHO_PIN, INPUT);
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
	float t0 = micros();
    while(digitalRead(UC_ECHO_PIN) == LOW){
        if(micros() - t0 > 30000) return -1.0f; // 30ms timeout
    }

    float startTime = micros();
    while(digitalRead(UC_ECHO_PIN) == HIGH){
        if(micros() - startTime > 30000) return -1.0f;
    }
	float endTime = micros();

	// Calculate distance in cm
	distance = (endTime - startTime) / 58.0;

	

	return distance;
}
float Ultrasonic_filtered(void){
	float d_now = Ultrasonic_GetDistance();
	if(d_now <= 0.0){
		return -1.0; // Invalid distance
	}
	
	static float prev_dfiltered = -1.0;
	if(prev_dfiltered < 0){
		prev_dfiltered = d_now;
	}

	const float alpha = 0.4; // 필터 계수 (0 < alpha < 1)
	float d_filtered= alpha*d_now + (1 - alpha)*prev_dfiltered;
	prev_dfiltered = d_filtered;
	return d_filtered;
}

void *Ultrasonic_thread(void* arg){
	while(1){
		pthread_mutex_lock(&io_sel_mutex);
		IOSEL_US();
		float cm = Ultrasonic_filtered();
		pthread_mutex_unlock(&io_sel_mutex);

		if (cm >= 0.0f) sdv_sys.distance_cm = cm; 

        sdv_sys.distance_flag = true;
		delay(100); // 100ms 간격
		}
}