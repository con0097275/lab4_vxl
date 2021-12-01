/*
 * scheduler.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Admin
 */
#include "scheduler.h"
#include "task.h"
#include "watchdog.h"
#include "timer.h"
#include "main.h"

#define ERROR_SCH_TOO_MANY_TASKS 1
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK	2
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER 3
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START 4
#define ERROR_SCH_LOST_SLAVE 5
#define ERROR_SCH_CAN_BUS_ERROR 6
#define ERROR_I2C_WRITE_BYTE_AT24C64   7
#define ERROR_SCH_CANNOT_DELETE_TASK  8

#define RETURN_ERROR 0
#define RETURN_NORMAL 1

//bien:
int Error_tick_count_G = 0;
int Last_error_code_G = 0;
int Error_code_G = 0;

//bien them:
static sTask temp;
static int numTask = 0;

unsigned char SCH_Delete_Task(int TASK_INDEX) {
unsigned char Return_code;
if (SCH_tasks_G[TASK_INDEX].pTask == 0 || (numTask<=0)) {
	// No task at this location...
	//
	// Set the global error variable
	Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;

	// ...also return an error code
	Return_code = RETURN_ERROR;
} else {
	Return_code = RETURN_NORMAL;
}

for (int i = TASK_INDEX; i < numTask - 1; i++) {
	SCH_tasks_G[i]= SCH_tasks_G[i+1];
}
SCH_tasks_G[numTask - 1].pTask = 0x0000;
SCH_tasks_G[numTask - 1].Delay = 0;
SCH_tasks_G[numTask - 1].Period = 0;
SCH_tasks_G[numTask - 1].RunMe = 0;
numTask -= 1;

return Return_code; // return status
}




unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY,
		unsigned int PERIOD) {
	// Have we reached the end of the list?
	if (numTask == SCH_MAX_TASKS) {
		// Task list is full
		// Set the global error variable
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		// Also return an error code
		return SCH_MAX_TASKS;
	}
	int index=0;
	if (numTask==0) { //neu numtask=0 thi them nhu binh thuong
		SCH_tasks_G[0].pTask = pFunction;
			SCH_tasks_G[0].Delay = DELAY;
			SCH_tasks_G[0].Period = PERIOD;
			SCH_tasks_G[0].RunMe = 0;
			numTask+=1;
			return 0;
	} else {    //neu numtask!=0
		for (int i=0;i<numTask;i++) {	//loop qua tung task, neu delay truoc > delay sau-> delay truoc=delay truoc-delay sau
			if (DELAY> SCH_tasks_G[i].Delay) {
				DELAY-=SCH_tasks_G[i].Delay;
				index+=1;
			} else {   // neu delay truoc < delay sau thi ket thuc vong lap, ta cung xac dinh duoc vi tri can them la index
				break;
			}
		}
		//de danh khoang trong cho vi tri index
		for (int i=numTask;i>index;i--) {
			SCH_tasks_G[i]=SCH_tasks_G[i-1];
		}
	//chen task can them vao vi tri index
	SCH_tasks_G[index].pTask = pFunction;
	SCH_tasks_G[index].Delay = DELAY;
	SCH_tasks_G[index].Period = PERIOD;
	SCH_tasks_G[index].RunMe = 0;
	numTask += 1;
		//tru delay cac task sau vi tri index 1 khoang la task[index].delay
		if (index+1<numTask) {
				SCH_tasks_G[index+1].Delay-=DELAY;
		}
		return index;  //tra ve vi tri vua them task
	}
}



void SCH_Update(void) {
	//lay task[0] ra chay:
	if (SCH_tasks_G[0].pTask) {
		//delay=0 thi flag runme+=1
		if (SCH_tasks_G[0].Delay == 0) {
			SCH_tasks_G[0].RunMe += 1;
			if (SCH_tasks_G[0].Period) {  //neu period !=0 thi cho delay=period -> xoa task[0]->them lai task[0] vua xoa->gan co runme
				SCH_tasks_G[0].Delay = SCH_tasks_G[0].Period;
				temp=SCH_tasks_G[0];
				SCH_Delete_Task(0);
				int i= SCH_Add_Task(temp.pTask, temp.Delay, temp.Period);
				SCH_tasks_G[i].RunMe=temp.RunMe;
			}
		} else {  //neu delay!=0 thi dem nguoc delay
			SCH_tasks_G[0].Delay -= 1;
		}
	}
}



void SCH_Init(void) {
	numTask=0;
	Error_code_G = 0;
	//set all reported error leg turn off
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3 |GPIO_PIN_4 |GPIO_PIN_5 |GPIO_PIN_6 |GPIO_PIN_7, 1);
	//Timer_init();
	//Watchdog_init();
}


void SCH_Dispatch_Tasks(void) {
unsigned char Index;
// Dispatches (runs) the next task (if one is ready)
for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
	if (SCH_tasks_G[Index].RunMe > 0) {
		(*SCH_tasks_G[Index].pTask)(); // Run the task
		SCH_tasks_G[Index].RunMe -= 1; // Reset / reduce RunMe flag
		// Periodic tasks will automatically run again
		// - if this is a 'one shot' task, remove it from the array
		if (SCH_tasks_G[Index].Period == 0) {
			SCH_Delete_Task(Index);
		}
	}
}
// Report system status
	SCH_Report_Status();
// The scheduler enters idle mode at this point
	SCH_Go_To_Sleep();
}


void SCH_Go_To_Sleep() {
//todo: Optional
	HAL_SuspendTick();
	// Enter  the  Sleep  Mode  now
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON ,PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
}

void SCH_Report_Status(void) {
#ifdef SCH_REPORT_ERRORS
	// ONLY APPLIES IF WE ARE REPORTING ERRORS
	// Check for a new error code
	if(Error_code_G != Last_error_code_G) {
		// Negative logic on LEDs assumed
		Error_port->ODR = 255 - Error_code_G; // Error_port is GPIOB declared via main.h
		Last_error_code_G = Error_code_G ;
		if(Error_code_G != 0) {
			Error_tick_count_G = 60000;
		} else {
			Error_tick_count_G = 0 ;
		}
	}else{
		if( Error_tick_count_G != 0) {
			if (--Error_tick_count_G == 0){
				Error_code_G = 0 ; // Reset error code
			}
		}
}
#endif
}
