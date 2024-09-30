/**
 ******************************************************************************
 * @file    iwdg.c
 * @brief   This file provides code for the configuration
 *          of the IWDG instances.
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
#include "iwdg.h"

IWDG_HandleTypeDef hiwdg;

/* IWDG init function */
void MX_IWDG_Init(void) {
    hiwdg.Instance       = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_16;
    hiwdg.Init.Reload    = 4095;
    HAL_CHECK_ERR(HAL_IWDG_Init(&hiwdg));
}

void ResetWatchdog() {
    // reset/refresh the watch dog
    HAL_IWDG_Refresh(&hiwdg);
}
