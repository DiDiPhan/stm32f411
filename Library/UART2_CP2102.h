#include "main.h"

void UART_Init();

void UART_SendData(char data);

char UART_ReadBytes(char *buffer, int size, unsigned int timeout);

void USART2_IRQHandler();
