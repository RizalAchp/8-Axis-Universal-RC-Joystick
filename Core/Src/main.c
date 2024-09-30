/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
/* Private includes ----------------------------------------------------------*/
#include "protocols.h"
#include "usart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* Private user code ---------------------------------------------------------*/

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* MCU Configuration--------------------------------------------------------*/
    int isPorReset = __HAL_RCC_GET_FLAG(RCC_FLAG_PORRST);
    __HAL_RCC_CLEAR_RESET_FLAGS();
    /* Reset of all peripherals, Initializes the Flash interface and the Systick.
     */
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();
    /* Initialize all configured peripherals */
    PROTO_Init(isPorReset);

    /* Infinite loop */
    while (1) {
        PROTO_Run();
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType           = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState                 = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue           = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState                 = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState                 = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState             = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource            = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL               = RCC_PLL_MUL9;
    HAL_CHECK_ERR(HAL_RCC_OscConfig(&RCC_OscInitStruct));

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_CHECK_ERR(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2));
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection    = RCC_USBCLKSOURCE_PLL_DIV1_5;
    HAL_CHECK_ERR(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit));
}

static inline const char *HAL_StatusCstr(int sts) {
    switch (sts) {
        case HAL_ERROR: return "ERROR";
        case HAL_BUSY: return "BUSY";
        case HAL_TIMEOUT: return "TIMEOUT";
        case HAL_OK: return "OK";
        default: return "UNKNOWN";
    }
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void _Error_Handler(const char *file, int line, int sts) {
    Logf("FATAL_ERROR", "%s:%d (Status: %s)", file, line, HAL_StatusCstr(sts));
    __disable_irq();
    while (1) {
    }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* ASSERT LOG */
    Log("ASSERT_FAILED", "%s:%d", file, line);
}
#endif /* USE_FULL_ASSERT */
