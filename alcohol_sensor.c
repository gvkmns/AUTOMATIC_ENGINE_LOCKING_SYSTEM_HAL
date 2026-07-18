/*
 * alcohol_sensor.c
 */

#include "alcohol_sensor.h"

static ADC_HandleTypeDef *s_hadc = NULL;

void AlcoholSensor_Init(ADC_HandleTypeDef *hadc)
{
    s_hadc = hadc;
    /* Calibrate ADC if the series/HAL supports it, e.g.:
     * HAL_ADCEx_Calibration_Start(s_hadc); (F3/F4/L4 families) */
}

uint16_t AlcoholSensor_Read(void)
{
    uint32_t total = 0;

    if (s_hadc == NULL)
    {
        return 0U;
    }

    for (uint8_t i = 0; i < ALCOHOL_SAMPLES; i++)
    {
        HAL_ADC_Start(s_hadc);
        if (HAL_ADC_PollForConversion(s_hadc, 10) == HAL_OK)
        {
            total += HAL_ADC_GetValue(s_hadc);
        }
        HAL_ADC_Stop(s_hadc);
        HAL_Delay(2);
    }

    return (uint16_t)(total / ALCOHOL_SAMPLES);
}

uint8_t AlcoholSensor_IsDetected(uint16_t level)
{
    return (level >= ALCOHOL_THRESHOLD) ? 1U : 0U;
}
