#include "motor.h"



uint16_t pwm_duty_cycle;


void Delay_ms(int time_ms)
{
    int i, j;
    for(i = 0 ; i < time_ms; i++)
        for(j = 0; j < 3180; j++)
            {}  /* excute NOP for 1ms */
}

void pwm_init(void)
{
		/* Clock setting for PWM and GPIO PORT */
    SYSCTL_RCGCPWM_R |= 2;       /* Enable clock to PWM1 module */
    SYSCTL_RCGCGPIO_R |= 0x20;  /* Enable system clock to PORTF */
	  SYSCTL_RCC_R |= (1<<20);    /* Enable System Clock Divisor function  */
    SYSCTL_RCC_R |= 0x000E0000; /* Use pre-divider valur of 64 and after that feed clock to PWM1 module*/

 /* Setting of PF2 pin for M1PWM6 channel output pin */
   	GPIO_PORTF_AFSEL_R |= (1<<2);          /* PF2 sets a alternate function */
    GPIO_PORTF_PCTL_R &= ~0x00000F00; /*set PF2 as output pin */
    GPIO_PORTF_PCTL_R |= 0x00000500; /* make PF2 PWM output pin */
    GPIO_PORTF_DEN_R |= (1<<2);          /* set PF2 as a digital pin */
    
    PWM1_3_CTL_R &= ~(1<<0);   /* Disable Generator 3 counter */
	  PWM1_3_CTL_R &= ~(1<<1);   /* select down count mode of counter 3*/
    PWM1_3_GENA_R = 0x0000008C;  /* Set PWM output when counter reloaded and clear when matches PWMCMPA */
    PWM1_3_LOAD_R = 5000;     /* set load value for 50Hz 16MHz/64 = 250kHz and (250KHz/5000) */
    PWM1_3_CMPA_R = 4999;     /* set duty cyle to to minumum value*/
    PWM1_3_CTL_R = 1;           /* Enable Generator 3 counter */
    PWM1_ENABLE_R = 0x40;      /* Enable PWM1 channel 6 output */
}


void motor_init(void)
{
	SYSCTL_RCGCGPIO_R|= 0x01;
	while((SYSCTL_PRGPIO_R & 0x01)!= 0x01);
	GPIO_PORTA_DIR_R |= (1<<2);
	GPIO_PORTA_DIR_R |= (1<<3);
	GPIO_PORTA_DEN_R |= (1<<2);
	GPIO_PORTA_DEN_R |= (1<<3);
	GPIO_PORTA_DATA_R |= (1<<2);
	GPIO_PORTA_DATA_R &=~ (1<<3);
	
	pwm_init();
	
	start_motor_default_rotation();
}


void set_duty_cycle(uint16_t pwm_duty_cycle_a)
{
	PWM1_3_CMPA_R = pwm_duty_cycle_a;
	
	pwm_duty_cycle = pwm_duty_cycle_a;
	
}

void change_motor_rotation_direction(void)
{
		if(pwm_duty_cycle != 4999) // motor is stopped
		{
					GPIO_PORTA_DATA_R ^= (1<<2);
			GPIO_PORTA_DATA_R ^= (1<<3);
			
			PWM1_3_CMPA_R = pwm_duty_cycle;

		}
}	


void speed_motor_up(void)
{
	
	if(!(pwm_duty_cycle <= 10))
	{
			pwm_duty_cycle -= 10;
	
		PWM1_3_CMPA_R = pwm_duty_cycle;
		
	}
}


void slow_motor_down(void)
{
	if(!(pwm_duty_cycle >= 4990))
	{ 
				pwm_duty_cycle += 10;
	
			PWM1_3_CMPA_R = pwm_duty_cycle;
		
	}
	
}



void stop_motor(void)
{
	pwm_duty_cycle = MAX_DUTY_CYCLE;
	
	PWM1_3_CMPA_R = pwm_duty_cycle;
	
}


void start_motor_default_rotation(void)
{
	pwm_duty_cycle = DEFAULT_DUTY_CYCLE;
	
	PWM1_3_CMPA_R = pwm_duty_cycle;
	
	GPIO_PORTA_DATA_R |= (1<<2);
	GPIO_PORTA_DATA_R &=~ (1<<3);
	
}


void start_motor_up_direction(void)
{
		pwm_duty_cycle = DEFAULT_DUTY_CYCLE;
	
	PWM1_3_CMPA_R = pwm_duty_cycle;
	
	GPIO_PORTA_DATA_R |= (1<<2);
	GPIO_PORTA_DATA_R &=~ (1<<3);
	
}


void start_motor_down_direction(void)
{
	pwm_duty_cycle = DEFAULT_DUTY_CYCLE;
	
	PWM1_3_CMPA_R = pwm_duty_cycle;
	
	GPIO_PORTA_DATA_R &= ~(1<<2);
	GPIO_PORTA_DATA_R |= (1<<3);
}
