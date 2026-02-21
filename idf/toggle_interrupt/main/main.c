#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "button_led.h"

void app_main(void)
{
	button_led_init();

	while (true) {
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
