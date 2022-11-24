#include <stdio.h>
#include <stdint.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <driver/adc.h>
#include <esp_adc_cal.h>


esp_adc_cal_characteristics_t adc_cal;

void app_main(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12); //Configura o ADC1 para 12b
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); //Configura 11db de atenuacao ao canal ADC1_0
 
    //Inicializa a estrutura de calibracao ao ADC1 para 11db e 12b    
    esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_cal);
 
    if (adc_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        ESP_LOGI(__func__, "Vref eFuse encontrado: %umV", adc_cal.vref);
    }
    else if (adc_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        ESP_LOGI(__func__, "Two Point eFuse encontrado");
    }
    else
    {
        ESP_LOGW(__func__, "Nada encontrado, utilizando Vref padrao: %umV", adc_cal.vref);
    }


    while (1)
    {
        uint32_t voltage = 0;
        for (int i = 0; i < 100; i++)
        {
            voltage += adc1_get_raw(ADC1_CHANNEL_0);
            ets_delay_us(30);
        }
        voltage /= 100;
 
 
        voltage = esp_adc_cal_raw_to_voltage(voltage, &adc_cal); //Converte e calibra o valor lido (RAW) para mV
        ESP_LOGI(__func__, "Read mV: [%u]", voltage); //Mostra a leitura calibrada no Serial Monitor
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}