#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"


//Define os GPIOs aos perifericos
#define GPIO_LED 2
#define GPIO_BUTTON 0

void app_main(void)
{
    //Estrutura com a configuracao do GPIO
    gpio_config_t gpio;

    //Configuracoes do LED
    gpio.pin_bit_mask = (1 << GPIO_LED); //Bit do GPIO
    gpio.mode = GPIO_MODE_OUTPUT;        //Modo output
    gpio.pull_up_en = false;             //Pull-up OFF
    gpio.pull_down_en = false;           //Pull-down OFF
    gpio.intr_type = GPIO_INTR_DISABLE;  //Interrupcao OFF
    gpio_config(&gpio); //Aplica configuracoes

    //Configuracoes do botao
    gpio.pin_bit_mask = (1 << GPIO_BUTTON); //Bit do GPIO
    gpio.mode = GPIO_MODE_INPUT;            //Modo input
    gpio.pull_up_en = true;                 //Pull-up ON
    gpio.pull_down_en = false;              //Pull-down OFF
    gpio.intr_type = GPIO_INTR_DISABLE;     //Interrupcao OFF
    gpio_config(&gpio); //Aplica configuracoes


    while (1)
    {
        //Pisca o LED quando o botao for pressionado
        if (gpio_get_level(GPIO_BUTTON) == false)
        {
            gpio_set_level(GPIO_LED, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
            gpio_set_level(GPIO_LED, 0);
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}