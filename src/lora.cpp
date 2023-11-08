#include "lora.h"
#include "config.h"
#include "bme280_sensor.h"
#include "ADC.h"

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
    uint8_t message2[MESSAGE_SIZE];

    /* Split each 16-bit data to 2x8-bit ones, with bit masking */
    message[0] = (data->temperature & 0xFF00) >> 8;
    message[1] = (data->temperature & 0x00FF);
    message[2] = (data->pressure & 0xFF00) >> 8;
    message[3] = (data->pressure & 0x00FF);

     // 2nd packet with humidity
    float soil_humidity =  mapADCValueToPercentage(ADC_Read());

    /*DEBUG*/
    //Serial.println("Zmierzona wilgotnosc gleby:");
    /*DEBUG*/

    message2[0] = (int(soil_humidity*100) & 0xFF00) >> 8;
    message2[1] = (int(soil_humidity*100) & 0x00FF);
    message2[2] = 0x04; //Attach EOT at the end of humidity string

    Serial.println("[INFO] Sending response");

    /*DEBUG*/  
    /*for (uint8_t i = 0; i<4; i++){
        Serial.println((float)message[i]);
    }
    for (uint8_t i = 0; i<4; i++){
        Serial.println((float)message2[i]);
    }*/
    /*DEBUG*/

    /*Sending 1st packet of data*/
    loraRadio.write(message, 4);
    delay(2000);

    /*Sending 2nd packet of data*/
    loraRadio.write(message2, 4);
}

/*Not in use*/
void ReadResponse(DataReceived_t *data, uint8_t message[])
{
    /* Merge each 2x8-bit packs to 16-bit ones, fix floats */
    
    data->temperature = (float)((message[0] << 8) + message[1]) / 100;
    data->pressure = (float)((message[2] << 8) + message[3]);
    
    memset(message, 0, MESSAGE_SIZE);

    // Message strings array
    String serialMessage[5] = {
        "[INFO] Response received:",
        "\tTemperature: " + String(data->temperature) + " \u00b0C",
        "\tPressure: " + String(data->pressure) + " hPa"
    };

    for (uint8_t idx = 0; idx < (sizeof(serialMessage) / sizeof(serialMessage[0])); idx++){
        Serial.println(serialMessage[idx]);
    }
    Serial.println(); //To separate incoming messages
}

void ReadData(uint8_t message[]){

    loraRadio.read(message);

    //Received packet with humidity
    if (message[2] == 0x04){
        float received_humidity = (float)((message[0] << 8) + message[1]) / 100;
        String humidity_response[] = {
            "\tSoil humidity: "+ String(received_humidity) + "%"
        };

        for (uint8_t idx = 0; idx < (sizeof(humidity_response) / sizeof(humidity_response[0])); idx++){
            Serial.println(humidity_response[idx]);
        }
    }
    //Received packet with temperature and pressure
    else{
        float received_temperature = (float)((message[0] << 8) + message[1]) / 100;
        float received_pressure = (float)((message[2] << 8) + message[3]);
        String BME_response[] = {
            "[INFO] Response received:",
            "\tTemperature: " + String(received_temperature) + " \u00b0C",
            "\tPressure: " + String(received_pressure) + " hPa"
        };

        for (uint8_t idx = 0; idx < (sizeof(BME_response) / sizeof(BME_response[0])); idx++){
            Serial.println(BME_response[idx]);
        }
    }
}
}
