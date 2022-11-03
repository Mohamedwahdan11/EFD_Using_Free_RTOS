/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


#define Task_1_PERIOD 50  //task 1 period
#define Task_2_PERIOD 50  //task 2 period
#define Task_3_PERIOD 100 //task 3 period
#define Task_4_PERIOD 20  //task 4 period
#define Task_5_PERIOD 10  //task 5 period
#define Task_6_PERIOD 100 //task 6 period




/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

TaskHandle_t Task_1Handle = NULL;
TaskHandle_t Task_2Handle = NULL;
TaskHandle_t Task_3Handle = NULL;
TaskHandle_t Task_4Handle = NULL;
TaskHandle_t Task_5Handle = NULL;
TaskHandle_t Task_6Handle = NULL;


int task_1_in_time=0,task_1_out_time=0,task_1_total_time;
int task_2_in_time=0,task_2_out_time=0,task_2_total_time;
int task_3_in_time=0,task_3_out_time=0,task_3_total_time;
int task_4_in_time=0,task_4_out_time=0,task_4_total_time;
int task_5_in_time=0,task_5_out_time=0,task_5_total_time;
int task_6_in_time=0,task_6_out_time=0,task_6_total_time;
int system_time=0;
int cpu_load=0;

/*initialize the requird variables for communication between tasks       */

uint8_t switch_1_buffer[10]={'\0'};
uint8_t task_1_event=0;
uint8_t switch_2_buffer[10]={'\0'};
uint8_t task_2_event=0;
uint8_t task_3_buffer[10]={'\0'};
uint8_t task_3_event=0;

/*************************************************************************/

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/


/* Task to be created. */

void Task_1( void * pvParameters )
{
	static uint8_t previous=0;
	uint8_t current=0;
	int XLastWakeTime=0;
	const TickType_t xFrequency = Task_1_PERIOD; //tsk 1 frequency
	 
	vTaskSetApplicationTaskTag( NULL , ( void *)1 );
	
    for( ;; )
    {
			TickType_t xTime = xTaskGetTickCount ();
			current=GPIO_read(PORT_1,PIN0);
			if((current==0)&&(previous==1))
			{
				switch_1_buffer[0]='R';
				switch_1_buffer[1]='i';
				switch_1_buffer[2]='s';
				switch_1_buffer[3]='i';
				switch_1_buffer[4]='n';
				switch_1_buffer[5]='g';
				switch_1_buffer[6]='\0';
				task_1_event=1;
				
			}
			else if((current==1)&&(previous==0))
			{
				switch_1_buffer[0]='F';
				switch_1_buffer[1]='A';
				switch_1_buffer[2]='l';
				switch_1_buffer[3]='l';
				switch_1_buffer[4]='i';
				switch_1_buffer[5]='n';
				switch_1_buffer[6]='g';
				switch_1_buffer[7]='\0';
				task_1_event=1;
				
			}
			previous=current;
			vTaskDelayUntil(&XLastWakeTime,xFrequency);
			GPIO_write(PORT_0,PIN6,PIN_IS_LOW);
			
    }
}

void Task_2( void * pvParameters )
{
	static uint8_t previous=0;
	uint8_t current=0;
	int XLastWakeTime=0;
	const TickType_t xFrequency = Task_2_PERIOD; //tsk 2 frequency
	 
	vTaskSetApplicationTaskTag( NULL , ( void *)2 );
	
    for( ;; )
    {
			TickType_t xTime = xTaskGetTickCount ();
			current=GPIO_read(PORT_1,PIN1);
			if((current==0)&&(previous==1))
			{
				switch_2_buffer[0]='R';
				switch_2_buffer[1]='i';
				switch_2_buffer[2]='s';
				switch_2_buffer[3]='i';
				switch_2_buffer[4]='n';
				switch_2_buffer[5]='g';
				switch_2_buffer[6]='\0';
				task_2_event=1;
				
			}
			else if((current==1)&&(previous==0))
			{
				switch_2_buffer[0]='F';
				switch_2_buffer[1]='A';
				switch_2_buffer[2]='l';
				switch_2_buffer[3]='l';
				switch_2_buffer[4]='i';
				switch_2_buffer[5]='n';
				switch_2_buffer[6]='g';
				switch_2_buffer[7]='\0';
				task_2_event=1;
				
			}
			previous=current;
			
			vTaskDelayUntil(&XLastWakeTime,xFrequency);
			GPIO_write(PORT_0,PIN8,PIN_IS_LOW);
			
    }
}

void Task_3( void * pvParameters )
{
	
	int XLastWakeTime=0;
	const TickType_t xFrequency = Task_3_PERIOD; //tsk 3 frequency
	 
	vTaskSetApplicationTaskTag( NULL , ( void *)3 );
	
    for( ;; )
    {
			TickType_t xTime = xTaskGetTickCount ();
			
		   	task_3_buffer[0]='H';
				task_3_buffer[1]='E';
				task_3_buffer[2]='l';
				task_3_buffer[3]='l';
				task_3_buffer[4]='O';
				task_3_buffer[5]='W';
				task_3_buffer[6]='\0';
				task_3_event=1;
		
			
			vTaskDelayUntil(&XLastWakeTime,xFrequency);
			GPIO_write(PORT_0,PIN8,PIN_IS_LOW);
			
    }
}

void Task_4( void * pvParameters )
{
	
	int XLastWakeTime=0;
	const TickType_t xFrequency = Task_4_PERIOD; //tsk 4 frequency
	 
	vTaskSetApplicationTaskTag( NULL , ( void *)4 );
	
    for( ;; )
    {
			TickType_t xTime = xTaskGetTickCount ();
			
			
			if(task_1_event==1)
			 {
				 xSerialPutChar('\n');
				  vSerialPutString(switch_1_buffer, 10);
				 
						

				 
				 task_1_event=0;
			 }
			 
			 if(task_2_event==1)
			 {
				 xSerialPutChar('\n');
				 vSerialPutString(switch_2_buffer, 10);
				
				 
				 task_2_event=0;
			 }
			 
		   if(task_3_event==1)
			 {
				 xSerialPutChar('\n');
				 vSerialPutString(task_3_buffer, 10);
				
				 
				 task_3_event=0;
			 }
				 
		
			
			vTaskDelayUntil(&XLastWakeTime,xFrequency);
			GPIO_write(PORT_0,PIN8,PIN_IS_LOW);
			
    }
}

void Task_5( void * pvParameters )
{
	int i;
	int XLastWakeTime=0;
	const TickType_t xFrequency = Task_5_PERIOD; //tsk 5 frequency
	 
	vTaskSetApplicationTaskTag( NULL , ( void *)5 );
	
    for( ;; )
    {
			TickType_t xTime = xTaskGetTickCount ();
				
			for(i=0;i<33110;i++)
			{
				i=i;
			}
	
			
			vTaskDelayUntil(&XLastWakeTime,xFrequency);
			GPIO_write(PORT_0,PIN8,PIN_IS_LOW);
			
    }
}

void Task_6( void * pvParameters )
{
		int i;
		int XLastWakeTime=0;
	const TickType_t xFrequency = Task_6_PERIOD; //tsk 6 frequency
	 
	vTaskSetApplicationTaskTag( NULL , ( void *)6 );
    for( ;; )
    {
			TickType_t xTime = xTaskGetTickCount ();
				
			for(i=0;i<79471;i++)
			{
				i=i;
			}
			
			vTaskDelayUntil(&XLastWakeTime,xFrequency);
			GPIO_write(PORT_0,PIN8,PIN_IS_LOW);
    }
}

/*Implement the TickHook*/
void vApplicationTickHook( void )
{
	/*write code here*/
	
	GPIO_write(PORT_0,PIN1,PIN_IS_HIGH);
		
	GPIO_write(PORT_0,PIN1,PIN_IS_LOW);
}
void vApplicationIdleHook( void )
{
	/*write code here*/
	
	GPIO_write(PORT_0,PIN8,PIN_IS_HIGH);
	
}



/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	
    /* Create Tasks here */
		 /* Create the task, storing the handle. */
	xTaskPeriodicCreate(
                    Task_1,       /* Function that implements the task. */
                    "Button_1_Monitor",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Task_1Handle,       /* Used to pass out the created task's handle. */
										Task_1_PERIOD);
		
	xTaskPeriodicCreate(
                    Task_2,       /* Function that implements the task. */
                    "Button_2_Monitor",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Task_2Handle,       /* Used to pass out the created task's handle. */
										Task_2_PERIOD);									

	xTaskPeriodicCreate(
                    Task_3,       /* Function that implements the task. */
                    "Periodic_Transmitter",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Task_3Handle,       /* Used to pass out the created task's handle. */
										Task_3_PERIOD);									

	xTaskPeriodicCreate(
                    Task_4,       /* Function that implements the task. */
                    "UART SEND",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Task_4Handle,       /* Used to pass out the created task's handle. */
										Task_4_PERIOD);									
									
										
	xTaskPeriodicCreate(
                    Task_5,       /* Function that implements the task. */
                    "Task 5",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Task_5Handle,       /* Used to pass out the created task's handle. */
										Task_5_PERIOD);

		xTaskPeriodicCreate(
                    Task_6,       /* Function that implements the task. */
                    "Task 6",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &Task_6Handle,       /* Used to pass out the created task's handle. */
										Task_6_PERIOD);
										
														


	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


