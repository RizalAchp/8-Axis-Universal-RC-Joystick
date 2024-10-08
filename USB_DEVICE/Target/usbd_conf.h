/**
 ******************************************************************************
 * @file           : usbd_conf.h
 * @version        : v2.0_Cube
 * @brief          : Header for usbd_conf.c file.
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
#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------- -----------*/
#define USBD_MAX_NUM_INTERFACES           1
/*---------- -----------*/
#define USBD_MAX_NUM_CONFIGURATION        1
/*---------- -----------*/
#define USBD_MAX_STR_DESC_SIZ             512
/*---------- -----------*/
#define USBD_SUPPORT_USER_STRING          1
/*---------- -----------*/
#define USBD_DEBUG_LEVEL                  0
/*---------- -----------*/
#define USBD_SELF_POWERED                 0
/*---------- -----------*/
#define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE 64
/*---------- -----------*/
#define USBD_CUSTOM_HID_REPORT_DESC_SIZE  55
/*---------- -----------*/
#define CUSTOM_HID_FS_BINTERVAL           0xA

/****************************************/
/* #define for FS and HS identification */
#define DEVICE_FS                         0

/* Memory management macros */

/** Alias for memory allocation. */
#define USBD_malloc                       (uint32_t *)USBD_static_malloc
/** Alias for memory release. */
#define USBD_free                         USBD_static_free
/** Alias for memory set. */
#define USBD_memset                       /* Not used */
/** Alias for memory copy. */
#define USBD_memcpy                       /* Not used */
/** Alias for delay. */
#define USBD_Delay                        HAL_Delay

/* For footprint reasons and since only one allocation is handled in the HID class
   driver, the malloc/free is changed into a static allocation method */
void *USBD_static_malloc(uint32_t size);
void  USBD_static_free(void *p);

/* DEBUG macros */

#if (USBD_DEBUG_LEVEL > 0)
#    define USBD_UsrLog(...) \
        printf(__VA_ARGS__); \
        printf("\n");
#else
#    define USBD_UsrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 1)

#    define USBD_ErrLog(...) \
        printf("ERROR: ");   \
        printf(__VA_ARGS__); \
        printf("\n");
#else
#    define USBD_ErrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 2)
#    define USBD_DbgLog(...) \
        printf("DEBUG : ");  \
        printf(__VA_ARGS__); \
        printf("\n");
#else
#    define USBD_DbgLog(...)
#endif

/* Exported functions -------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CONF__H__ */
