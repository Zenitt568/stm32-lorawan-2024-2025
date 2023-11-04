#ifndef _LORA_H
#define _LORA_H

#include <Arduino.h>
#include <Wire.h>
#include "LoRaRadio.h"
#include "bme280_sensor.h"

namespace LoRa
{
    void ShieldInit(void);
    void DataInit_Lora(DataReceived_t *data);

    void SendRequest(void);
    void ReadData(uint8_t message[]);
    void SendResponse(DataRead_t *data);
    void ReadResponse(DataReceived_t *data, uint8_t message[]);
};

#endif /* _LORA_H */
