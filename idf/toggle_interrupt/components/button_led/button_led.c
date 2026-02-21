#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "button_led.h"

#define LED_PIN 1
#define BUTTON_PIN 2

#define LED_ON 0
#define LED_OFF 1

static const char *TAG = "button_led";

static bool led_state = LED_OFF;

/* ISR for button press */
static void IRAM_ATTR button_isr_handler(void* arg)
{
    static uint32_t last_press = 0;
    uint32_t now = xTaskGetTickCount();

    if ((now - last_press) > pdMS_TO_TICKS(500)) {
        last_press = now;

        led_state = !led_state;
        gpio_set_level(LED_PIN, led_state);
        ESP_EARLY_LOGI(TAG, "Button pressed! LED is now %s", led_state == LED_OFF ? "OFF" : "ON");
    }
}

void button_led_init(void)
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
	io_conf.intr_type = GPIO_INTR_NEGEDGE;
	gpio_config(&io_conf);

    // Install ISR service and add ISR handler for button pin
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);
}
