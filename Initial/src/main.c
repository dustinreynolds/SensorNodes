
#include "stm32l1xx.h"

/**************************************************************************************/

void RCC_Configuration(void)
{
	/* --------------------------- System Clocks Configuration -----------------*/
	/* USART1 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* GPIOA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Enable the GPIO Clock for the pushbutton */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

/**************************************************************************************/

void GPIO_Configuration(void)
{
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		/*-------------------------- GPIO Configuration ----------------------------*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // PA.2 USART2_TX, PA.3 USART1_RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}

	{
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Connect USART pins to AF */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

		//Setup GPIO for user button on PC13
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
}

uint8_t main_readUserButton1(void){
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
}

/**************************************************************************************/

void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE);
}

/**************************************************************************************/

void OutString(char *s)
{
	while(*s)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty

		USART_SendData(USART2, *s++); // Send Char
	}
}

/**************************************************************************************/

int main(void)
{
	RCC_Configuration();

	GPIO_Configuration();

	USART2_Configuration();

	OutString("Welcome to Nucleo L152RE\r\n");

	while(1) // Don't want to exit
	{
		uint16_t Data;

		while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET); // Wait for Char

		Data = USART_ReceiveData(USART2); // Collect Char

		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty

		USART_SendData(USART2, Data); // Echo Char

		if(main_readUserButton1() == 0)
		{
			while(main_readUserButton1() == 0);

			OutString("Button Pressed!\r\n");
		}
	}
}

/**************************************************************************************/

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	while (1)
	{}
}
#endif

/**************************************************************************************/
