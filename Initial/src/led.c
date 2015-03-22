/*
 * led.c
 *
 *  Created on: Mar 22, 2015
 *      Author: Dustin
 */

#include "stm32l1xx.h"

void led_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Configure the GPIO pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void led_on(void)
{
	GPIOA->BSRRL = GPIO_Pin_5;
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *         This parameter must be: LED2
  * @retval None
  */
void led_off(void)
{
	GPIOA->BSRRH = GPIO_Pin_5;
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *         This parameter must be: LED2
  * @retval None
  */
void led_toggle(void)
{
  GPIOA->ODR ^= GPIO_Pin_5;
}



