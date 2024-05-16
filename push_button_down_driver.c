#include "push_button_down_driver.h"


#define GPIO_PORTE_PRIORITY_MASK      0xFFFFFF1F
#define GPIO_PORTE_PRIORITY_BITS_POS  5
#define GPIO_PORTE_INTERRUPT_PRIORITY 5


void push_button_down_driver_init(void)
{
		SYSCTL_RCGCGPIO_R|= 0x10;
	while((SYSCTL_PRGPIO_R & 0x10)!= 0x10);
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_AMSEL_R &= ~(1<<2);
	GPIO_PORTE_DIR_R &=~ (1<<2);
	GPIO_PORTE_DEN_R |= (1<<2);
	GPIO_PORTE_PUR_R   |= (1<<2);
	GPIO_PORTE_IS_R    &= ~(1<<2);      /* PD2 detect edges */
  GPIO_PORTE_IBE_R   &= ~(1<<2);      /* PD2 will detect a certain edge */
  GPIO_PORTE_IEV_R   &= ~(1<<2);      /* PD2 will detect a falling edge */
  GPIO_PORTE_ICR_R   |= (1<<2);       /* Clear Trigger flag for PD2 (Interupt Flag) */
  GPIO_PORTE_IM_R    |= (1<<2);       /* Enable Interrupt on PD2 pin */
  /* Set GPIO PORTF priotiy as 2 by set Bit number 29, 30 and 31c with value 2 */
  NVIC_PRI1_R = (NVIC_PRI1_R & GPIO_PORTE_PRIORITY_MASK) | (GPIO_PORTE_INTERRUPT_PRIORITY<<GPIO_PORTE_PRIORITY_BITS_POS); 
  NVIC_EN0_R         |= 0x00000010;   /* Enable NVIC Interrupt for GPIO PORTD by set bit number 3 in EN0 Register */
	
}


boolean push_button_down_driver_pressed(void)
{
	return !(GPIO_PORTE_DATA_R & 0x04);
	
}

