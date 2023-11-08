#include "ADC.h"
#include "config.h"

#if SW_TYPE == SLAVE

ADC_HandleTypeDef hadc1;

void ADC_Init() {
  __HAL_RCC_ADC1_CLK_ENABLE();
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);

  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  HAL_ADC_Start(&hadc1);
}

uint32_t ADC_Read() {
  if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK) {
    return HAL_ADC_GetValue(&hadc1);
  }
  return 0;
}

float mapADCValueToPercentage(int adcValue) {
    int minValue = 2900; //Record the sensor value when the probe is exposed to the air
    int maxValue = 1740;  //Record the sensor value when the probe is exposed to the water as

    if (adcValue > minValue) {
        return 0.00;
    } else if (adcValue < maxValue) {
        return 100.00;
    } else {
        float percentage = ((float)(adcValue - minValue) / (maxValue - minValue)) * 100;
        return percentage;
    }
}
#endif // SW_TYPE == SLAVE