#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "button_led.h"

#define LED_PIN GPIO_NUM_1
#define BUTTON_ON_PIN GPIO_NUM_2
#define BUTTON_OFF_PIN GPIO_NUM_42

#define LED_ON 0
#define LED_OFF 1

static const char *TAG = "button_led";

static QueueHandle_t gpio_evt_queue = NULL;

static bool led_state = LED_OFF;

/* ISR for button press */
static void IRAM_ATTR button_isr_handler(void* arg)
{
    gpio_num_t gpio_num = (gpio_num_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}


/* Task handling button events */
static void button_task(void *arg)
{
    static uint32_t last_press = 0;
    gpio_num_t gpio_num;

    while (true) {
        if (xQueueReceive(gpio_evt_queue, &gpio_num, portMAX_DELAY)) {
            uint32_t now = xTaskGetTickCount();

            if ((now - last_press) > pdMS_TO_TICKS(500)) {
                last_press = now;

                if (gpio_num == BUTTON_ON_PIN) {
                    led_state = LED_ON;
                } else if (gpio_num == BUTTON_OFF_PIN) {
                    led_state = LED_OFF;
                } else {
                    return; // Unknown button
                }
                gpio_set_level(LED_PIN, led_state);
                ESP_LOGI(TAG, "Button pressed! LED is now %s", led_state == LED_ON ? "ON" : "OFF");
            }
        }
    }
}

void led_init(void)
{
    // Configure LED pin as output
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << LED_PIN);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    // Initialize LED to OFF
    gpio_set_level(LED_PIN, LED_OFF);
}

void button_init(gpio_num_t button_pin)
{
    // Configure button pin as input with internal pull-up
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << button_pin);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&io_conf);

    // Add ISR handler for button
    gpio_isr_handler_add(button_pin, button_isr_handler, (void*) button_pin);
}

void button_led_init(void)
{
    /* Queue */
    gpio_evt_queue = xQueueCreate(5, sizeof(uint32_t));

    /* Task */
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
    
    ESP_LOGI(TAG, "Initializing button and LED");
    
    // Install ISR service
    gpio_install_isr_service(0);

	// Configure LED
    led_init();

	// Configure buttons
    button_init(BUTTON_ON_PIN);
    button_init(BUTTON_OFF_PIN);

    ESP_LOGI(TAG, "Button and LED initialized");
}
