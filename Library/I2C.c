#include <I2C.h>

void I2C_Init() {
	__HAL_RCC_GPIOB_CLK_ENABLE();
	uint32_t* GPIOB_MODER = (uint32_t*)0x40020400;
	//MODER: PB6 & PB9
	*GPIOB_MODER &= ~(0b11 << 12) & ~(0b11 << 18);
	*GPIOB_MODER |= (0b10 << 12) | (0b10 << 18);
	//ALTERNATE 4: PB6 & PB9
	uint32_t* GPIOB_AFRL = (uint32_t*)0x40020420;
	*GPIOB_AFRL &= ~(0b1111 << 24);
	*GPIOB_AFRL |= (0b0100 << 24);

	uint32_t* GPIOB_AFRH = (uint32_t*)0x40020424;
	*GPIOB_AFRH &= ~(0b1111 << 4);
	*GPIOB_AFRH |= (0b0100 << 4);


	__HAL_RCC_I2C1_CLK_ENABLE();
	uint32_t* I2C1_CR1 = (uint32_t*)0x40005400;
	uint32_t* I2C1_CR2 = (uint32_t*)0x40005404;
	uint32_t* I2C1_CCR = (uint32_t*)0x4000541c;

	*I2C1_CR1 &= ~(1 << 0); //I2C disable

	//peripheral clock: 16MHz
	*I2C1_CR2 &= ~(0b111111 << 0);
	*I2C1_CR2 |= (0b010110 << 0);

	//set clock: 16MHz/80 = 200KHz
	*I2C1_CCR = 80;

	*I2C1_CR1 |= (1 << 0);//I2C enable
}

uint8_t I2C_Read(uint8_t address, uint8_t cmd) {
	uint32_t* I2C1_CR1 = (uint32_t*)0x40005400;
	uint32_t* I2C1_SR1 = (uint32_t*)0x40005414;
	uint32_t* I2C1_SR2 = (uint32_t*)0x40005418;
	uint32_t* I2C1_DR = (uint32_t*)0x40005410;

	//check busy flag
	while (((*I2C1_SR2 >> 1) & 1) == 1);
	//start bit
	*I2C1_CR1 &= ~(0b111 << 8);
	*I2C1_CR1 |= (1 << 8);
	//check start bit
	while (((*I2C1_SR1 >> 0) & 1) != 1);
	//acceleration address - write
	*I2C1_DR = (address << 1) | 0;
	//check address
	while (((*I2C1_SR1 >> 1) & 1) != 1);
	//clean address flag: read SR1 after that read SR2
	uint32_t temp = *I2C1_SR2;
	//check ACK
	while (((*I2C1_SR1 >> 10) & 1) != 0);
	//write CMD
	*I2C1_DR = cmd;
	//check DR: Byte transfer finished
	while (((*I2C1_SR1 >> 2) & 1) != 1);
	//check ACK
	while (((*I2C1_SR1 >> 10) & 1) != 0);

	//write restart bit
	*I2C1_CR1 |= (1 << 8);
	//check start bit
	while (((*I2C1_SR1 >> 0) & 1) != 1);
	//acceleration address - read
	*I2C1_DR = (address << 1) | 1;
	//check address
	while (((*I2C1_SR1 >> 1) & 1) != 1);
	//clean address flag: read SR1 after that read SR2
	temp = *I2C1_SR2;
	//check data empty
	while (((*I2C1_SR1 >> 6) & 1) != 1);
	//read data
	uint8_t data = *I2C1_DR;
	//check ACK
	while (((*I2C1_SR1 >> 10) & 1) != 0);

	//stop bit
	*I2C1_CR1 |= (1 << 9);

	return data;
}
