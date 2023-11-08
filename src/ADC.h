#ifndef ADC_H
#define ADC_H

#include <Arduino.h>

extern ADC_HandleTypeDef hadc1;

uint8_t ADC_Init(void);
uint32_t ADC_Read(void);
float mapADCValueToPercentage(int adcValue);

#endif // ADC_H