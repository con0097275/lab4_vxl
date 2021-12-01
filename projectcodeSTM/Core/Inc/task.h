/*
 * task.h
 *
 *  Created on: Nov 4, 2021
 *      Author: Admin
 */

#include "stdint.h"
#ifndef INC_TASK_H_
#define INC_TASK_H_

typedef struct {
    // Pointer to the task (must be a 'void (void)' function)
	void ( * pTask)(void);
	// Delay (ticks) until the function will (next) be run
	uint32_t Delay;
	// Interval (ticks) between subsequent runs.
	uint32_t Period;
	// Incremented (by scheduler) when task is due to execute
	uint8_t RunMe;
	//This is a hint to s5oolve the question below.
	uint32_t TaskID;
} sTask;

// MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCH_MAX_TASKS 			5
#define	NO_TASK_ID				0
sTask SCH_tasks_G[SCH_MAX_TASKS];


void function1();
void function2();
void function3();
void function4();
void function5();

#endif /* INC_TASK_H_ */
