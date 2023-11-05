#include "main.h"
#include "lora.h"
#include "bme280_sensor.h"
#include "ADC.h"


uint8_t requestMessage[1];
uint8_t receivedMessage[MESSAGE_SIZE];

bool nextMessage = true;
uint8_t interruptState = 0;
uint32_t currentTime = 0;

volatile int interruptLedCount = 0;

//LoRa::ModuleType_t moduleType = MODULE_TYPE;
DataRead_t sensorData;
DataReceived_t receivedData;

// Create instance of TIM2
HardwareTimer *MyTim = new HardwareTimer(TIM2);

void turnOffLed(void){
  digitalWrite(LED_GREEN, false);
}

void updateLedState(void)
{
  digitalWrite(LED_GREEN, !digitalRead(LED_GREEN)); // Change state of LED
  interruptLedCount++;
  if (interruptLedCount >= 8){ //After 8th change -> turn off timer
    MyTim->pause();
    interruptLedCount = 0;
    turnOffLed(); //In case of LED staying turned ON
  }
}

void setup()
{
// TODO: ADD INIT FUNCTIONS WITH CRC 
  /* Init LoRa shield */
  LoRa::ShieldInit();  //dont need to pass module type, it is defined in config.h

  /* Init data container structs */
  BME280::DataInit_Bme280(&sensorData);  // Init BME280 sensor duplicate
  LoRa::DataInit_Lora(&receivedData);

  pinMode(LED_GREEN, OUTPUT); // configure PA5(builtin LED) as OUTPUT

  /*Init Timer*/
  MyTim->setOverflow(30, HERTZ_FORMAT); // set interrupt interval ~33ms
  MyTim->attachInterrupt(updateLedState);

#if SW_TYPE == MASTER
    attachInterrupt(digitalPinToInterrupt(BOARD_BTN), ButtonClickInterrupt, RISING);

#endif // SW_TYPE == MASTER

#if SW_TYPE == SLAVE
   ADC_Init();

#endif // SW_TYPE == SLAVE
}

void loop()
{

  // TODO: ADD HEARTBEAT FUNCTION
  #if SW_TYPE == MASTER
    if (interruptState)
    {
      MyTim->resume(); // Start TIM2 and blinking
      LoRa::SendRequest();    // Should communication be done here or in lora.cpp?
      BOOL(interruptState);
    }

    if (loraRadio.read(receivedMessage) > 0)
    {
      //LoRa::ReadResponse(&receivedData, receivedMessage);
      LoRa::ReadData(receivedMessage);
    }

  #elif SW_TYPE == SLAVE
    if (loraRadio.read(requestMessage) == 0x04)
    {
      MyTim->resume(); // Start TIM2 and blinking
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


