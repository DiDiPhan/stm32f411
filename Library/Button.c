#include <Button.h>

void Button_Interrupt() {
	//EXTI0
	uint32_t* SYSCFG_EXTICR1 = (uint32_t*)0x40013808;
	*SYSCFG_EXTICR1 &= ~(0b1111 << 0);
	//Rising enable
	uint32_t* EXTI0_RTSR = (uint32_t*)0x40013c08;
	*EXTI0_RTSR &= ~(1 << 0);
	*EXTI0_RTSR |= (1 << 0);
	//Interrupt mask
	uint32_t* EXTI0_IMR = (uint32_t*)0x40013c00;
	*EXTI0_IMR &= ~(1 << 0);
	*EXTI0_IMR |= (1 << 0);
	//NVIC - bit 6 - ISER0
	uint32_t* ISER0 = (uint32_t*)0xE000E100;
	*ISER0 &= ~(1 << 6);
	*ISER0 |= (1 << 6);
}

void Button_Init() {
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//mode: output
	uint32_t* GPIOA_MODER = (uint32_t*)0x40020000;
	*GPIOA_MODER &= ~(1 << 0);
	//pull-up - pull-down
	uint32_t* GPIOA_PUPDR = (uint32_t*)0x4002000c;
	*GPIOA_PUPDR &= ~(0b11 << 0);

	//Interrupt
	Button_Interrupt();
}

int Button_Read() {
	//input data
	uint32_t* GPIOA_IDR = (uint32_t*)0x40020010;
	if (((*GPIOA_IDR >> 0) & 1) == 1) {
		return 1;
	}
	return 0;

}
