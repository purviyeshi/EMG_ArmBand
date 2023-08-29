#include "driver/adc.h"
#include "esp_err.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_now.h"

#define NUM_SENSORS 5

#define DEFAULT_VREF 1100

static const char *TAG = "READINGS";

void app_main(void)
{
    
    esp_err_t err; // Declare the error variable at the beginning of the function
    
    
    err = adc1_config_width(ADC_WIDTH_BIT_12);            //    Configure ADC width to 12 bits
    
    err = adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);          //       Configure ADC channel attenuation
    err = adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    err = adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    err = adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    err = adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    
    if (err != ESP_OK) {
        // Handle error here
    }
    
   
   //Characterize ADC at particular atten
    esp_adc_cal_characteristics_t *adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    
    
    //Check type of calibration value used to characterize ADC
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("eFuse Vref");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Two Point");
    } else {
        printf("Default");
    }
   
    uint16_t raw_sensor_readings[NUM_SENSORS];          //       Read and store raw values from ADC channels    
    
    const adc1_channel_t sensor_channels[NUM_SENSORS] = {
        ADC1_CHANNEL_4,
        ADC1_CHANNEL_7,
        ADC1_CHANNEL_6,
        ADC1_CHANNEL_3,
        ADC1_CHANNEL_0
    };
       
    while (1) {
        for (int i = 0; i < NUM_SENSORS; i++) 
        {
            raw_sensor_readings[i] = adc1_get_raw(sensor_channels[i]);
        }

        // Display the updated array of raw ADC readings
        
        ESP_LOGI(TAG, "LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d \t LSA_5: %d", raw_sensor_readings[0], raw_sensor_readings[1], raw_sensor_readings[2], raw_sensor_readings[3], raw_sensor_readings[4]);
        
        // Convert the raw sensor readings to a string
        char readings_str[100];  // Adjust the buffer size as needed
        snprintf(readings_str, sizeof(readings_str), "LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d \t LSA_5: %d",raw_sensor_readings[0], raw_sensor_readings[1], raw_sensor_readings[2], raw_sensor_readings[3], raw_sensor_readings[4]);

        vTaskDelay(10 / portTICK_PERIOD_MS); // Add a delay if needed
       
    }
   
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
