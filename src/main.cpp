#include "main.h"
#include "lora.h"
#include "bme280_sensor.h"
#include "ADC.h"
#include "LiquidCrystal_I2C.h"

uint8_t requestMessage[1];
uint8_t receivedMessage[MESSAGE_SIZE];

bool nextMessage = true;
bool SlaveRequestProcessingFLAG = false;
uint8_t interruptState = 0;
uint32_t currentTime = 0;

volatile int interruptLedCount = 0;

// LoRa::ModuleType_t moduleType = MODULE_TYPE;
DataRead_t sensorData;
DataReceived_t receivedData;

// Create instance of TIM2ja 
HardwareTimer *MyTim = new HardwareTimer(TIM2);

LiquidCrystal_I2C lcd(0x27,  16, 2); 

void turnOffLed(void)
{
  digitalWrite(LED_GREEN, false);
}

void updateLedState(void)
{
  digitalWrite(LED_GREEN, !digitalRead(LED_GREEN)); // Change state of LED
  interruptLedCount++;
  if (interruptLedCount >= 8)
  { // After 8th change -> turn off timer
    MyTim->pause();
    interruptLedCount = 0;
    turnOffLed(); // In case of LED staying turned ON
  }
}

void setup()
{
  pinMode(MODE_PIN, INPUT_PULLUP);
  uint8_t init_error = 0;
  init_error = initRegs();
  Serial.print("[INFO] Init Errors: ");
  Serial.println(init_error);

  // State of PIN PA0
  if (digitalRead(MODE_PIN) == LOW) {
    // state LOW = MASTER
    Serial.println("MASTER mode is ON");
 
  } else {
    // state HIGH = SLAVE
    Serial.println("SLAVE mode is ON");

  }
}

void loop()
{
  if (digitalRead(MODE_PIN) == LOW) 
    if (interruptState)
    {
      MyTim->resume();     // Start TIM2 and blinking
      LoRa::SendRequest(); // Should communication be done here or in lora.cpp?
      BOOL(interruptState);
    }


// TODO: ADD HEARTBEAT FUNCTION
#if SW_TYPE == MASTER
  if (interruptState)
  {
    MyTim->resume();     // Start TIM2 and blinking
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Request Sent");

    LoRa::SendRequest(); // Should communication be done here or in lora.cpp?
    BOOL(interruptState);
  }

  if (loraRadio.read(receivedMessage) > 0)
  {
    // LoRa::ReadResponse(&receivedData, receivedMessage);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Messege");
    lcd.setCursor(0,1);
    lcd.print("Received");

    LoRa::ReadData(receivedMessage);
  }

  } else if (digitalRead(MODE_PIN) == HIGH)
  {
    if (loraRadio.read(requestMessage) == 0x04)
    {
      MyTim->resume(); // Start TIM2 and blinking
      memset(requestMessage, 0, 1);
      Serial.println("[INFO] Data request received");

      BME280::ReadData(&sensorData);
      LoRa::SendResponse(&sensorData);
    }
  }
// the commented CODE was used for SOFTWARE configuration of the MASTER and SLAVE.

  // #if SW_TYPE == MASTER
  //   if (interruptState)
  //   {
  //     MyTim->resume();     // Start TIM2 and blinking
  //     LoRa::SendRequest(); // Should communication be done here or in lora.cpp?
  //     BOOL(interruptState);
  //   }

  //   if (loraRadio.read(receivedMessage) > 0)
  //   {
  //     // LoRa::ReadResponse(&receivedData, receivedMessage);
  //     LoRa::ReadData(receivedMessage);
  //   }

  // #elif SW_TYPE == SLAVE
  //   if (loraRadio.read(requestMessage) == 0x04)
  //     {
  //       MyTim->resume(); // Start TIM2 and blinking
  //       memset(requestMessage, 0, 1);
  //       Serial.println("[INFO] Data request received");

  //       BME280::ReadData(&sensorData);
  //       LoRa::SendResponse(&sensorData);
  //     }
  // #endif
#elif SW_TYPE == SLAVE
  if ((loraRadio.read(requestMessage) == 0x04) && (SlaveRequestProcessingFLAG == false))
  {
    MyTim->resume(); // Start TIM2 and blinking
    memset(requestMessage, 0, 1);
    Serial.println("[INFO] Data request received");
    SlaveRequestProcessingFLAG = true;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Data request");
    lcd.setCursor(0,1);
    lcd.print("received");
  }

  if (SlaveRequestProcessingFLAG)
  {
    if (BME_ACTIVE){
    BME280::ReadData(&sensorData);
    }
    LoRa::SendResponse(&sensorData);

    lcd.setCursor(0,0);
    lcd.print("Data sent");
    delay(2000);
    SlaveRequestProcessingFLAG = false;

    Serial.println("dupa");
  }
  
#endif
}

void ButtonClickInterrupt(void)
{
  BOOL(interruptState);
}

uint8_t initRegs(void)
{
  uint8_t ret = 0;

  /* Init LoRa shield */
  LoRa::ShieldInit(); // dont need to pass module type, it is defined in config.h


      //initialize lcd screen
  lcd.init();
      // turn on the backlight
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Warm LCD");
  lcd.setCursor(0,1);
  lcd.print("Greetings!");

#if SW_TYPE == SLAVE
  ret += BME280::HardwareInit(); // Init BME280 sensor duplicate

  // #endif // SW_TYPE == SLAVE

  /* Init data container structs */
  BME280::DataInit_Bme280(&sensorData); // Init BME280 sensor duplicate
  LoRa::DataInit_Lora(&receivedData);

  pinMode(LED_GREEN, OUTPUT); // configure PA5(builtin LED) as OUTPUT

  /*Init Timer*/
  MyTim->setOverflow(30, HERTZ_FORMAT); // set interrupt interval ~33ms
  MyTim->attachInterrupt(updateLedState);

  if (digitalRead(MODE_PIN) == LOW){
    attachInterrupt(digitalPinToInterrupt(BOARD_BTN), ButtonClickInterrupt, RISING);

  }
  // #if SW_TYPE == MASTER
  //   attachInterrupt(digitalPinToInterrupt(BOARD_BTN), ButtonClickInterrupt, RISING);

  // #endif // SW_TYPE == MASTER
  if (digitalRead(MODE_PIN) == HIGH){
    ret += ADC_Init();
  }
  // #if SW_TYPE == SLAVE
  //   ret += ADC_Init();
  // #endif // SW_TYPE == SLAVE

  return ret;
}
