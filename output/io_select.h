#ifndef IO_SELECT_H
#define IO_SELECT_H
#include <pthread.h>
extern pthread_mutex_t io_sel_mutex;
void IOSEL_TM1637(void);
void IOSEL_US(void);

#endif // IO_SELECT_H