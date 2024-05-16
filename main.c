#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include "tm4c123gh6pm.h"
#include "motor.h"
#include "push_button_up_passenger.h"
#include "push_button_up_driver.h"
#include "push_button_down_passenger.h"
#include "push_button_down_driver.h"
#include "on_off_switch.h"
#include "limit_switch_up.h"
#include "limit_switch_down.h"
#include "obstacle_detection.h"


#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 762




/* Enable Exceptions ... This Macro enable IRQ interrupts, Programmble Systems Exceptions and Faults by clearing the I-bit in the PRIMASK. */
#define Enable_Exceptions()    __asm("CPSIE I")

/* Disable Exceptions ... This Macro disable IRQ interrupts, Programmble Systems Exceptions and Faults by clearing the I-bit in the PRIMASK. */
#define Disable_Exceptions()   __asm("CPSID I")

/* Enable Faults ... This Macro enable Faults by clearing the F-bit in the FAULTMASK */
#define Enable_Faults()        __asm("CPSIE F")

/* Disable Faults ... This Macro disable Faults by setting the F-bit in the FAULTMASK */
#define Disable_Faults()       __asm("CPSID F") 

/* Go to low power mode while waiting for the next interrupt */
#define Wait_For_Interrupt()   __asm("WFI")


uint32_t SystemCoreClock = 16000000;


volatile boolean window_locked = FALSE;

volatile boolean motor_stopped = TRUE;

volatile boolean motor_move_up = FALSE;

volatile boolean motor_move_down = FALSE;


xSemaphoreHandle xWindowUpDriverBinarySemaphore;

xSemaphoreHandle xWindowDownDriverBinarySemaphore;

xSemaphoreHandle xWindowUpPassengerBinarySemaphore;

xSemaphoreHandle xWindowDownPassengerBinarySemaphore;

xSemaphoreHandle xWindowLockBinarySemaphore;

xSemaphoreHandle xWindowLimitUpBinarySemaphore;

xSemaphoreHandle xWindowLimitDownBinarySemaphore;

xSemaphoreHandle xWindowObstacleDetectedBinarySemaphore;

xSemaphoreHandle xMutex;

void move_motor_up()
{
		xSemaphoreTake( xMutex, portMAX_DELAY );
	
		start_motor_up_direction();
					
		motor_move_up = TRUE;
	
		motor_move_down = FALSE;
					
		motor_stopped = FALSE;
	
		xSemaphoreGive( xMutex );
	
}


void move_motor_down()
{
		xSemaphoreTake( xMutex, portMAX_DELAY );
		
		start_motor_down_direction();
					
		motor_move_down = TRUE;
	
		motor_move_up = FALSE;
					
		motor_stopped = FALSE;
	
		xSemaphoreGive( xMutex );
	
}


void make_motor_stop()
{
		xSemaphoreTake( xMutex, portMAX_DELAY );
	
		stop_motor();
	
		
		motor_move_up = FALSE;
	
		motor_move_down = FALSE;
					
		motor_stopped = FALSE;
	
		xSemaphoreGive( xMutex );
}




void vApplicationIdleHook( void )
{
	Wait_For_Interrupt();
}

void Delay_MS(unsigned long n)
{
    volatile unsigned long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}



/* GPIO PORTD External Interrupt - ISR */
void GPIOPortD_Handler(void)
{
		portBASE_TYPE xTaskWoken = pdFALSE;
		if(PUSH_BUTTON_UP_PASSENGER_INTERRUPT_TRIGGERED())
		{
				/* 'Give' the semaphore to unblock the task. */
				xSemaphoreGiveFromISR( xWindowUpPassengerBinarySemaphore, &xTaskWoken );
			
				PUSH_BUTTON_UP_PASSENGER_CLEAR_INTERRUPT_FLAG();
			
				portEND_SWITCHING_ISR( xTaskWoken );
		}      
		
		else if(PUSH_BUTTON_DOWN_PASSENGER_INTERRUPT_TRIGGERED())
		{
				/* 'Give' the semaphore to unblock the task. */
				xSemaphoreGiveFromISR( xWindowDownPassengerBinarySemaphore, &xTaskWoken );
				
				PUSH_BUTTON_DOWN_PASSENGER_CLEAR_INTERRUPT_FLAG();
			
				portEND_SWITCHING_ISR( xTaskWoken );
		}
}

static void vWindowUpPassengerTask( void *pvParameters )
{
		xSemaphoreTake( xWindowUpPassengerBinarySemaphore, 0 );
	
	
    for( ;; )
    {
	
        xSemaphoreTake( xWindowUpPassengerBinarySemaphore, portMAX_DELAY );
			
				if(on_off_switch_on())
		{
						
					move_motor_up();
				
					Delay_MS(500 * portTICK_RATE_MS);
				
					if(push_button_up_passenger_pressed() && !window_locked && !motor_stopped)
					{
							while(push_button_up_passenger_pressed() && !window_locked && !motor_stopped)
							{
									
							}
							make_motor_stop();
					}
				}

    }
}

static void vWindowDownPassengerTask( void *pvParameters )
{
		xSemaphoreTake( xWindowDownPassengerBinarySemaphore, 0 );
	
	
    for( ;; )
    {
	
        xSemaphoreTake( xWindowDownPassengerBinarySemaphore, portMAX_DELAY );
			
				if(on_off_switch_on())
				{
					move_motor_down();
				
					Delay_MS(500 * portTICK_RATE_MS);
				
					if(push_button_down_passenger_pressed() && !window_locked && !motor_stopped)
					{
							while(push_button_down_passenger_pressed() && !window_locked && !motor_stopped)
							{
									
							}
							
							make_motor_stop();
					}
				}

    }
}


/* GPIO PORTA External Interrupt - ISR */
void GPIOPortA_Handler(void)
{
		portBASE_TYPE xTaskWoken = pdFALSE;
		if(LIMIT_SWITCH_UP_INTERRUPT_TRIGGERED())
		{
				/* 'Give' the semaphore to unblock the task. */
				xSemaphoreGiveFromISR( xWindowLimitUpBinarySemaphore, &xTaskWoken );			

				LIMIT_SWITCH_UP_CLEAR_INTERRUPT_FLAG();
			
				portEND_SWITCHING_ISR( xTaskWoken );

		}      
		
		else if(LIMIT_SWITCH_DOWN_INTERRUPT_TRIGGERED())
		{
				/* 'Give' the semaphore to unblock the task. */
				xSemaphoreGiveFromISR( xWindowLimitDownBinarySemaphore, &xTaskWoken );		
			
				LIMIT_SWITCH_DOWN_CLEAR_INTERRUPT_FLAG();
			
				portEND_SWITCHING_ISR( xTaskWoken );
		}
}

static void vWindowLimitUpTask( void *pvParameters )
{
		xSemaphoreTake( xWindowLimitUpBinarySemaphore, 0 );
	
	
    for( ;; )
    {
	
        xSemaphoreTake( xWindowLimitUpBinarySemaphore, portMAX_DELAY );
			
				if(motor_move_up)
				{
						make_motor_stop();
				}

    }
}

static void vWindowLimitDownTask( void *pvParameters )
{
		xSemaphoreTake( xWindowLimitDownBinarySemaphore, 0 );
	
	
    for( ;; )
    {
	
        xSemaphoreTake( xWindowLimitDownBinarySemaphore, portMAX_DELAY );
			
			
				if(motor_move_down)
				{
						make_motor_stop();
				}

    }
}


/* GPIO PORTB External Interrupt - ISR */
void GPIOPortB_Handler(void)
{
	
			portBASE_TYPE xTaskWoken = pdFALSE;
			if(ON_OFF_SWITCH_INTERRUPT_TRIGGERED())
			{
				/* 'Give' the semaphore to unblock the task. */
				xSemaphoreGiveFromISR( xWindowLockBinarySemaphore, &xTaskWoken );		
				
				ON_OFF_SWITCH_CLEAR_INTERRUPT_FLAG();
				
				portEND_SWITCHING_ISR( xTaskWoken );
			}      
			
			else if(OBSTACLE_DETECTION_INTERRUPT_TRIGGERED())
			{
					/* 'Give' the semaphore to unblock the task. */
				xSemaphoreGiveFromISR( xWindowObstacleDetectedBinarySemaphore, &xTaskWoken );	
				
				OBSTACLE_DETECTION_CLEAR_INTERRUPT_FLAG();
				
				portEND_SWITCHING_ISR( xTaskWoken );
			}
}


static void vWindowLockTask( void *pvParameters )
{
		xSemaphoreTake( xWindowLockBinarySemaphore, 0 );
	
	
    for( ;; )
    {
	
        xSemaphoreTake( xWindowLockBinarySemaphore, portMAX_DELAY );
			
			
				make_motor_stop();
				

    }
}

static void vWindowObstacleDetectedTask( void *pvParameters )
{
		xSemaphoreTake( xWindowObstacleDetectedBinarySemaphore, 0 );
	
	
    for( ;; )
    {
	
        xSemaphoreTake( xWindowObstacleDetectedBinarySemaphore, portMAX_DELAY );
			
			
				move_motor_down();
			
			
				Delay_MS(500);
			
				make_motor_stop();

    }
}

/* GPIO PORTE External Interrupt - ISR */
void GPIOPortE_Handler(void)
{
		portBASE_TYPE xTaskWoken = pdFALSE;
		if(PUSH_BUTTON_UP_DRIVER_INTERRUPT_TRIGGERED())
		{
				/* 'Give' the semaphore to unblock the task. */
				xSemaphoreGiveFromISR( xWindowUpDriverBinarySemaphore, &xTaskWoken );
			
				PUSH_BUTTON_UP_DRIVER_CLEAR_INTERRUPT_FLAG();
			
				portEND_SWITCHING_ISR( xTaskWoken );
		}      
		
		else if(PUSH_BUTTON_DOWN_DRIVER_INTERRUPT_TRIGGERED())
		{
				/* 'Give' the semaphore to unblock the task. */
				xSemaphoreGiveFromISR( xWindowDownDriverBinarySemaphore, &xTaskWoken );
				
				PUSH_BUTTON_DOWN_DRIVER_CLEAR_INTERRUPT_FLAG();
			
				portEND_SWITCHING_ISR( xTaskWoken );
		}
}

static void vWindowUpDriverTask( void *pvParameters )
{
		xSemaphoreTake( xWindowUpDriverBinarySemaphore, 0 );
	
	
    for( ;; )
    {
	
        xSemaphoreTake( xWindowUpDriverBinarySemaphore, portMAX_DELAY );
			
			
					move_motor_up();
				
					Delay_MS(500 * portTICK_RATE_MS);
				
					if(push_button_up_driver_pressed() && !motor_stopped)
					{
							while(push_button_up_driver_pressed() && !motor_stopped)
							{
									
							}
							make_motor_stop();
					}

    }
}

static void vWindowDownDriverTask( void *pvParameters )
{
		xSemaphoreTake( xWindowDownDriverBinarySemaphore, 0 );
	
	
    for( ;; )
    {
	
        xSemaphoreTake( xWindowDownDriverBinarySemaphore, portMAX_DELAY );
			
			
				move_motor_down();
				
					Delay_MS(500 * portTICK_RATE_MS);
				
					if(push_button_down_driver_pressed() && !motor_stopped)
					{
							while(push_button_down_driver_pressed() && !motor_stopped)
							{
									
							}
							make_motor_stop();
					}

    }
}


void init_window( void )
{
		push_button_up_passenger_init();
	
	push_button_up_driver_init();
	
	push_button_down_passenger_init();
	
	push_button_down_driver_init();
	
	on_off_switch_init();
	
	limit_switch_up_init();
	
	limit_switch_down_init();
	
	obstacle_detection_init();
	
		motor_init();
	
		make_motor_stop();
	
}




int main()
{
	
		vSemaphoreCreateBinary( xWindowUpDriverBinarySemaphore );
	
		vSemaphoreCreateBinary( xWindowDownDriverBinarySemaphore );
	
	
		vSemaphoreCreateBinary( xWindowUpPassengerBinarySemaphore );
	
		vSemaphoreCreateBinary( xWindowDownPassengerBinarySemaphore );
	
		vSemaphoreCreateBinary( xWindowLockBinarySemaphore );
	
		vSemaphoreCreateBinary( xWindowLimitUpBinarySemaphore );
	
		vSemaphoreCreateBinary( xWindowLimitDownBinarySemaphore );
	
		vSemaphoreCreateBinary( xWindowObstacleDetectedBinarySemaphore );
	
		xMutex = xSemaphoreCreateMutex();
	
		init_window();
	
		if(xWindowUpDriverBinarySemaphore != NULL && xWindowDownDriverBinarySemaphore != NULL
			&& xWindowUpPassengerBinarySemaphore != NULL && xWindowDownPassengerBinarySemaphore != NULL
			&& xWindowLockBinarySemaphore != NULL
			&& xWindowLimitUpBinarySemaphore != NULL && xWindowLimitDownBinarySemaphore != NULL
			&& xWindowObstacleDetectedBinarySemaphore != NULL
			&& xMutex != NULL)
		{
			
				xTaskCreate( vWindowUpDriverTask, "Window Up Driver", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
			
				xTaskCreate( vWindowDownDriverTask, "Window Down Driver", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
			
				xTaskCreate( vWindowUpPassengerTask, "Window Up Passenger", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
			
				xTaskCreate( vWindowDownPassengerTask, "Window Down Passenger", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
			
				xTaskCreate( vWindowLimitUpTask, "Window Limit Up", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
			
				xTaskCreate( vWindowLimitDownTask, "Window Limit Down", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
			
				xTaskCreate( vWindowLockTask, "Window Lock", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
			
				xTaskCreate( vWindowObstacleDetectedTask, "Window Obstacle Detected", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
	
				Enable_Exceptions();
	
				/* Start the scheduler so the created tasks start executing. */
				vTaskStartScheduler();
		}
	


	for(;;);
	
}

