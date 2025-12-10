#ifndef FCW_LOGIC_H
#define FCW_LOGIC_H

typedef enum {
    FCW_SAFE = 0,
    FCW_WARNING,
    FCW_DANGER
} fcw_state;
fcw_state fcw_update(float distance);

const char* fcw_state_to_string(fcw_state state);

float fcw_get_relative_speed(void);
float fcw_get_ttc(void);

#endif // FCW_LOGIC_H