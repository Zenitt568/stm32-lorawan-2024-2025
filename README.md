# STM32 LoraWan

Embedded Systems IOT project that utilizes STM32 L152 Nucleo and USI STM32 to create a two-way LoRa message exchange.

Developed for IOT: Internet of Things course @ WUT Warsaw University of
Technology.

Visit the [Wiki](https://github.com/pmielech/stm32-lorawan/wiki) to read more
about the project.

# Features

- BME280 Sensor to read surrounding's general temperature and pressure
- [SEN0193](https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193) Sensor to measure soil moisture levels
- Request to measuring SLAVE is sent only when button on MASTER board is pressed,

# Built with

- STM32 L152 Nucleo-64 development board,
- USI STM32 Nucleo expansion board for LoRa,
- BME280 I2C/SPI breakout board,

# File structure:

```
|
|-- src
|   |
|   |- config.h
|   |- pinout.h
|   |- ADC.h
|   |- ADC.cpp
|   |- main.h
|   |- main.cpp
|   |- lora.h (interfacing with LoRa WAN Shield)
|   |- lora.cpp
|   |- bme280_sensor.h (interfacing, using Adafruit BME280 sensor)
|   |- bme280_sensor.cpp
|
|- LICENSE
|- platformio.ini
|- README.md --> This file
```

## Future development note

- [Issue] Debugging with current PlatformIO configuration don't work properly
- [Issue] Sending larger number of bytes

## Useful links

For more information on how to use ST I-NUCLEO-LRWAN1 Shield with Arduino framework for STM32 look [here](https://github.com/stm32duino/I-NUCLEO-LRWAN1/blob/main/src/LoRaRadio.cpp).