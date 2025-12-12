#include <pthread.h>
#include "../../OutputCtrl.h"
#include "io_select.h"


void IOSEL_TM1637(void){ OC_SelectIO(0); }
void IOSEL_US(void){     OC_SelectIO(1); }

