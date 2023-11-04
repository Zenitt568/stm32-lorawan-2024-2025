#ifndef _BME280_H
#define _BME280_H

#include <Wire.h>
#include <SPI.h>
#include "pinout.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "config.h"


typedef struct      
{
    uint16_t temperature;
    uint16_t pressure;

} DataRead_t;

typedef struct
{
    float temperature;
    float pressure;

} DataReceived_t;

namespace BME280
{
    /* Inits BME280 sensor*/
    void HardwareInit(void);
    void DataInit_Bme280(DataRead_t *data);
    void ReadData(DataRead_t *data);
};

#endif /* _BME_280 */