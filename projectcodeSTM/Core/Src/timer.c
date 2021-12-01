/*
 * timer.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Admin
 */


#include "main.h"
#include "timer.h"
#include "scheduler.h"

//timer
static int timer0_counter = 0;
static int timer0_flag = 0;

static int timer1_counter = 0;
static int timer1_flag = 0;

#define TIMER_CYCLE  10    //10ms

int is_flag0(){
	return (timer0_flag==1);
}
int is_flag1(){
	return (timer1_flag==1);
}

void setTimer0(int duration) {
	timer0_counter = duration / TIMER_CYCLE;
	timer0_flag = 0;
}
void setTimer1(int duration) {
	timer1_counter = duration / TIMER_CYCLE;
	timer1_flag = 0;
}

void timer_run() {
	if (timer0_counter > 0) {
		timer0_counter--;
	}
	if (timer0_counter==0) {
		timer0_flag=1;
	}
	if (timer1_counter > 0) {
		timer1_counter--;
	}
	if (timer1_counter == 0) {
		timer1_flag = 1;
	}
}

//10 ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if ( htim->Instance == TIM2) {
		timer_run();
		SCH_Update();
	}
}
