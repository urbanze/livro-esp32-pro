#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rmt.h"



void rgb(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t idx = 0;

    for (int8_t i = 7; i > -1; i--)
    {
        uint8_t x = (g >> i) & 1;

        if (x)
        {
            rmt_item32_t bit[] = {{{{14, 1, 12, 0}}}};
            ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
        }
        else
        {
            rmt_item32_t bit[] = {{{{7, 1, 16, 0}}}};
            ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
        }
    }

    for (int8_t i = 7; i > -1; i--)
    {
        uint8_t x = (r >> i) & 1;

        if (x)
        {
            rmt_item32_t bit[] = {{{{14, 1, 12, 0}}}};
            ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
        }
        else
        {
            rmt_item32_t bit[] = {{{{7, 1, 16, 0}}}};
            ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
        }
    }

    for (int8_t i = 7; i > -1; i--)
    {
        uint8_t x = (b >> i) & 1;

        if (x)
        {
            rmt_item32_t bit[] = {{{{14, 1, 12, 0}}}};
            ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
        }
        else
        {
            rmt_item32_t bit[] = {{{{7, 1, 16, 0}}}};
            ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
        }
    }

    rmt_tx_start(RMT_CHANNEL_0, 1);
    rmt_wait_tx_done(RMT_CHANNEL_0, pdMS_TO_TICKS(5));
    ets_delay_us(10);
    rmt_tx_stop(RMT_CHANNEL_0);
}

void app_main(void)
{
    //Inicia o RMT0 no GPIO 18 com o clock de (80MHz / 4)
    rmt_config_t cfg = RMT_DEFAULT_CONFIG_TX(GPIO_NUM_18, RMT_CHANNEL_0);
    cfg.clk_div = 4;

    rmt_config(&cfg);
    rmt_driver_install(cfg.channel, 0, 0);


    //RED
    rgb(255, 0, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    //GREEN
    rgb(0, 255, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));

    //BLUE
    rgb(0, 0, 255);
    vTaskDelay(pdMS_TO_TICKS(1000));

    //OFF
    rgb(0, 0, 0);
}