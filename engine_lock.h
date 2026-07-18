/*
 * engine_lock.h
 *
 * Engine immobilization actuator control. Drives an H-bridge (e.g. L298N)
 * whose output stands in for the vehicle's starter/ignition relay:
 *   - "engine running" = motor driven / relay energized
 *   - "engine locked"  = motor stopped / relay de-energized
 *
 * Requires two GPIO pins (direction) and one PWM timer channel (speed/
 * enable) already configured via CubeMX.
 */

#ifndef ENGINE_LOCK_H
#define ENGINE_LOCK_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef   *in1_port;
    uint16_t        in1_pin;
    GPIO_TypeDef   *in2_port;
    uint16_t        in2_pin;
    TIM_HandleTypeDef *pwm_timer;
    uint32_t        pwm_channel;
} EngineLock_HandleTypeDef;

void    EngineLock_Init(EngineLock_HandleTypeDef *heng);
void    EngineLock_Start(EngineLock_HandleTypeDef *heng);   /* unlock/run  */
void    EngineLock_Stop(EngineLock_HandleTypeDef *heng);    /* lock/halt  */
uint8_t EngineLock_IsLocked(EngineLock_HandleTypeDef *heng);

#endif /* ENGINE_LOCK_H */
