#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "OutputCtrl.h"
#define UC_TRIG_PIN	12
#define UC_ECHO_PIN	6

int main(int argc, char** argv)
{
	float dist , s, e;
	wiringPiSetupGpio();
	OC_Init();
	OC_SelectIO(1);
	pinMode(UC_TRIG_PIN, OUTPUT);
	pinMode(UC_ECHO_PIN, INPUT);

	while(1)
	{
		digitalWrite(UC_TRIG_PIN, 0);
		delay(100);
		digitalWrite(UC_TRIG_PIN, 1);
		delayMicroseconds(10);
		digitalWrite(UC_TRIG_PIN, 0);
		while(digitalRead(UC_ECHO_PIN) == 0)
			s = micros();
		while(digitalRead(UC_ECHO_PIN) == 1)
			e = micros();
		dist = (e - s) / 58;
		printf("distance(cm) : %f\n", dist);
		delay(100);
	}
	return 0;
}
