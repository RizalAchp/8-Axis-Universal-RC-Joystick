/**
 ******************************************************************************
 * @file           : usb_device.c
 * @version        : v2.0_Cube
 * @brief          : This file implements the USB Device
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
#include "usb_device.h"

#include "usbd_core.h"
#include "usbd_custom_hid_if.h"
#include "usbd_customhid.h"
#include "usbd_desc.h"
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceFS;

/**
 * Init USB device Library, add supported class and start the library
 * @retval None
 */
void MX_USB_DEVICE_Init(void) {
    /* Init Device Library, add supported class and start the library. */
    HAL_CHECK_ERR(USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS));
    HAL_CHECK_ERR(USBD_RegisterClass(&hUsbDeviceFS, &USBD_CUSTOM_HID));
    HAL_CHECK_ERR(USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS, &USBD_CustomHID_fops_FS));
    HAL_CHECK_ERR(USBD_Start(&hUsbDeviceFS));
}

void GPIO_USB_Reset(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin   = GPIO_PIN_12;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    uint32_t time = HAL_GetTick() + 1000;
    int      i    = 0;
    while (HAL_GetTick() < time) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, i++ & 1);
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
}
