/*
 * watchdog.h
 *
 *  Created on: Nov 4, 2021
 *      Author: Admin
 */

#ifndef INC_WATCHDOG_H_
#define INC_WATCHDOG_H_
void MX_IWDG_Init(void);
void Watchdog_Refresh(void);
unsigned char Is_Watchdog_Reset(void);
void Watchdog_Counting(void);
void Reset_Watchdog_Counting(void);

void Watchdog_init(void);


#endif /* INC_WATCHDOG_H_ */
