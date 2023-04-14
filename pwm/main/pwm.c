#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "driver/ledc.h"

#define EXAMPLE_ADC1_CHAN0 ADC_CHANNEL_6 //CANAL 6 UTILIZADO PARA ENVIAR SEÑAL ANALOGA
#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_6 //ATENUACION

#define LEDC_TIMER LEDC_TIMER_0 // Timer que usaremos
#define LEDC_MODE LEDC_HIGH_SPEED_MODE // Modo de velocidad
#define LEDC_OUTPUT 32// Pin de salida
#define LEDC_CHANNEL LEDC_CHANNEL_0 // Canal de control
#define LEDC_DUTY_RES LEDC_TIMER_12_BIT // Resolucion en bits
#define LEDC_FREQUENCY 5000 // Frecuencia ded funcionamiento
#define LEDC_DUTY 0 // Valor inicial
#define LEDC_HPOINT 0 // Ajuste de fase


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
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT,
        .duty = LEDC_DUTY,
        .hpoint = LEDC_HPOINT

    };
    ledc_channel_config(&ledc_channel);

    //ADC INIT
ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
    //ADC CONFIG
adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config);

while(1) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &raw_value));
    printf("adc raw: %d\n", raw_value);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, raw_value);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);

    vTaskDelay(200 / portTICK_PERIOD_MS);
}
    adc_oneshot_del_unit(adc1_handle);
}
