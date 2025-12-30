#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define LED_PIN 1
#define BUTTON_PIN 2

#define LED_ON 0
#define LED_OFF 1


void app_main(void)
{
	// Configure LED pin as output
	gpio_config_t io_conf = {};
	io_conf.pin_bit_mask = (1ULL << LED_PIN);
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&io_conf);
	gpio_set_level(LED_PIN, LED_OFF); // Initialize LED to OFF

	// Configure button pin as input with internal pull-up
	io_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&io_conf);

    // Led state variable
    bool led_state = LED_OFF;

    printf("Press the button to toggle the LED state.\n");

    while (true) {
        if (gpio_get_level(BUTTON_PIN) == 0) { // Button pressed
            vTaskDelay(pdMS_TO_TICKS(50)); // Debounce delay
            while (gpio_get_level(BUTTON_PIN) == 0) { // Wait for button release
                vTaskDelay(pdMS_TO_TICKS(10));
            }

            led_state = !led_state; // Toggle LED state
            gpio_set_level(LED_PIN, led_state);
            printf("Key pressed! LED is now %s\n", led_state ? "OFF" : "ON");
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // Polling delay
    }
}
