#include <UART2_CP2102.h>

char receiveData[32] = {0};

void DMA_UART_Rx_Init() {
	uint32_t* UART2_CR3 = (uint32_t*)0x40004414;
	//set DMA enable receiver
	*UART2_CR3 &= ~(1 << 6);
	*UART2_CR3 |= (1 << 6);

	__HAL_RCC_DMA1_CLK_ENABLE();
	uint32_t* DMA_S7CR = (uint32_t*)0x400260b8;
	uint32_t* DMA_S7NDTR = (uint32_t*)0x400260bc;
	uint32_t* DMA_S7PAR = (uint32_t*)0x400260c0;
	uint32_t* DMA_S7M0AR = (uint32_t*)0x400260c4;
	*DMA_S7NDTR = sizeof(receiveData);
	*DMA_S7PAR = 0x40004404;
	*DMA_S7M0AR = (uint32_t*)receiveData;
	//bit 25: channel selection
	//bit 10: memory increment mode
	//bit 0: stream enable
	*DMA_S7CR &= ~(0b111 << 25) & ~(1 << 10) & ~(1 << 0);
	*DMA_S7CR |= (0b110 << 25) | (1 << 10) | (1 << 0);
}

void UART_Rx_interrput_Init() {
	uint32_t* UART2_CR1 = (uint32_t*)0x4000440c;
	//bit 5: RXNE interrupt enable
	*UART2_CR1 &= ~(1 << 5);
	*UART2_CR1 |= (1 << 5);

	//vector table
	uint32_t* ISER1 = (uint32_t*)0xE000E104;
	*ISER1 &= ~(1 << (38-32));
	*ISER1 |= (1 << (38-32));
}

void UART_Init() {
	//PA2 - PA3 - mode: alternate function:
	__HAL_RCC_GPIOA_CLK_ENABLE();
	uint32_t* GPIOA_MODER = (uint32_t*)0x40020000;
	*GPIOA_MODER &= ~(0b1111 << 4);
	*GPIOA_MODER |= (0b10 << 4) | (0b10 << 6);

	//alternate function: AF07
	uint32_t* GPIOA_AFRL = (uint32_t*)0x40020020;
	*GPIOA_AFRL &= ~(0xff << 8);
	*GPIOA_AFRL |= (7 << 8) | (7 << 12);

	//UART2
	__HAL_RCC_USART2_CLK_ENABLE();
	uint32_t* UART2_BRR = (uint32_t*)0x40004408;
	//16 000 000 / (16 * 9600) = 104.16666667
	//0.1666667 * 16 = 2.6666667
	*UART2_BRR = (104 << 4) | 3;

	uint32_t* UART2_CR1 = (uint32_t*)0x4000440c;
	// set CR1 bit 13: UART enable
	//bit 3: transmitter enable
	//bit 2: receiver enable
	*UART2_CR1 &= ~(1 << 13) & ~(0b11 << 2);
	*UART2_CR1 |= (1 << 13) | (1 << 3) | (1 << 2);

//	UART_Rx_interrput_Init();
	DMA_UART_Rx_Init();
}

void UART_SendData(char data) {
	uint32_t* UART2_SR = (uint32_t*)0x40004400;
	uint32_t* UART2_DR = (uint32_t*)0x40004404;

	//bit 7: transmit data register empty
	while (((*UART2_SR >> 7) & 1) != 1);
	*UART2_DR = data;
	//bit 6: transmission complete
	while (((*UART2_SR >> 6) & 1) != 1);

}

char UART_ReadBytes(char *buffer, int size, unsigned int timeout)
{
	uint32_t* UART2_SR = (uint32_t*)0x40004400;
	uint32_t* UART2_DR = (uint32_t*)0x40004404;

	for(int i = 0; i < size; i++)
	{
		unsigned int time = 0;
		//bit 5: read data register not empty (1: ready to be read)
		while(((*UART2_SR >> 5) & 1) != 1)
		{
			if(time++ > timeout)
				return 0;
		}
		buffer[i] = *UART2_DR;
	}

	return 1;
}

int cnt = 0;
void USART2_IRQHandler() {
	uint32_t* UART2_DR = (uint32_t*)0x40004404;
	receiveData[cnt++] = *UART2_DR;
}
