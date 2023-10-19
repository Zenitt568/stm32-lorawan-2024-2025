#include "lora.h"
#include "config.h"
#include "bme280_sensor.h"

HardwareSerial SerialLora(PA10, PA9);

namespace LoRa{


void ShieldInit(void)
{
    Serial.begin(SERIAL_SPEED);

#if SW_TYPE == SLAVE
    Serial.println("LoRa SLAVE Module");

#elif SW_TYPE == MASTER
    Serial.println("LoRa MASTER Module");

#endif // SW_TYPE
    
    while (!loraRadio.begin(&SerialLora))
    {
        Serial.println("[INFO] LoRa Shield not ready!");
        delay(1000); /* Give module 1s to init */
    }

    Serial.println("[INFO] Shield ready!");


#if SW_TYPE == SLAVE
    BME280::HardwareInit();     // Init BME280 sensor duplicate

#endif // SW_TYPE == SLAVE
    
}

void DataInit_Lora(DataReceived_t *data)
{
    data->temperature = 0;
    data->pressure = 0;
}

void SendRequest(void)
{
    uint8_t message[1];
    message[0] = 0xFF;

    Serial.println("[INFO] Sending new request");

    loraRadio.write(message, 1);
}

void SendResponse(DataRead_t *data)
{
    uint8_t message[MESSAGE_SIZE];

    /* Split each 16-bit data to 2x8-bit ones, with bit masking */
    message[0] = (data->temperature & 0xFF00) >> 8;
    message[1] = (data->temperature & 0x00FF);
    message[2] = (data->pressure & 0xFF00) >> 8;
    message[3] = (data->pressure & 0x00FF);

    Serial.println("[INFO] Sending response");

    loraRadio.write(message, MESSAGE_SIZE);
}

void ReadResponse(DataReceived_t *data, uint8_t message[])
{
    /* Merge each 2x8-bit packs to 16-bit ones, fix floats */

    
    data->temperature = (float)((message[0] << 8) + message[1]) / 100;
    data->pressure = (float)((message[2] << 8) + message[3]);

    memset(message, 0, MESSAGE_SIZE);

    /* Message strings array */
    String serialMessage[3] = {
        "[INFO] Response received:",
        "\tTemperature: " + String(data->temperature) + " \u00b0C",
        "\tPressure: " + String(data->pressure) + " hPa",
    };

    for (uint8_t idx = 0; idx < (sizeof(serialMessage) / sizeof(serialMessage[0])); idx++)
    {
        Serial.println(serialMessage[idx]);
    }

    Serial.println();
}
}
