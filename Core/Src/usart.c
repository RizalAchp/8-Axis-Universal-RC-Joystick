/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "usart.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifndef NO_LOG_SERIAL
UART_HandleTypeDef huart1;

void               MX_USART1_UART_Init(void) {
    huart1.Instance          = USART1;
    huart1.Init.BaudRate     = 115200;
    huart1.Init.WordLength   = UART_WORDLENGTH_8B;
    huart1.Init.StopBits     = UART_STOPBITS_1;
    huart1.Init.Parity       = UART_PARITY_NONE;
    huart1.Init.Mode         = UART_MODE_TX;
    huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_CHECK_ERR(HAL_UART_Init(&huart1));
}

void _Log(const char *fmt, ...) {
    static char buf[64] = {0};
    va_list     arg;
    va_start(arg, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);
    // Ensure len is not negative and does not exceed buffer size
    if (len < 0) {
        len = 0;
    } else if (len >= sizeof(buf)) {
        buf[sizeof(buf) - 1] = '\0';
    }
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, len, 200);
}
#endif

#ifdef USART2_ENABLED
UART_HandleTypeDef huart2;
/* USART2 init function */
void MX_USART2_UART_Init(void) {
    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = 115200;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_CHECK_ERR(HAL_UART_Init(&huart2));
}

inline static void UART_Reset() {
    __HAL_RCC_USART2_FORCE_RESET();
    __HAL_RCC_USART2_RELEASE_RESET();
    memset(&huart2, 0, sizeof(huart2));
}

void UART2_Set_115200_8N1(void) {
    UART_Reset();

    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = 115200;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_CHECK_ERR(HAL_UART_Init(&huart2));
}

void UART2_Set_100000_8E2(void) {
    UART_Reset();

    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = 100000;
    huart2.Init.WordLength   = UART_WORDLENGTH_9B;
    huart2.Init.StopBits     = UART_STOPBITS_2;
    huart2.Init.Parity       = UART_PARITY_EVEN;
    huart2.Init.Mode         = UART_MODE_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_CHECK_ERR(HAL_UART_Init(&huart2));
}
#endif

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (uartHandle->Instance == USART1) {
        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin   = USRT_DLOG_TX_Pin;
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(USRT_DLOG_TX_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = USRT_DLOG_RX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(USRT_DLOG_RX_GPIO_Port, &GPIO_InitStruct);

    }
#ifdef USART2_ENABLED
    else if (uartHandle->Instance == USART2) {
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin   = USRT_PROTO_TX_Pin;
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(USRT_PROTO_TX_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = USRT_PROTO_RX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(USRT_PROTO_RX_GPIO_Port, &GPIO_InitStruct);
    }
#endif /* ifndef USART2_ENABLED */
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {
    if (uartHandle->Instance == USART1) {
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, USRT_DLOG_TX_Pin | USRT_DLOG_RX_Pin);

    }
#ifdef USART2_ENABLED
    else if (uartHandle->Instance == USART2) {
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        HAL_GPIO_DeInit(GPIOA, USRT_PROTO_TX_Pin | USRT_PROTO_RX_Pin);
    }
#endif
}
