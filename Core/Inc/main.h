/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_IC_Pin GPIO_PIN_13
#define LED_IC_GPIO_Port GPIOC
#define USRT_PROTO_TX_Pin GPIO_PIN_2
#define USRT_PROTO_TX_GPIO_Port GPIOA
#define USRT_PROTO_RX_Pin GPIO_PIN_3
#define USRT_PROTO_RX_GPIO_Port GPIOA
#define DATA_OUT_Pin GPIO_PIN_4
#define DATA_OUT_GPIO_Port GPIOA
#define DATA_IN_Pin GPIO_PIN_7
#define DATA_IN_GPIO_Port GPIOA
#define USRT_DLOG_TX_Pin GPIO_PIN_9
#define USRT_DLOG_TX_GPIO_Port GPIOA
#define USRT_DLOG_RX_Pin GPIO_PIN_10
#define USRT_DLOG_RX_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
