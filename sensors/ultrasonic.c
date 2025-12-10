#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../OutputCtrl.h"

#define UC_TRIG_PIN	12
#define UC_ECHO_PIN	6

void Ultrasonic_Init(void)
{
	// WiringPi setup
	// wiringPiSetupGpio();
	OC_Init();
	OC_SelectIO(1); // Use GPIO pins

	// Set pin modes
	pinMode(UC_TRIG_PIN, OUTPUT);
	pinMode(UC_ECHO_PIN, INPUT);
}
float Ultrasonic_GetDistance(void){
	float distance;
	float startTime, endTime;

	// Trigger the ultrasonic pulse
	digitalWrite(UC_TRIG_PIN, LOW);
	delayMicroseconds(5);
	digitalWrite(UC_TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(UC_TRIG_PIN, LOW);

	// Wait for echo to go HIGH
	while(digitalRead(UC_ECHO_PIN) == LOW);

	// Record the start time
	startTime = micros();

	// Wait for echo to go LOW
	while(digitalRead(UC_ECHO_PIN) == HIGH);

	// Record the end time
	endTime = micros();

	// Calculate distance in cm
	distance = (endTime - startTime) / 58.0;

	return distance;
}
float Ultrasonic_filtered(void){
	#define SAMPLE_SIZE 5
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

