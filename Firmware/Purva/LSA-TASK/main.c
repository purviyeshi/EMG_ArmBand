#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_err.h"
#include "lsa_pin_def.h"

#define DEFAULT_VREF 1100

#define TAG "LSA"

#define SENSOR_CHANNEL_1 ADC1_CHANNEL_4
#define SENSOR_CHANNEL_2 ADC1_CHANNEL_7
#define SENSOR_CHANNEL_3 ADC1_CHANNEL_6
#define SENSOR_CHANNEL_4 ADC1_CHANNEL_3
#define SENSOR_CHANNEL_5 ADC1_CHANNEL_0





/** 
esp_err_t CONFIG_ADC_WIDTH_CHANNEL()
{	
	adc1_config_width(ADC_WIDTH_BIT_12);                                              // 12-bit width
	
	adc1_config_channel_atten(ADC_CHANNEL_4, ADC_ATTEN_DB_11);       // Configure ADC Characteristics
	adc1_config_channel_atten(ADC_CHANNEL_7, ADC_ATTEN_DB_11);
	adc1_config_channel_atten(ADC_CHANNEL_6, ADC_ATTEN_DB_11);
	adc1_config_channel_atten(ADC_CHANNEL_3, ADC_ATTEN_DB_11);
	adc1_config_channel_atten(ADC_CHANNEL_0, ADC_ATTEN_DB_11);
	
	// return ESP_OK;
}
**/


void configure_adc() {
    adc1_config_width(ADC_WIDTH_12Bit); // Configure the ADC width to 12 bits
    adc1_config_channel_atten(SENSOR_CHANNEL_1, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(SENSOR_CHANNEL_2, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(SENSOR_CHANNEL_3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(SENSOR_CHANNEL_4, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(SENSOR_CHANNEL_5, ADC_ATTEN_DB_11);
}






/**esp_err_t CALIBRATE_ADC_VALUE()
{
	esp_adc_cal_characteristics_t *adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, ESP_ADC_CAL_VAL_DEFAULT_VREF, adc_chars);
}
**/


void app_main()
{
	// CONFIG_ADC_WIDTH_CHANNEL();
	configure_adc();
	
	// CALIBRATE_ADC_VALUE();
	esp_adc_cal_characteristics_t *adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_12Bit, ESP_ADC_CAL_VAL_DEFAULT_VREF, adc_chars);
	
	
	uint32_t sensor_values[5];
	
	while (1) 
	{
        	for (int i = 0; i < 5; i++) 
        	{
        	sensor_values[i] = adc1_get_raw((adc1_channel_t)(SENSOR_CHANNEL_1 + i));          // adc1_get_raw(ADC_CHANNEL_4);        	
        	}

        	// Display the array of ADC values using ESP_LOGI
        	// ESP_LOGI(TAG, "ADC Values:");
        	// for (int j = 0; j < 5; j++) 
        	// {
                // ESP_LOGI(TAG, "Sensor %d: %d", i+1, sensor_values[i]);
                
                ESP_LOGI(TAG, "LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d \t LSA_5: %d", sensor_values[0], sensor_values[1], sensor_values[2], sensor_values[3], sensor_values[4]);
                // vTaskDelay(10 / portTICK_PERIOD_MS);
        	//}
	}
}

