#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

#define EXAMPLE_ADC1_CHAN0 ADC_CHANNEL_6
#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_11

adc_oneshot_unit_handle_t adc1_handle;
adc_oneshot_unit_init_cfg_t init_config1 = {
    .unit_id = ADC_UNIT_1,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};

adc_oneshot_chan_cfg_t config = {
    .atten = EXAMPLE_ADC_ATTEN,
    .bitwidth = ADC_BITWIDTH_12,
};

static int raw_value;

void app_main(void)
{
    //ADC INIT
ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
    //ADC CONFIG
adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config);

while(1) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &raw_value));
    printf("adc raw: %d\n", raw_value);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
    adc_oneshot_del_unit(adc1_handle);
}
