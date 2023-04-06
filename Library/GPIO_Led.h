#include "main.h"

typedef enum {
	LED_OFF,
	LED_ON
}led_state;

typedef enum {
	LED_12,
	LED_13,
	LED_14,
	LED_15
}led_num;
void Led_Init();
void Led_Ctrl(led_state state, led_num led) ;
