/*
 * engine_lock.c
 */

#include "engine_lock.h"

static uint8_t s_locked = 0U;

void EngineLock_Init(EngineLock_HandleTypeDef *heng)
{
    /* Ensure engine starts in the "running/unlocked" state at boot */
    EngineLock_Start(heng);
    HAL_TIM_PWM_Start(heng->pwm_timer, heng->pwm_channel);
}

void EngineLock_Start(EngineLock_HandleTypeDef *heng)
{
    HAL_GPIO_WritePin(heng->in1_port, heng->in1_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(heng->in2_port, heng->in2_pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(heng->pwm_timer, heng->pwm_channel, 800); /* ~run duty */
    s_locked = 0U;
}

void EngineLock_Stop(EngineLock_HandleTypeDef *heng)
{
    HAL_GPIO_WritePin(heng->in1_port, heng->in1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(heng->in2_port, heng->in2_pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(heng->pwm_timer, heng->pwm_channel, 0);
    s_locked = 1U;
}

uint8_t EngineLock_IsLocked(EngineLock_HandleTypeDef *heng)
{
    (void)heng;
    return s_locked;
}
