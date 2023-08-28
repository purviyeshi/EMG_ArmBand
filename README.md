
# EMG_ArmBand

To create an EMG (Electromyography) Arm Band, which may be worn as a bracelet, to detect hand movements and orientation via muscle impulses transmitted through your forearm. One will be designing PCBs based on EMG sensors, integrating them with microcontrollers, and subsequently train the system using Machine Learning (ML).

## Table of Contents

-  [About The Project](#emg_armband)
- [Domains Explored](#domains-explored)
- [Project Workflow](#project-workflow)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Contributors](#contributors)
- [References](#references)


## Domains Explored

Embedded Systems, IoT, PCB Designing, ML

## Project Workflow

- Learn how to initialize ADC and read digital output readings of Analog sensor
- Study ESP_NOW Communication protocol and send message from one ESP to other ESP
- Learn to design Flex PCB
- Study EMG (Electromyography) Sensors and try to integrate it with microcontrollers (ESP32)


## Getting Started
These instructions will help you get a copy of the project up and running on your local machine.

### Prerequisites
To download and use this code, the minimum requirements are:

- [ESP_IDF](https://github.com/espressif/esp-idf)
- Windows 7 or later (64-bit), Ubuntu 20.04 or later
- [Microsoft VS Code](https://code.visualstudio.com/download) or any other IDE

### Installation
Clone the project by typing the following command in your Terminal

```bash
  git clone https://github.com/purviyeshi/EMG_ArmBand.git
```

Navigate to the EMG_ArmBand folder
```bash
  cd EMG_ArmBand
```

### Usage
Once the requirements are satisfied, you can easily download the project and use it on your machine. After following the above steps , use the following commands to:

To activate the IDF
```bash
  get_idf
```
To build the code
```bash
  idf.py build
```
To flash the code
```bash
  idf.py -p (PORT) flash monitor
```
## Contributors

- [Purva Yeshi](https://github.com/purviyeshi/EMG_ArmBand.git)
- [Anuj Akotkar](https://github.com/anuj-akotkar/EMG_armband.git)

## References
[ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/v4.4.3/esp32/index.html)
