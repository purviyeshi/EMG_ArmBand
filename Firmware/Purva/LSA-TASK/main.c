#include <stdio.h>
#include <stdlib.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_err.h"
#include "lsa_pin_def.h"

#include "sdkconfig.h"

#define NUMBER_OF_SAMPLES CONFIG_NUMBER_OF_SAMPLES

#define DEFAULT_VREF 1100

// tested Margin Value Constants for Black and White Surfaces
#define BLACK_MARGIN 4095
#define WHITE_MARGIN 0

// targetted Mapping Value Constants for Tested Margin Values
#define CONSTRAIN_LSA_LOW 0
#define CONSTRAIN_LSA_HIGH 1000


esp_err_t config_adc1();

esp_err_t characterize_adc1();

esp_err_t enable_adc1();

int read_adc(int adc_pin);





typedef union line_sensor_array
{
    struct line_sensor
    {
        int A0;
        int A1;
        int A2;
        int A3;
        int A4;
    } lsa;

    int adc_reading[5];
} line_sensor_array;

esp_err_t enable_line_sensor();

line_sensor_array read_line_sensor();


static const char *TAG_ADC = "adc";

esp_err_t config_adc1()
{
    // Configure ADC to 12 bit width
    CHECK(adc1_config_width(ADC_WIDTH_BIT_12));

    // Configure ADC to 11dB attenuation
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_4, ADC_ATTEN_DB_11));
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_7, ADC_ATTEN_DB_11));
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_6, ADC_ATTEN_DB_11));
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_3, ADC_ATTEN_DB_11));
    CHECK(adc1_config_channel_atten(ADC_CHANNEL_0, ADC_ATTEN_DB_11));

    ESP_LOGI(TAG_ADC, "Configured ADC_1 to 12 Bit and 11dB attenuation");

    return ESP_OK;
}

esp_err_t characterize_adc1()
{
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);

    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        ESP_LOGI(TAG_ADC, "eFuse Two Point: Supported");
        ESP_LOGI(TAG_ADC, "Characterized using Two Point Value");
    }
    else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        ESP_LOGI(TAG_ADC, "eFuse Vref: Supported");
        ESP_LOGI(TAG_ADC, "Characterized using eFuse Vref");
    }
    else
    {
        ESP_LOGI(TAG_ADC, "Characterized using Default Vref");
    }
    return ESP_OK;
}

esp_err_t enable_adc1()
{
    CHECK(config_adc1());
    CHECK(characterize_adc1());

    ESP_LOGI(TAG_ADC, "Configured and Characterized adc 1");

    return ESP_OK;
}

int read_adc(int adc_pin)
{
    if (adc_pin == LSA_A0)
    {
        return adc1_get_raw(ADC_CHANNEL_4);
    }
    else if (adc_pin == LSA_A1)
    {
        return adc1_get_raw(ADC_CHANNEL_7);
    }
    else if (adc_pin == LSA_A2)
    {
        return adc1_get_raw(ADC_CHANNEL_6);
    }
    else if (adc_pin == LSA_A3)
    {
        return adc1_get_raw(ADC_CHANNEL_3);
    }
    else if (adc_pin == LSA_A4)
    {
        return adc1_get_raw(ADC_CHANNEL_0);
    }
    else
    {
        ESP_LOGE(TAG_ADC, "Invalid adc pin");
        return ESP_FAIL;
    }
}


static const int line_sensor_pins[5] = {LSA_A0, LSA_A1, LSA_A2, LSA_A3, LSA_A4};

esp_err_t enable_line_sensor()
{
    esp_err_t err = enable_adc1(line_sensor_pins);
    return err;
}

line_sensor_array read_line_sensor()
{
    line_sensor_array line_sensor_readings;

    for (int i = 0; i < 5; i++)
    {
        line_sensor_readings.adc_reading[i] = 0;
    }

    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            line_sensor_readings.adc_reading[j] = line_sensor_readings.adc_reading[j] + read_adc(line_sensor_pins[j]);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        line_sensor_readings.adc_reading[i] = line_sensor_readings.adc_reading[i] / NUMBER_OF_SAMPLES;
    }

    return line_sensor_readings;
}


static const char *TAG = "LSA_READINGS";

void app_main(void)
{
    // enable line sensor after checking optimal working state of ESP
    ESP_ERROR_CHECK(enable_line_sensor());

    // Union containing line sensor readings
    line_sensor_array line_sensor_readings;
    
    while (1)
    {
        // get line sensor readings from the LSA sensors
        line_sensor_readings = read_line_sensor();
        for (int i = 0; i < 5; i++)
        {
            line_sensor_readings.adc_reading[i];
            
            // constrain lsa readings between BLACK_MARGIN and WHITE_MARGIN
            //line_sensor_readings.adc_reading[i] = bound(line_sensor_readings.adc_reading[i], WHITE_MARGIN, BLACK_MARGIN);
            
            //line_sensor_readings.adc_reading[i] = map(line_sensor_readings.adc_reading[i], WHITE_MARGIN, BLACK_MARGIN, CONSTRAIN_LSA_LOW, CONSTRAIN_LSA_HIGH);
            //line_sensor_readings.adc_reading[i] = 1000 - line_sensor_readings.adc_reading[i];
        }
// #ifdef CONFIG_ENABLE_OLED
        // Displaying LSA Bar on OLED
//         display_lsa(line_sensor_readings);
// #endif
        // Displaying Information logs - final lsa readings
        ESP_LOGI(TAG, "LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d \t LSA_5: %d", line_sensor_readings.adc_reading[0], line_sensor_readings.adc_reading[1], line_sensor_readings.adc_reading[2], line_sensor_readings.adc_reading[3], line_sensor_readings.adc_reading[4]);
        // delay of 10 ms after each log
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
