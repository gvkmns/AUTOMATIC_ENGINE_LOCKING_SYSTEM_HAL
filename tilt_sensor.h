/*
 * tilt_sensor.h
 *
 * Tilt / rollover sensor driver (e.g. SW-520D ball-tilt switch or a
 * mercury tilt switch) wired to a single digital GPIO input. The
 * switch closes/opens its contact when the vehicle tips beyond a
 * mechanical angle, independent of the alcohol-detection path -- this
 * lets the system also lock the engine (and send a location alert) if
 * the vehicle has rolled over or tilted sharply, e.g. after a crash.
 */

#ifndef TILT_SENSOR_H
#define TILT_SENSOR_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t      pin;
    uint8_t       active_low;   /* 1 if switch pulls the pin LOW when tilted */
} TiltSensor_HandleTypeDef;

/**
 * @brief Debounce window for tilt detection, in consecutive samples,
 *        to reject momentary bumps (potholes, gear changes, etc.).
 */
#define TILT_DEBOUNCE_SAMPLES  3U

void    TiltSensor_Init(TiltSensor_HandleTypeDef *htilt);

/**
 * @brief Polls the tilt switch once. Call this on the same periodic
 *        cadence as the alcohol sensor sampling loop.
 * @return 1 once the debounce threshold of consecutive tilted readings
 *         is reached, 0 otherwise. Internally tracks its own counter.
 */
uint8_t TiltSensor_Poll(TiltSensor_HandleTypeDef *htilt);

/**
 * @brief Clears the internal debounce counter/state, e.g. after a
 *        manual system reset.
 */
void    TiltSensor_Reset(void);

#endif /* TILT_SENSOR_H */
