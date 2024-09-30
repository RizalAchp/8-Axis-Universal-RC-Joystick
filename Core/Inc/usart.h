/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.h
 * @brief   This file contains all the function prototypes for
 *          the usart.c file
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
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#ifdef USART2_ENABLED
extern UART_HandleTypeDef huart2;
#endif

#if defined(NO_LOG_SERIAL)
#    define MX_USART1_UART_Init()
#    define Log(TAG, FMT)
#    define Logf(TAG, FMT, ...)
#else
extern UART_HandleTypeDef huart1;
void                      MX_USART1_UART_Init(void);
void                      _Log(const char *fmt, ...) __attribute__((__format__(__printf__, 1, 2)));
#    define Log(TAG, FMT)       _Log(TAG ":" FMT)
#    define Logf(TAG, FMT, ...) _Log(TAG ":" FMT, __VA_ARGS__)
#endif

#ifdef USART2_ENABLED
void MX_USART2_UART_Init(void);
void UART2_Set_115200_8N1(void);
void UART2_Set_100000_8E2(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
