## RECEIVER SIDE
As we transfer the data form one esp module to another esp module on the receiver side ,data is serially transfer in form of blocks of 5-6 sec approx. 1000 values that is required for the training and testing set of ML model  

Data transfer process between ESP modules, we transmit the data in the form of sequential blocks, each containing approximately 1000 values, and each block spans a duration of around 5-6 seconds. These data blocks are significant as they serve as the training and testing datasets for our Machine Learning (ML) model. By transferring data in this organized manner, we ensure that the ML model receives a consistent and substantial dataset for training and evaluation, which is crucial for achieving accurate and meaningful results in our application. This structured data transfer approach supports the development and validation of our ML model for its intended purpose.

How the receiver codes works for ESP-32:

1. **Receiver Address Initialization:**
   ```c
   uint8_t receiverAddress[] = {0xC4, 0xDD, 0x57, 0x5B, 0xF8, 0x54};
   ```
   In this line, an array `receiverAddress` is initialized with a 6-byte MAC (Media Access Control) address. This address is used to uniquely identify the receiver module. You should replace this with the actual MAC address of your ESP module.

2. **Received Message Buffer:**
   ```c
   char receivedMessage[100];
   ```
   Here, an array `receivedMessage` is defined to hold received data. It is set to have a maximum capacity of 100 characters, which can be used to store received data.

3. **`ondatareceived` Function:**
   ```c
   void ondatareceived(const uint8_t *mac_addr, const uint8_t *data, int len) {
       ESP_LOGI(TAG, "Received message: %.*s\n", len, data);
       //int data_len = sizeof(receivedMessage) / sizeof(receivedMessage[0]);
       //memcpy(receivedMessage, data, data_len);
       //receivedMessage[data_len];
   }
   ```
   This is a function called `ondatareceived`, which is typically used as a callback function to handle received data. It takes three parameters: `mac_addr` (the MAC address of the sender), `data` (the received data), and `len` (the length of the received data in bytes). In this function, a log message is printed with the received data, and you also see some commented-out lines that suggest the code might have been a work in progress for storing received data into `receivedMessage`.

4. **`app_main` Function:**
   ```c
   void app_main(void) {
       esp_err_t cpy = nvs_flash_init();
       if (cpy == ESP_ERR_NVS_NO_FREE_PAGES || cpy == ESP_ERR_NVS_NEW_VERSION_FOUND) {
           ESP_ERROR_CHECK(nvs_flash_erase());
           cpy = nvs_flash_init();
       }
       ESP_ERROR_CHECK(cpy);
   
       // Initialize WiFi
       wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
   }
   ```
   In the `app_main` function, non-volatile storage (`nvs_flash`) is initialized to store configuration data. The code checks if there are free pages or a new version found, and if so, it erases and reinitializes the flash. Then, it initializes the WiFi configuration (`wifi_init_config_t`).

An ESP32-based program that initializes essential components like MAC addresses, storage, and WiFi. It also includes a function to handle received data. Please note that there are some commented-out lines, so it might be a work in progress or need further development for specific functionality.
