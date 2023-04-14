#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

void app_main(void)
{
    int32_t raw_value;
    adc2_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

    while(1) {
        adc2_get_raw(ADC1_CHANNEL_6, ADC_WIDTH_BIT_12, &raw_value);
        printf("adc raw: %ld\n", raw_value);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
