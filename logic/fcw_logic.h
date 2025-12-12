#ifndef FCW_LOGIC_H
#define FCW_LOGIC_H
#include <stdint.h>

void fcw_update(void);

const char* fcw_state_to_string(void);

void fcw_get_relative_speed(void);
float fcw_get_ttc(void);

#endif // FCW_LOGIC_H