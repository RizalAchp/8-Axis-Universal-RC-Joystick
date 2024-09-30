/**
 ******************************************************************************
 * @file           : usbd_custom_hid_if.h
 * @version        : v2.0_Cube
 * @brief          : Header for usbd_custom_hid_if.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CUSTOM_HID_IF_H__
#define __USBD_CUSTOM_HID_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_customhid.h"

/** CUSTOMHID Interface callback. */
extern USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS;

#define JOY_REPORT_NUM_AXES    8
#define JOY_REPORT_NUM_BUTTONS 8

typedef struct {
    uint16_t axes[JOY_REPORT_NUM_AXES];
    uint8_t  buttons;
} __attribute__((packed)) JoyReport;

/**
 * @brief  Send the report to the Host
 * @param  report: The report to be sent
 * @param  len: The report length
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t USBD_Send_JoystickReport(JoyReport *report);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CUSTOM_HID_IF_H__ */
