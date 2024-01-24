#include "bme280_sensor.h"

Adafruit_BME280 sensor;

bool BME_ACTIVE = false;

namespace BME280{

uint8_t HardwareInit(void)
{

    uint8_t ret = 0;
    Serial.println("[INFO] Trying to connect BME280 sensor...");

    Wire.setSDA(SDA_PIN);
    Wire.setSCL(SCL_PIN);

    if (!sensor.begin(SENSOR_ADDR))
    {
        Serial.println("[ERR] BME280 sensor not found!");
        BME_ACTIVE = false;
        ret++;
        // while (true)
        // {
        //     /* Infinite loop, do not continue if no sensor */
        // }
    } else {
        Serial.println("[INFO] BME280 sensor connected.");
        BME_ACTIVE = true;

    }
    
    return ret;
}

void DataInit_Bme280(DataRead_t *data) // name duplicate
{
    data->temperature = 0;
    data->pressure = 0;
}

void ReadData(DataRead_t *data)
{
    /* Magnitude change for easier sending */
    data->temperature = (sensor.readTemperature() * 100);
    data->pressure = (sensor.readPressure() / 100.00f);
}
}