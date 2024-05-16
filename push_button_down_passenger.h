#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "std_types.h"


#define PUSH_BUTTON_DOWN_PASSENGER_INTERRUPT_TRIGGERED() (GPIO_PORTD_MIS_R & 0x02)

#define PUSH_BUTTON_DOWN_PASSENGER_CLEAR_INTERRUPT_FLAG()	(GPIO_PORTD_ICR_R   |= (1<<1)) 

void push_button_down_passenger_init(void);


boolean push_button_down_passenger_pressed(void);

