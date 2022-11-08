#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"


void app_main(void)
{
    gpio_config_t gpio;

    //Configuracoes do LED
    gpio.pin_bit_mask = (1 << 2);
    gpio.mode = GPIO_MODE_OUTPUT;
    gpio.pull_up_en = false;
    gpio.pull_down_en = false;
    gpio.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio);

    //Configuracoes do botao
    gpio.pin_bit_mask = (1 << 0);
    gpio.mode = GPIO_MODE_INPUT;
    gpio.pull_up_en = true;
    gpio.pull_down_en = false;
    gpio.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio);


    while (1)
    {
        //Pisca o LED quando o botao for pressionado
        if (!gpio_get_level(GPIO_NUM_0))
        {
            gpio_set_level(GPIO_NUM_2, 1);
            vTaskDelay(pdMS_TO_TICKS(1000));
            gpio_set_level(GPIO_NUM_2, 0);
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}