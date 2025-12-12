#ifndef TM1637_H
#define TM1637_H

#include <stdint.h>
#include <stdbool.h>

void TMsetup(uint8_t pinCLK, uint8_t pinDIO);
void TMsetBrightness(uint8_t brightness);
void TMsetSegments(const uint8_t segments[], uint8_t length, uint8_t pos);
void TMclear();
void TMshowNumber(int num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos);
void TMshowFloat(float x);
void TMsetSegments_safe(const uint8_t segments[], uint8_t len, uint8_t pos);
void TMshowNumber_safe(int num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos);
void TMshowFloat_safe(float x);
void TMclear_safe();

#endif // TM1637_H