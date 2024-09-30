/**
 ******************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 *          of the TIM instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

#include "main.h"

TIM_HandleTypeDef htim3;

/* TIM3 init function */
void MX_TIM3_Init(void) {
    TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef  sSlaveConfig       = {0};
    TIM_MasterConfigTypeDef sMasterConfig      = {0};
    TIM_IC_InitTypeDef      sConfigIC          = {0};

    htim3.Instance                             = TIM3;
    htim3.Init.Prescaler                       = 71;
    htim3.Init.CounterMode                     = TIM_COUNTERMODE_UP;
    htim3.Init.Period                          = 65535;
    htim3.Init.ClockDivision                   = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload               = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_CHECK_ERR(HAL_TIM_Base_Init(&htim3));

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_CHECK_ERR(HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig));
    HAL_CHECK_ERR(HAL_TIM_IC_Init(&htim3));

    sSlaveConfig.SlaveMode       = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger    = TIM_TS_TI2FP2;
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerFilter   = 0;
    HAL_CHECK_ERR(HAL_TIM_SlaveConfigSynchro(&htim3, &sSlaveConfig));

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_CHECK_ERR(HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig));

    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    HAL_CHECK_ERR(HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2));
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (tim_baseHandle->Instance == TIM3) {
        /* TIM3 clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM3 GPIO Configuration
        PA7     ------> TIM3_CH2
        */
        GPIO_InitStruct.Pin  = DATA_IN_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DATA_IN_GPIO_Port, &GPIO_InitStruct);

        __HAL_AFIO_REMAP_TIM3_PARTIAL();
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle) {
    if (tim_baseHandle->Instance == TIM3) {
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();

        /**TIM3 GPIO Configuration
        PA7     ------> TIM3_CH2
        */
        HAL_GPIO_DeInit(DATA_IN_GPIO_Port, DATA_IN_Pin);
    }
}
