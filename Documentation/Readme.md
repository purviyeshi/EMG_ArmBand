# TABLE OF CONTENT:
1. TRANSMITTER SETUP
    * [EMG signal acquisition](EMG-signal-acquisition)
    * [Amplifying circuit](Amplifying-circuit)
    * [Bandpass filter](Bandpass-filter)
    * [EMG signals to ADC channels of ESP-32](EMG-signals-to-ADC-channels-of-ESP-32)
   *   [Errors and It's Solutions.](ERRORS-and-It's-Solutions)
2. RECEIVER  SETUP
   * [Data  acquisition form ADC channel(esp-32).](Data-acquisition-form-ADC-channel-(esp-32))
3. ESP-NOW

5. ML MODEL
6. PCB DESIGNING
   * [Board images.]([Board-images)
   * [changes need to be Done in Board.](changes-need-to-be-Done-in-Board)

## TRANSMITTER SETUP

- ### EMG signal acquisition.
 #### Wet electrodes

conductive gel or electrolyte solution enhances the signal quality by reducing impedance.

Wet electrodes offer superior signal quality and lower noise compared to dry electrodes.

 #### Dry electrode

Dry electrodes do not require any additional gel .They are designed to make direct contact with the skin's surface.

They are more user-friendly and convenient for quick and easy application . Dry electrodes may have slightly higher impedance compared to wet electrodes, which can lead to lower signal quality, especially in challenging conditions.
**Dry electrode are ideal for applications where convenience and ease of use are prioritized over the absolute highest signal quality.
It this project we use wet electrode as we are prioritized over the absolute highest signal quality.**
 
 ![233](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/4f347012-8bbc-493d-8b23-a280a1d69cf5)


- ### Amplifying circuit.

EMG signals are weak, low-amplitude electrical signals produced by muscle contractions. Amplification increases signal strength, reduces noise, enhances sensitivity, and allows for precise data analysis. This is critical for applications like monitoring muscle activity, and ensuring high-quality, reliable EMG data.

EMG signal acquisition system, we utilize five electrodes, with each amplifying module being dedicated to one specific electrode. Therefore, we require a total of five amplifying modules, each serving as an interface for an individual electrode. This setup allows us to amplify and process the signals from all five electrodes independently and enhances the accuracy and specificity of our EMG data collection.

###### Instrumental Amplifier
![INSTU  AMPL](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/628677fb-256e-48c7-add5-0407b27e1081)

In-Amps, are essential because they offer high-precision amplification of weak signals while effectively rejecting common-mode noise. They provide accuracy, customization of gain, and low noise levels, making them ideal for applications where signal fidelity is critical, such as medical instruments and data acquisition systems. Additionally, their high input impedance minimizes source loading, and they can be tailored to specific requirements, ensuring signal quality and accuracy in a wide range of applications.
   
###### amplifying circuit
for an Amplifying circuits check out this links below:

- https://www.instructables.com/Muscle-EMG-Sensor-for-a-Microcontroller/
- https://github.com/upsidedownlabs/BioAmp-EXG-Pill

- ### Bandpass filter

A bandpass filter for EMG signal acquisition is an electronic circuit designed to isolate and emphasize a specific frequency range within the EMG signal. It helps reduce noise, isolate muscle activity-related frequencies, and improve the quality and specificity of the acquired EMG data. By tailoring the lower and upper cutoff frequencies, the bandpass filter can be customized to match the frequency characteristics of the EMG signal, making it a crucial component in medical, research, and clinical applications for accurate and meaningful analysis of muscle activity.
##### 4th order bandpass filter
![342](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/091cea03-248b-4bb8-b407-fafddba9b6de)


![235](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/00ee053e-c1bb-4873-9779-60e22ef640a6)



- ### EMG signals to ADC channels of ESP-32.(Analog to Digital converter)

The ESP32 integrates 2 SAR (Successive Approximation Register) ADCs, supporting a total of 18 measurement channels (analog enabled pins)

These channels are supported:
- ##### 8 channels: GPIO32 - GPIO39
- ##### 10 channels: GPIO0, GPIO2, GPIO4, GPIO12 - GPIO15, GOIO25 - GPIO27.

![aaj11](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/b8764f82-aeae-4df4-95bf-fa711ba12b2e)


- #### ADC Attenuation

Vref is the reference voltage used internally by ESP32 ADCs for measuring the input voltage. The ESP32 ADCs can measure analog voltages from 0 V to Vref. Among different chips, the Vref varies, the median is 1.1 V. In order to convert voltages larger than Vref, input voltages can be attenuated before being input to the ADCs. There are 4 available attenuation options, the higher the attenuation is, the higher the measurable input voltage could be.

- #### ADC Conversion

An ADC conversion is to convert the input analog voltage to a digital value. The ADC conversion results provided by the ADC driver APIs are raw data. Resolution of ESP32 ADC raw results under Single Read mode is 12-bit.

- ##### esp_err_t adc1_config_width(adc_bits_width_t width_bit)
Configure ADC1 capture width, meanwhile enable output invert for ADC1. The configuration is for all channels of ADC1.

- ##### int adc1_get_raw(adc1_channel_t channel)
Take an ADC1 reading from a single channel


- ### Errors and It’s Solutions
1. output of an amplifyng circuit is not varies on muscle movement.
solution: In our setup, we employ a two-step approach to amplify the EMG (Electromyography) signal effectively. Initially, we increase the signal's gain through one amplifying circuit, optimizing it for preliminary processing and noise reduction. Following this, we transition to a second amplifying circuit known as "muscle_sensor_v3." This transition is made to further amplify the already refined signal. By employing this sequential amplification strategy, we gain more precise control over the signal quality and amplification, thereby enhancing the overall performance and accuracy of our EMG signal acquisition system
2. variation observed in the emg signal reading on muscle movement but signals are containing high noise.
SOLUTION:  The implementation of a 4th-order bandpass filter in EMG (Electromyography) signal processing is essential for refining the data acquired from muscle activity. This filter is designed to selectively pass a specific range of frequencies within the EMG signal while suppressing unwanted frequency components outside of this range. 

click here for 4th order butterworth filter:-
- https://github.com/upsidedownlabs/BioAmp-EXG-Pill/blob/main/software/EMGFilter/EMGFilter.ino

## RECEIVER SETUP
As we transfer the data form one esp module to another esp module on the receiver side ,data is serially transfer in form of blocks of 5-6 sec approx. 1000 values that is required for the training and testing set of ML model  

Data transfer process between ESP modules, we transmit the data in the form of sequential blocks, each containing approximately 1000 values, and each block spans a duration of around 5-6 seconds. These data blocks are significant as they serve as the training and testing datasets for our Machine Learning (ML) model. By transferring data in this organized manner, we ensure that the ML model receives a consistent and substantial dataset for training and evaluation, which is crucial for achieving accurate and meaningful results in our application. This structured data transfer approach supports the development and validation of our ML model for its intended purpose.
## ESP-NOW
ESP-NOW is a kind of connectionless Wi-Fi communication protocol that is defined by Espressif. In ESP-NOW, application data is encapsulated in a vendor-specific action frame and then transmitted from one Wi-Fi device to another without connection. CTR with CBC-MAC Protocol(CCMP) is used to protect the action frame for security. ESP-NOW is widely used in smart light, remote controlling, sensor, etc.

- #### MAC HEADER
The MAC (Media Access Control) address of an ESP (Espressif) module, such as the ESP8266 or ESP32, is a unique identifier for the module's network interface. It is assigned by the manufacturer and is used to distinguish one ESP module from another on a network.

The first address field is set to the destination address. The second address field is set to the source address. The third address field is set to broadcast address (0xff:0xff:0xff:0xff:0xff:0xff).

NOTE:
Call esp now init() to initialize ESP-NOW and esp now deinit() to de-initialize ESP-NOW

When esp now deinit() is called, all of the information of paired devices will be deleted.


- #### ADD PAIRED DEVICE
Call esp now add peer() to add the device to the paired device list before you send data to this device.

- #### SEND ESP-NOW DATA
Call esp now send() to send ESP-NOW data and esp now register send cb() to register sending callback function. It will return ESP NOW SEND SUCCESS in sending callback function if the data is received successfully on the MAC layer. Otherwise, it will return ESP NOW SEND FAIL. 

- #### Receiving ESP-NOW Data
 Call esp now register recv cb() to register receiving callback function. Call the receiving callback function when receiving ESP-NOW.
 
 refer this for implementation of esp-now:
 - https://docs.espressif.com/projects/esp-idf/en/latest/esp32

## PCB DESIGNING

- #### BOARD IMAGES

- ##### Amplifying circuit(front side)
![5](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/0197d590-3286-4fe5-84fa-ea3599c68015)


- ##### amplifying circuit and esp-32 intregrted module:

- ##### front side
![PCB 1](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/f266df9c-00aa-4537-88e3-fa2ccc35f153)

- ##### back side
 ![PCB 22](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/39dc0846-2d10-45b5-bd3b-6e918e5a8419)


- ##### routing

![PCB 33](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/547042e3-ee44-4962-a636-7bad6727cc81)

## ML MODEL
In this project, a machine learning model is implemented to predict real-time data, specifically discerning between two hand movements: one for a clenched fist and the other for a relaxed hand. The ML model is designed to classify the ongoing hand movements into these two distinct classes.

For improved accuracy, the model is trained on a dataset that captures diverse instances of both fist-clenching and hand relaxation

![Screenshot 2023-11-05 021133](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/28efadf5-43e4-412e-94c2-ee3039bdef7e)


![Screenshot 2023-11-05 021205](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/877c4f30-2a79-4a4e-8c74-318a6020de2b)


![Screenshot 2023-11-05 021233](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/868438e4-9a9d-4396-be0b-6315f4a8e0b6)






