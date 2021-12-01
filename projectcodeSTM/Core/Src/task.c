/*
 * task.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Admin
 */
#include "task.h"
#include "main.h"

void function1(){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
//	HAL_Delay(1000);
}
//
void function2(){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
//	HAL_Delay(2000);
}

void function3(){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
//	HAL_Delay(3000);
}

void function4(){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
//	HAL_Delay(5000);
}
void function5(){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
//	HAL_Delay(7000);
}
