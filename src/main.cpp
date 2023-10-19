#include "main.h"
#include "lora.h"
#include "bme280_sensor.h"

uint8_t requestMessage[1];
uint8_t receivedMessage[MESSAGE_SIZE];

bool nextMessage = true;
uint8_t interruptState = 0;
uint32_t currentTime = 0;

//LoRa::ModuleType_t moduleType = MODULE_TYPE;
DataRead_t sensorData;
DataReceived_t receivedData;


void setup()
{
// TODO: ADD INIT FUNCTIONS WITH CRC 
  /* Init LoRa shield */
  LoRa::ShieldInit();  //dont need to pass module type, it is defined in config.h

  /* Init data container structs */
  BME280::DataInit_Bme280(&sensorData);  // Init BME280 sensor duplicate
  LoRa::DataInit_Lora(&receivedData);

#if SW_TYPE == MASTER
    attachInterrupt(digitalPinToInterrupt(BOARD_BTN), ButtonClickInterrupt, RISING);

#endif // SW_TYPE == MASTER

}

void loop()
{

  // TODO: ADD HEARTBEAT FUNCTION
  #if SW_TYPE == MASTER
    if (interruptState)
    {
      LoRa::SendRequest();    // Should communication be done here or in lora.cpp?
      BOOL(interruptState);
    }

    if (loraRadio.read(receivedMessage) > 0)
    {
      LoRa::ReadResponse(&receivedData, receivedMessage);
    }

  #elif SW_TYPE == SLAVE
    if (loraRadio.read(requestMessage) == 0x04)
    {
      memset(requestMessage, 0, 1);
      Serial.println("[INFO] Data request received");

      BME280::ReadData(&sensorData);
      LoRa::SendResponse(&sensorData);
    }
  #endif
}

void ButtonClickInterrupt(void)
{
  BOOL(interruptState);
}
