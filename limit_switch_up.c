#include "limit_switch_up.h"


#define GPIO_PORTA_PRIORITY_MASK      0xFFFFFF1F
#define GPIO_PORTA_PRIORITY_BITS_POS  5
#define GPIO_PORTA_INTERRUPT_PRIORITY 5


void limit_switch_up_init(void)
{
		SYSCTL_RCGCGPIO_R|= 0x01;
	while((SYSCTL_PRGPIO_R & 0x01)!= 0x01);
	GPIO_PORTA_LOCK_R = 0x4C4F434B;
	GPIO_PORTA_AMSEL_R &= ~(1<<7);
	GPIO_PORTA_DIR_R &=~ (1<<7);
	GPIO_PORTA_DEN_R |= (1<<7);
	GPIO_PORTA_PUR_R   |= (1<<7);
	GPIO_PORTA_IS_R    &= ~(1<<7);      /* PD2 detect edges */
  GPIO_PORTA_IBE_R   &= ~(1<<7);      /* PD2 will detect a certain edge */
  GPIO_PORTA_IEV_R   &= ~(1<<7);      /* PD2 will detect a falling edge */
  GPIO_PORTA_ICR_R   |= (1<<7);       /* Clear Trigger flag for PD2 (Interupt Flag) */
  GPIO_PORTA_IM_R    |= (1<<7);       /* Enable Interrupt on PD2 pin */
  /* Set GPIO PORTF priotiy as 2 by set Bit number 29, 30 and 31c with value 2 */
  NVIC_PRI0_R = (NVIC_PRI0_R & GPIO_PORTA_PRIORITY_MASK) | (GPIO_PORTA_INTERRUPT_PRIORITY<<GPIO_PORTA_PRIORITY_BITS_POS); 
  NVIC_EN0_R         |= 0x00000001;   /* Enable NVIC Interrupt for GPIO PORTD by set bit number 3 in EN0 Register */
	
}


boolean limit_switch_up_pressed(void)
{
	return !(GPIO_PORTA_DATA_R & 0x80);
	
}

