#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include <driver/rtc_io.h>
#include "soc/rtc_cntl_reg.h"
#include "esp32/ulp.h"
#include <ulp_main.h>

//Binarios embarcados diretamente na flash
extern const uint8_t ulp_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_bin_end[] asm("_binary_ulp_main_bin_end");

void app_main(void)
{
    //Inicializa o LED (GPIO2) no dominio RTC
    rtc_gpio_init(GPIO_NUM_2);
    rtc_gpio_set_direction(GPIO_NUM_2, RTC_GPIO_MODE_OUTPUT_ONLY);

    //Carrega e executa o binario do ULP
    ulp_load_binary(0, ulp_bin_start, (ulp_bin_end - ulp_bin_start) / sizeof(uint32_t));
    ulp_run((&ulp_main - RTC_SLOW_MEM));

    //Dorme por 1 dia, enquanto o ULP continua executando normalmente
    uint64_t sleep_time = 86400*1000000;
    esp_sleep_enable_timer_wakeup(sleep_time);
    esp_deep_sleep_start();
}