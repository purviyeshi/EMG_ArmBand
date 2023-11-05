## TRANSMITTER SIDE
The overall goal of the transmitter side of the EMG armband is to collect, process, and transmit muscle signal data accurately and efficiently. This data can then be used for various applications, including gesture recognition and other human-computer interaction systems.

1. ## ELECTRODES


### "In our project, we will be using either wet or dry electrodes."

Here we Specify features of wet and dry electrodes

- ### Wet electrodes:

conductive gel or electrolyte solution enhances the signal quality by reducing impedance.

Wet electrodes offer superior signal quality and lower noise compared to dry electrodes.

- ### Dry electrode:

Dry electrodes do not require any additional gel .They are designed to make direct contact with the skin's surface.

They are more user-friendly and convenient for quick and easy application . Dry electrodes may have slightly higher impedance compared to wet electrodes, which can lead to lower signal quality, especially in challenging conditions.

## CONCLUSION :-
Dry electrode are ideal for applications where convenience and ease of use are prioritized over the absolute highest signal quality.

It this project we use wet electrode as we are prioritized over the absolute highest signal quality.

![2](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/44b9faf3-59e3-4399-8701-4d03a4bd2c04)


###### Wet Electrodes

![3](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/feb25b9f-e87f-4bf9-8566-ebe8dc1888f3)

###### Dry Electrodes

#### NOTE: 
 As shown above image of one electrode,In this project we are works on five elerodes for more accurately capturing an emg signals , to executive diffrent gesture or hand movements recognition.


2. ## AMPLIFING CIRCUITS

- ##### why we need an amplifing circuit ?
   EMG signals are weak, low-amplitude electrical signals produced by muscle contractions. Amplification increases signal strength, reduces noise, enhances sensitivity, and allows for precise data analysis. This is critical for applications like monitoring muscle activity, and ensuring high-quality, reliable EMG data.


#### NOTE:
In the EMG signal acquisition system, we utilize five electrodes, with each amplifying module being dedicated to one specific electrode. Therefore, we require a total of five amplifying modules, each serving as an interface for an individual electrode. This setup allows us to amplify and process the signals from all five electrodes independently and enhances the accuracy and specificity of our EMG data collection.

In our EMG signal acquisition setup, we combine one amplification PCB with the ESP-32 microcontroller within a single module. Consequently, we have a total of five separate modules, with one of them featuring the integrated amplification circuit alongside the ESP-32. The remaining four modules are dedicated to individual electrodes, allowing for efficient signal processing. 

##### AMPLIFYING CIRCUIT:



 - #### FRONT SIDE OF AMPLIFYING CIRCUIT
![5](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/2186c5e6-d295-4333-a77e-c659dad35cb1)



- ##### ROUTING

![4](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/88ecfbe2-2a02-4468-ad28-2e30dff4b4f8)


- #### The "REF," "IN-," and "IN+" connectors located on the left side of the PCB should be appropriately connected to their respective reference, detection, and muscle electrodes. This ensures that each electrode is correctly linked to the corresponding input for precise signal acquisition and processing.


- #### The 3.7V and GND connector pins on the left side of the PCB should be connected to the circuit's power source, typically a battery supply. This connection provides the necessary power to operate the circuit effectively.

- #### EMG muscle signals vary in amplitude: at rest, they're in the microvolt (µV) to tens of µV range, during normal contractions, from tens of µV to hundreds of µV (µV to mV), and during intense contractions, they can reach millivolts (mV). Amplification and signal processing should handle this range in EMG signal acquisition.

- #### The ESP32 ADCs can measure analog voltages from 0 V to Vref. the Vref varies, the median is 1.1 V. In order to convert voltages larger than Vref, input voltages can be attenuated before being input to the ADCs. There are 4 available attenuation options, the higher the attenuation is, the higher the measurable input voltage could be.

![attenuation-input voltage range.png](https://hackmd.io/_uploads/B1Ff7YZmT.png)


- ### ESP-32 INTREGATED WITH AMPLIFYING CIRCUIT
 

- ##### FRONT SIDE 
![PCB 1](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/26e5fba6-d7e3-44ff-9b3c-2e9110fdee33)

## BACK SIDE
![PCB 22](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/04b432e2-e95c-437d-92a4-0cb485a335ed)


- #### ROUTING

![PCB 33](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/ebf1d0f3-5d1e-48b7-a1d7-5faeb9fc207a)


3. AMPLIFYING SIGNAL TO ADC CHANNELS OF ESP-32:

The ESP32 integrates 2 SAR (Successive Approximation Register) ADCs, supporting a total of 18 measurement channels (analog enabled pins)

These channels are supported:
- ##### 8 channels: GPIO32 - GPIO39
- ##### 10 channels: GPIO0, GPIO2, GPIO4, GPIO12 - GPIO15, GOIO25 - GPIO27.

![ESP-32 PINOUT](https://github.com/anuj-akotkar/EMG_armband/assets/142881764/8f3719dd-6361-40a8-94c0-21fc51ed62ef)


- #### ADC Attenuation

Vref is the reference voltage used internally by ESP32 ADCs for measuring the input voltage. The ESP32 ADCs can measure analog voltages from 0 V to Vref. Among different chips, the Vref varies, the median is 1.1 V. In order to convert voltages larger than Vref, input voltages can be attenuated before being input to the ADCs. There are 4 available attenuation options, the higher the attenuation is, the higher the measurable input voltage could be.

- #### ADC Conversion

An ADC conversion is to convert the input analog voltage to a digital value. The ADC conversion results provided by the ADC driver APIs are raw data. Resolution of ESP32 ADC raw results under Single Read mode is 12-bit.

