#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>
#include "driver/uart.h"


void app_main(void)
{
    uart_config_t cfg;
    cfg.baud_rate = 115200;
    cfg.data_bits = UART_DATA_8_BITS;
    cfg.parity = UART_PARITY_DISABLE;
    cfg.stop_bits =  UART_STOP_BITS_1;
    cfg.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    cfg.source_clk = UART_SCLK_APB;

    uart_param_config(UART_NUM_1, &cfg);
    uart_set_pin(UART_NUM_1, 22, 23, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, 256, 0, 0, NULL, 0);


    uint8_t inp[32];
    char out[16] = "ESP32 UART";
    while (1)
    {
        uart_write_bytes(UART_NUM_1, out, strlen(out));

        memset(inp, 0, sizeof(inp));
        if (uart_read_bytes(UART_NUM_1, inp, sizeof(inp)-1, pdMS_TO_TICKS(1000)))
        {
            ESP_LOGI(__func__, "UART received: [%s]", (char*)inp);
        }
    }
}