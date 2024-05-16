#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "std_types.h"

#define OBSTACLE_DETECTION_INTERRUPT_TRIGGERED() (GPIO_PORTB_MIS_R & 0x02)

#define OBSTACLE_DETECTION_CLEAR_INTERRUPT_FLAG()	(GPIO_PORTB_ICR_R   |= (1<<1)) 


void obstacle_detection_init(void);


boolean obstacle_detection_detected(void);

