/*
 * tilt_sensor.c
 */

#include "tilt_sensor.h"

static uint8_t s_debounceCount = 0U;

void TiltSensor_Init(TiltSensor_HandleTypeDef *htilt)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin  = htilt->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = htilt->active_low ? GPIO_PULLUP : GPIO_PULLDOWN;
    HAL_GPIO_Init(htilt->port, &GPIO_InitStruct);

    s_debounceCount = 0U;
}

uint8_t TiltSensor_Poll(TiltSensor_HandleTypeDef *htilt)
{
    GPIO_PinState raw = HAL_GPIO_ReadPin(htilt->port, htilt->pin);
    uint8_t tiltedNow = htilt->active_low ? (raw == GPIO_PIN_RESET)
                                           : (raw == GPIO_PIN_SET);

    if (tiltedNow)
    {
        if (s_debounceCount < 0xFF)
        {
            s_debounceCount++;
        }
    }
    else
    {
        s_debounceCount = 0U;
    }

    return (s_debounceCount >= TILT_DEBOUNCE_SAMPLES) ? 1U : 0U;
}

void TiltSensor_Reset(void)
{
    s_debounceCount = 0U;
}
