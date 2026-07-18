/*
 * alcohol_sensor.h
 *
 * MQ-3 alcohol sensor driver using STM32 HAL ADC.
 * Assumes the MQ-3 analog output is wired to a single-ended ADC channel
 * configured in CubeMX (e.g. ADC1, Channel x) with handle `hadc1`.
 */

#ifndef ALCOHOL_SENSOR_H
#define ALCOHOL_SENSOR_H

#include "stm32f1xx_hal.h"   /* Swap for your series, e.g. stm32f4xx_hal.h */
#include <stdint.h>

/* Calibrate this against your specific MQ-3 module and supply voltage. */
#define ALCOHOL_THRESHOLD   1600U   /* 12-bit ADC (0-4095) example value */
#define ALCOHOL_SAMPLES     5U

/**
 * @brief  Initializes the alcohol sensor module (no extra HW init needed
 *         beyond what CubeMX already configured for the ADC peripheral;
 *         this function exists for symmetry / future calibration routines).
 */
void AlcoholSensor_Init(ADC_HandleTypeDef *hadc);

/**
 * @brief  Reads and returns an averaged raw ADC value from the MQ-3 sensor.
 * @return Averaged 12-bit ADC reading (0-4095).
 */
uint16_t AlcoholSensor_Read(void);

/**
 * @brief  Returns 1 if the last read alcohol level is at/above threshold.
 */
uint8_t AlcoholSensor_IsDetected(uint16_t level);

#endif /* ALCOHOL_SENSOR_H */
