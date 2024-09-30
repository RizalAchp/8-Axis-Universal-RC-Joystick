/*
 * protocol_common.c
 *
 *  Created on: 2.11.2018 �.
 *      Author: Cleric
 */

#include <stm32f1xx_hal.h>

#include "gpio.h"
#include "iwdg.h"
#include "protocols.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "usbd_custom_hid_if.h"

#define TAG "PROTO"

uint16_t         channels[MAX_CHANNELS] = {0};
uint32_t         lastSentReportTime;
int              uartInvert;

static JoyReport report;

void             PROTO_Init(int isPorReset) {
    MX_GPIO_Init();
    if (!isPorReset) {
        GPIO_USB_Reset();
    }
    MX_USB_DEVICE_Init();
    MX_USART1_UART_Init();
#ifdef USART2_ENABLED
    MX_USART2_UART_Init();
#endif
    MX_IWDG_Init();
#ifdef PROTO_PPM
    MX_TIM3_Init();
#endif

    Log(TAG, "Initialized...\n");
    if (!isPorReset) {
        Log(TAG, "USB Reset\n");
    }
}

void PROTO_Run(void) {
#if PROTO_IBUS
    uartInvert = 0;
    UART2_Set_115200_8N1();
    PROTO_IbusReader();

    UART2_Set_115200_8N1();
    PROTO_IbusIa6Reader();
#endif /* if PROTO_IBUS */

#if PROTO_SBUS
    UART2_Set_100000_8E2();
    // try inverted first
    uartInvert = 1;
    PROTO_SbusReader();

    UART2_Set_100000_8E2();
    // try uninverted
    uartInvert = 0;
    PROTO_SbusReader();
#endif /* if PROTO_SBUS */

#if PROTO_DSM
    UART2_Set_115200_8N1();
    PROTO_DsmReader();
#endif /* if PROTO_DSM */

#if PROTO_PPM
    GPIO_Data_Out_DeInit();
    HAL_TIM_Base_Init(&htim3);
    HAL_GPIO_WritePin(DATA_OUT_GPIO_Port, DATA_OUT_Pin, GPIO_PIN_RESET);
    PROTO_PpmReader();
    HAL_TIM_Base_DeInit(&htim3);
    GPIO_Data_Out_Init();
#endif /* if PROTO_PPM */

#if PROTO_FPORT
    UART2_Set_115200_8N1();
    // try inverted first
    uartInvert = 1;
    PROTO_FportReader();

    UART2_Set_115200_8N1();
    // try uninverted
    uartInvert = 0;
    PROTO_FportReader();
#endif /* if PROTO_FPORT */

    BuildAndSendReport();
}

void BuildAndSendReport(void) {
    for (int i = 0; i < JOY_REPORT_NUM_AXES; i++) {
        uint16_t ch    = channels[i];
        ch             = MAX(1000, MIN(2000, ch)) - 1000;

        report.axes[i] = ch * 0x7fff / 1000;
    }

    // HID uses top left coordinates origin for X, Y
    // while radios have bottom left origin
    // so invert Y
    // report.axes[1] = 0x7fff - report.axes[1];

    uint8_t buttons = 0;
    for (int i = 0; i < (MAX_CHANNELS - JOY_REPORT_NUM_AXES); i++) {
        if (channels[JOY_REPORT_NUM_AXES + i] >= 1750) buttons |= 1 << i;
    }
    report.buttons = buttons;

    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&report, sizeof(report));
    lastSentReportTime = HAL_GetTick();
}

void ClearChannels(void) { memset(channels, 0, sizeof(channels)); }

#ifdef USART2_ENABLED
/**
 * Waits for interframe interval of silence on the UART RX
 *
 * iframe says how many ms must the line be silent before it is considered
 * interframe timeout is how many ms overall we are willing to wait for the
 * silence, in case data is arriving If 0, try only once
 *
 * Returns 0 if no interframe interval was found (data was arriving more
 * frequently than iframe) Otherwise returns 1
 */
_Bool PROTO_WaitForInterframe(uint32_t iframe, uint32_t timeout) {
    uint8_t           buf[1];
    uint32_t          time = HAL_GetTick() + timeout;
    HAL_StatusTypeDef ret;
    while ((ret = HAL_UART_Receive(&huart2, buf, 1, iframe)) != HAL_TIMEOUT && timeout > 0) {
        if (HAL_GetTick() >= time) break;
    }
    return ret == HAL_TIMEOUT;
}
#endif
