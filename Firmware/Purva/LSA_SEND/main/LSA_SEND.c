#include "driver/adc.h"
#include "esp_err.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_now.h"


#include <string.h>
#include "esp_wifi.h"
#include "esp_now.h"
#include "nvs_flash.h"  // Include the NVS flash header


#define NUM_SENSORS 5

#define DEFAULT_VREF 1100

char readings_str[100];

static const char *TAG = "READINGS";

// Define the sender and receiver MAC addresses
uint8_t unicastAddress[] = {0xE8, 0x31, 0xCD, 0xC3, 0xEF, 0x8C};   
uint8_t receiverAddress[] = {0xE8, 0x31, 0xCD, 0xC3, 0xEF, 0x8C}; //   Receiver MAC Address: E8:31:CD:C3:EF:8C


// Callback function to handle ESP-NOW sending status
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        ESP_LOGI(TAG, "Message sent successfully");
    } else {
        ESP_LOGE(TAG, "Message sending failed");
    }
}



// Initialize NVS
void initialize_nvs(){
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}


// Initialize WiFi
void initialize_wifi(){
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

// Initialize ESP-NOW
void initialize_esp_now(){
    ESP_ERROR_CHECK(esp_now_init());
    esp_now_register_send_cb(OnDataSent);
}



// Add the receiver
void peer_info(){ 
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(esp_now_peer_info_t));
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}

   

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
    
    
    void OnDataSent();
    void initialize_nvs();
    void initialize_wifi();
    void initialize_esp_now();
    void peer_info();
       
    while (1) {
        for (int i = 0; i < NUM_SENSORS; i++) 
        {
            raw_sensor_readings[i] = adc1_get_raw(sensor_channels[i]);
        }


	

        // Display the updated array of raw ADC readings
        // ESP_LOGI(TAG, "LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d \t LSA_5: %d", raw_sensor_readings[0], raw_sensor_readings[1], raw_sensor_readings[2], raw_sensor_readings[3], raw_sensor_readings[4]);
        
        // Convert the raw sensor readings to a string
          // Adjust the buffer size as needed
        snprintf(readings_str, sizeof(readings_str), "LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d \t LSA_5: %d",raw_sensor_readings[0], raw_sensor_readings[1], raw_sensor_readings[2], raw_sensor_readings[3], raw_sensor_readings[4]);
        
        
        // Send the message
    esp_err_t result = esp_now_send(unicastAddress, (uint8_t *)readings_str, strlen(readings_str));
    if (result == ESP_OK) 
    {
        ESP_LOGI(TAG, "Message sending succeeded");
    } 
    else 
    {
        ESP_LOGE(TAG, "Message sending failed_______");
    }
        
        
       
    }
   
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
