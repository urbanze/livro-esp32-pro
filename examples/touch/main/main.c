#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/touch_pad.h"


void app_main(void)
{
    touch_pad_init();
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    touch_pad_config(TOUCH_PAD_NUM0, 0);

    while (1)
    {
        uint16_t val = 0;
        touch_pad_read(TOUCH_PAD_NUM0, &val);
        ESP_LOGI(__func__, "Touch: [%d]", val);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}