#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "std_types.h"

#define LIMIT_SWITCH_DOWN_INTERRUPT_TRIGGERED()	(GPIO_PORTA_MIS_R & 0x40)

#define LIMIT_SWITCH_DOWN_CLEAR_INTERRUPT_FLAG()	(GPIO_PORTA_ICR_R |= (1<<6))


void limit_switch_down_init(void);


boolean limit_switch_down_pressed(void);

