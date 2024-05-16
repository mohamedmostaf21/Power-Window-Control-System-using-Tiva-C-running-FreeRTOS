#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "std_types.h"

#define LIMIT_SWITCH_UP_INTERRUPT_TRIGGERED()	(GPIO_PORTA_MIS_R & 0x80)

#define LIMIT_SWITCH_UP_CLEAR_INTERRUPT_FLAG()	(GPIO_PORTA_ICR_R |= (1<<7))


void limit_switch_up_init(void);


boolean limit_switch_up_pressed(void);

