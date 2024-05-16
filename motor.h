#include "tm4c123gh6pm.h"
#include <stdint.h>


#define MAX_DUTY_CYCLE	4999

#define DEFAULT_DUTY_CYCLE	4200



void motor_init(void);


void set_duty_cycle(uint16_t pwm_duty_cycle);


void change_motor_rotation_direction(void);


void speed_motor_up(void);


void slow_motor_down(void);


void stop_motor(void);

void start_motor_default_rotation(void);

void start_motor_up_direction(void);

void start_motor_down_direction(void);
