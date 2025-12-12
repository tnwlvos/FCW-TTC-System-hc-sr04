#include  <wiringPi.h>
#include <stdint.h>
#include <stdbool.h>
#include "tm1637.h"
#include "io_select.h"
#define TM1637_I2C_COMM1 0x40
#define TM1637_I2C_COMM2 0xC0
#define TM1637_I2C_COMM3 0x80
#define BITDELAY 100
#include <pthread.h>


const uint8_t digitToSegment[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001  // F
};
const uint8_t blank[]={0,0,0,0};
uint8_t m_pinCLK, m_pinDIO, m_brightness;

void TMstartWrite(){
    pinMode(m_pinDIO, OUTPUT);
    delayMicroseconds(BITDELAY);
}
void TMstopWrite(){
    pinMode(m_pinDIO, OUTPUT);
    delayMicroseconds(BITDELAY);
    pinMode(m_pinCLK, INPUT);
    delayMicroseconds(BITDELAY);
    pinMode(m_pinDIO, INPUT);
    delayMicroseconds(BITDELAY);
}
void TMwriteByte(uint8_t b){
    for (uint8_t i = 0; i < 8; i++) {
        pinMode(m_pinCLK, OUTPUT);
        delayMicroseconds(BITDELAY);
        if (b & 0x01) {
            pinMode(m_pinDIO, INPUT);
        } else {
            pinMode(m_pinDIO, OUTPUT);
        }
        delayMicroseconds(BITDELAY);
        pinMode(m_pinCLK, INPUT);
        delayMicroseconds(BITDELAY);
        b >>= 1;
    }
    // Wait for ACK
    pinMode(m_pinCLK, OUTPUT);
    pinMode(m_pinDIO, INPUT);
    delayMicroseconds(BITDELAY);
    pinMode(m_pinCLK, INPUT);
    delayMicroseconds(BITDELAY);
    if(digitalRead(m_pinDIO) == LOW){
        pinMode(m_pinDIO, OUTPUT);
    }
    delayMicroseconds(BITDELAY);
    pinMode(m_pinCLK, OUTPUT);
    delayMicroseconds(BITDELAY);
}
void TMsetup(uint8_t pinCLK, uint8_t pinDIO){
    m_pinCLK = pinCLK;
    m_pinDIO = pinDIO;
    pinMode(m_pinCLK, INPUT);
    pinMode(m_pinDIO, INPUT);
    digitalWrite(m_pinCLK, LOW);
    digitalWrite(m_pinDIO, LOW);
}
void TMsetBrightness(uint8_t brightness){
    m_brightness = ((brightness & 0x7) | 0x08)& 0x0f; // 0 to 7
}

void TMsetSegments(const uint8_t segments[], uint8_t length, uint8_t pos){
    TMstartWrite();
    TMwriteByte(TM1637_I2C_COMM1);
    TMstopWrite();

    TMstartWrite();
    TMwriteByte(TM1637_I2C_COMM2 | (pos & 0x03));
    for (uint8_t i = 0; i < length; i++) {
        TMwriteByte(segments[i]);
    }
    TMstopWrite();

    TMstartWrite();
    TMwriteByte(TM1637_I2C_COMM3 | m_brightness);
    TMstopWrite();
}

void TMclear(){
    TMsetSegments(blank, 4, 0);
}   

void TMshowNumber(int num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos){
    uint8_t digits[4];
    const static int divisors[] ={ 1, 10, 100, 1000 };
    bool leading = true;
    
    for(int8_t k=0; k<4 ;k++){
        int divisor = divisors[4 - 1 - k];
        int d = num / divisor;
        uint8_t digit =0;

        if(d ==0){
            if (leading_zero || !leading || (k==3)) digit = digitToSegment[d];
            else digit = 0;
        }
        else{
            leading = false;
            digit = digitToSegment[d];
            num -= d* divisor;
        }
        digit |= (dots & 0x80);
        dots <<= 1;
        digits[k] = digit;
    }
    TMsetSegments(digits + (4- length), length, pos);   
}

void TMshowFloat(float x){
    const uint8_t minus[] ={64, 64,64,64},zeropoint[] = {0b10111111};
    int x100;
    if(x>99) x=99.99;
    x100= x*(x<0 ? -1000:1000);
    x100= x100/10 + (x100%10 >4);
    if(x100<100){
        TMsetSegments(zeropoint,1,1);
        TMshowNumber(x100, 0b1000000, true, 2, 2);
        TMsetSegments(x<0 ? minus: blank,1,0);

    }
    else if(x<0){
        TMsetSegments(minus,1,0);
        TMshowNumber(x100, 0b1000000, false, 3, 1);
      
    }
    else{
        TMshowNumber(x100, 0b1000000, false, 4, 0);
    }
}


void TMsetSegments_safe(const uint8_t seg[], uint8_t len, uint8_t pos){
    pthread_mutex_lock(&io_sel_mutex);
    IOSEL_TM1637();
    TMsetSegments(seg, len, pos);               
    pthread_mutex_unlock(&io_sel_mutex);
}

void TMshowNumber_safe(int num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos){
    pthread_mutex_lock(&io_sel_mutex);
    IOSEL_TM1637();
    TMshowNumber(num, dots, leading_zero, length, pos);
    pthread_mutex_unlock(&io_sel_mutex);
}

void TMshowFloat_safe(float x){
    pthread_mutex_lock(&io_sel_mutex);
    IOSEL_TM1637();
    TMshowFloat(x);
    pthread_mutex_unlock(&io_sel_mutex);
}

void TMclear_safe(void){
    pthread_mutex_lock(&io_sel_mutex);
    IOSEL_TM1637();
    TMclear();   
    pthread_mutex_unlock(&io_sel_mutex);
}

