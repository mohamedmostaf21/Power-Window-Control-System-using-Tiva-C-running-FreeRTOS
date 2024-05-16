#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "std_types.h"

#define PUSH_BUTTON_UP_PASSENGER_INTERRUPT_TRIGGERED()	(GPIO_PORTD_MIS_R & 0x04)

#define PUSH_BUTTON_UP_PASSENGER_CLEAR_INTERRUPT_FLAG()	(GPIO_PORTD_ICR_R |= (1<<2))


void push_button_up_passenger_init(void);


boolean push_button_up_passenger_pressed(void);

