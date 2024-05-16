#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "std_types.h"


#define PUSH_BUTTON_UP_DRIVER_INTERRUPT_TRIGGERED()	(GPIO_PORTE_MIS_R & 0x02)

#define PUSH_BUTTON_UP_DRIVER_CLEAR_INTERRUPT_FLAG()	(GPIO_PORTE_ICR_R |= (1<<1))

void push_button_up_driver_init(void);


boolean push_button_up_driver_pressed(void);

