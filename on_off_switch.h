#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "std_types.h"


#define ON_OFF_SWITCH_INTERRUPT_TRIGGERED()	(GPIO_PORTB_MIS_R & 0x01)

#define ON_OFF_SWITCH_CLEAR_INTERRUPT_FLAG()	(GPIO_PORTB_ICR_R |= (1<<0))

void on_off_switch_init(void);


boolean on_off_switch_on(void);


boolean on_off_switch_off(void);
