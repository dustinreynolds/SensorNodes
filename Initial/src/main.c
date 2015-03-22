/*
 * main.c
 *
 *  Created on: Mar 21, 2015
 *      Author: Dustin
 */

#include "stm32l1xx.h"
#include "init.h"
#include "uart.h"
#include "button.h"
#include "led.h"

int main(void)
{
	init_RCC_Configuration();

	init_GPIO_Configuration();

	uart_Configuration(UART_POLLING);

	button_init();

	led_init();

	uart_OutString("Welcome to Nucleo L152RE\r\n");

	while(1){

		uart_OutString("Switching to interrupt mode\r\n");

		uart_switch_mode(UART_INTERRUPT_RX);
		led_toggle();
		uint32_t i = 0;
		for (i = 0; i<8000000; i++);  //roughly 10 second delay

		uart_OutString("Switching to polling mode\r\n");

		uart_switch_mode(UART_POLLING);
		led_toggle();
		for (i = 0; i < 800000; i++){
			uint16_t Data;

			/*Only get a character if it is ready*/
			if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET){

				Data = USART_ReceiveData(USART2); // Collect Char

				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty

				USART_SendData(USART2, Data); // Echo Char
			}
		}

	}
}
