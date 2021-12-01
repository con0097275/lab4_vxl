/*
 * scheduler.h
 *
 *  Created on: Nov 4, 2021
 *      Author: Admin
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_


void SCH_Init(void);
void SCH_Update(void);
unsigned char SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Delete_Task(const int TASK_INDEX);
void SCH_Go_To_Sleep();
void SCH_Report_Status(void);


#endif /* INC_SCHEDULER_H_ */
