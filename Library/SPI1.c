#include <SPI1.h>


void SPI1_Init() {
	__HAL_RCC_GPIOA_CLK_ENABLE();

	uint32_t* GPIOA_MODER = (uint32_t*)0x40020000;
	*GPIOA_MODER &= ~(0b111111 << 10);
	*GPIOA_MODER |= (0b10 << 10) | (0b10 << 12) | (0b10 << 14);

	uint32_t* GPIOA_AFRL = (uint32_t*)0x40020020;
	*GPIOA_AFRL &= ~(0xfff << 20);
	*GPIOA_AFRL |= (5 << 20) | (5 << 24) | (5 << 28);

	__HAL_RCC_GPIOE_CLK_ENABLE();

	uint32_t* GPIOE_MODER = (uint32_t*)0x40021000;
	*GPIOE_MODER &= ~(0b11 << 6);
	*GPIOE_MODER |= (0b01 << 6);

	__HAL_RCC_SPI1_CLK_ENABLE();

	uint32_t* SPI1_CR1 = (uint32_t*)0x40013000;
	*SPI1_CR1 &= ~(0x3FF << 0);
	*SPI1_CR1 |= (0b100 << 3);
	*SPI1_CR1 |= (1 << 2) | (1 << 8) | (1 << 9) | (1 << 6);
}

uint8_t SPI1_SensorRead(uint8_t cmd) {
	uint32_t* SPI1_SR = (uint32_t*)0x40013008;
	uint16_t* SPI1_DR = (uint16_t*)0x4001300c;

	uint16_t temp = 0;

	uint32_t* GPIOE_ODR = (uint32_t*)0x40021014;
	*GPIOE_ODR &= ~(1 << 3);

	while (((*SPI1_SR >> 1) & 1) != 1);
	*SPI1_DR = cmd | (1 << 7);
	while (((*SPI1_SR >> 7) & 1) == 1);
	while (((*SPI1_SR >> 0) & 1) != 1);
	temp = *SPI1_DR;

	while (((*SPI1_SR >> 1) & 1) != 1);
	*SPI1_DR = 0xff;
	while (((*SPI1_SR >> 7) & 1) == 1);
	while (((*SPI1_SR >> 0) & 1) != 1);
	temp = *SPI1_DR;

	*GPIOE_ODR |= (1 << 3);

	return temp;
}

void SPI1_SensorWrite(uint8_t cmd, uint8_t data) {
	uint32_t* SPI1_SR = (uint32_t*)0x40013008;
	uint32_t* SPI1_DR = (uint32_t*)0x4001300c;

	uint32_t* GPIOE_ODR = (uint32_t*)0x40021014;
	*GPIOE_ODR &= ~(1 << 3);

	uint32_t temp = 0;

	while (((*SPI1_SR >> 1) & 1) != 1);
	*SPI1_DR = cmd | (0 << 7);
	while (((*SPI1_SR >> 7) & 1) == 1);
	while (((*SPI1_SR >> 0) & 1) != 1);
	temp = *SPI1_DR;

	while (((*SPI1_SR >> 1) & 1) != 1);
	*SPI1_DR = data;
	while (((*SPI1_SR >> 7) & 1) == 1);
	while (((*SPI1_SR >> 0) & 1) != 1);
	temp = *SPI1_DR;

	*GPIOE_ODR |= (1 << 3);
}
