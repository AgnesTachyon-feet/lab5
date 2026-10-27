#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#define ADC_MIN 0
#define ADC_MAX 1600
#define DEG_MAX 180.0

float adc_to_angle(int adc) {
    if (adc < ADC_MIN) adc = ADC_MIN;
    if (adc > ADC_MAX) adc = ADC_MAX;
    
    return (adc - ADC_MIN) * DEG_MAX / (ADC_MAX - ADC_MIN);
}

void app_main(void) {
    // 1. ตั้งค่าและสร้าง Unit Handle สำหรับ ADC1
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    // 2. ตั้งค่า Channel 6 ให้อ่านค่า 12-bit
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12, 
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config);

    // 3. วนลูปอ่านค่า
    while (1) {
        int adc = 0;
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &adc);
        
        float angle = adc_to_angle(adc);
        printf("ADC = %d  Angle = %.2f deg\n", adc, angle);
        
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}