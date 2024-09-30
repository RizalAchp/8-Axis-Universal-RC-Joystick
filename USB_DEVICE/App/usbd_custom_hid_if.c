/**
 ******************************************************************************
 * @file           : usbd_custom_hid_if.c
 * @version        : v2.0_Cube
 * @brief          : USB Device Custom HID interface file.
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
#include "usbd_custom_hid_if.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END = {
    /* USER CODE BEGIN 0 */
    0x05, 0x01,                    // Usage Page (Generic Desktop Ctrls)
    0x09, 0x04,                    // Usage (Joystick)
    0xA1, 0x01,                    // Collection (Application)
    0xA1, 0x00,                    //   Collection (Physical)
    0x05, 0x01,                    //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,                    //     Usage (X)
    0x09, 0x31,                    //     Usage (Y)
    0x09, 0x32,                    //     Usage (Z)
    0x09, 0x33,                    //     Usage (Rx)
    0x09, 0x34,                    //     Usage (Ry)
    0x09, 0x35,                    //     Usage (Rz)
    0x09, 0x36,                    //     Usage (Slider)
    0x09, 0x37,                    //     Usage (Dial)
    0x15, 0x00,                    //     Logical Minimum (0)
    0x26, 0xFF, 0x7F,              //     Logical Maximum (32767)
    0x75, 0x10,                    //     Report Size (16)
    0x95, 0x08,                    //     Report Count (8)
    0x81, 0x02,                    //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x09,                    //     Usage Page (Button)
    0x19, 0x01,                    //     Usage Minimum (0x01)
    0x29, 0x01,                    //     Usage Maximum (0x01)
    0x15, 0x00,                    //     Logical Minimum (0)
    0x25, 0x01,                    //     Logical Maximum (1)
    0x75, 0x01,                    //     Report Size (1)
    0x95, JOY_REPORT_NUM_BUTTONS,  //     Report Count (8)
    0x81, 0x02,                    //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                          //   End Collection

    // 61 bytes

    /* USER CODE END 0 */
    0xC0 /*     END_COLLECTION	             */
};

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
 * @brief Public variables.
 * @{
 */
extern USBD_HandleTypeDef  hUsbDeviceFS;

static int8_t              CUSTOM_HID_Init_FS(void);
static int8_t              CUSTOM_HID_DeInit_FS(void);
static int8_t              CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS = {
    CUSTOM_HID_ReportDesc_FS,  // report
    CUSTOM_HID_Init_FS,        // init
    CUSTOM_HID_DeInit_FS,      // deinit
    CUSTOM_HID_OutEvent_FS     // outevent
};

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Initializes the CUSTOM HID media low layer
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CUSTOM_HID_Init_FS(void) {
    /* USER CODE BEGIN 4 */
    return (USBD_OK);
    /* USER CODE END 4 */
}

/**
 * @brief  DeInitializes the CUSTOM HID media low layer
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CUSTOM_HID_DeInit_FS(void) {
    /* USER CODE BEGIN 5 */
    return (USBD_OK);
    /* USER CODE END 5 */
}

/**
 * @brief  Manage the CUSTOM HID class events
 * @param  event_idx: Event index
 * @param  state: Event state
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state) {
    /* USER CODE BEGIN 6 */
    return (USBD_OK);
    /* USER CODE END 6 */
}

int8_t USBD_Send_JoystickReport(JoyReport *report) {
    return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)report, sizeof(JoyReport));
}
