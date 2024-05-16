#include "obstacle_detection.h"

#define GPIO_PORTB_PRIORITY_MASK      0xFFFF1FFF
#define GPIO_PORTB_PRIORITY_BITS_POS  13
#define GPIO_PORTB_INTERRUPT_PRIORITY 5


void obstacle_detection_init(void)
{
	SYSCTL_RCGCGPIO_R|= 0x02;
	while((SYSCTL_PRGPIO_R & 0x02)!= 0x02);
	GPIO_PORTB_LOCK_R   = 0x4C4F434B;   /* Unlock the GPIO_PORTB_CR_REG */
	GPIO_PORTB_AMSEL_R &= ~(1<<1);
	GPIO_PORTB_DIR_R &=~ (1<<1);
	GPIO_PORTB_DEN_R |= (1<<1);
	GPIO_PORTB_PUR_R   |= (1<<1);
	GPIO_PORTB_IS_R    &= ~(1<<1);      /* PB0 detect edges */
  GPIO_PORTB_IBE_R   &= ~(1<<1);      /* PD1 will detect a certain edge */
  GPIO_PORTB_IEV_R   &= ~(1<<1);      /* PD1 will detect a falling edge */
  GPIO_PORTB_ICR_R   |= (1<<1);       /* Clear Trigger flag for PB0 (Interupt Flag) */
  GPIO_PORTB_IM_R    |= (1<<1);       /* Enable Interrupt on PB0 pin */
  /* Set GPIO PORTB priotiy as 2 by set Bit number 15, 14 and 13c with value 2 */
  NVIC_PRI0_R = (NVIC_PRI0_R & GPIO_PORTB_PRIORITY_MASK) | (GPIO_PORTB_INTERRUPT_PRIORITY<<GPIO_PORTB_PRIORITY_BITS_POS); 
  NVIC_EN0_R |= 0x00000002;   /* Enable NVIC Interrupt for GPIO PORTB by setting bit number 1 in EN0 Register */
	
}


boolean obstacle_detection_detected(void)
{
	return !(GPIO_PORTB_DATA_R & 0x02);
	
}


