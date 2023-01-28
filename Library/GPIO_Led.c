#include <GPIO_Led.h>

void Led_Init() {
	__HAL_RCC_GPIOD_CLK_ENABLE();
	//mode: input
	uint32_t* GPIOD_MODER = (uint32_t*)0x40020c00;
	*GPIOD_MODER &= ~(0b11111111 << 24);
	*GPIOD_MODER |= (0b01 << 24);
	*GPIOD_MODER |= (0b01 << 26);
	*GPIOD_MODER |= (0b01 << 28);
	*GPIOD_MODER |= (0b01 << 30);
	//output type register
	uint32_t* GPIOD_OTYPER = (uint32_t*)0x40020c04;
	*GPIOD_OTYPER &= ~(0b1111 << 12);
}

void Led_Ctrl(led_state state, led_num led) {
	//output data
	uint32_t* GPIOD_ODR = (uint32_t*)0x40020c14;
	*GPIOD_ODR &= ~(1 << (12 + led));
	if (state == LED_ON) {
		*GPIOD_ODR |= (1 << (12 + led));
	}
}

void EXTI0_IRQHandler() {
	uint32_t* GPIOA_IDR = (uint32_t*)0x40020010;

	while (((*GPIOA_IDR >> 0) & 1) == 1) {
		Led_Ctrl(LED_ON, LED_14);
	}
	Led_Ctrl(LED_OFF, LED_14);

	uint32_t* PR = (uint32_t*)0x40013c14;
	*PR |= (1 << 0);
}
