/*
 * protocols.h
 *
 *  Created on: 2.11.2018 �.
 *      Author: Cleric
 */

#ifndef INC_PROTOCOLS_H_
#define INC_PROTOCOLS_H_

#include <main.h>

#define MAX_CHANNELS        10
#define INTERFRAME_MS       2
#define FRAME_TIMEOUT       100

#define READ_UINT16(buf, i) ((buf)[(i)] | (((buf)[(i) + 1]) << 8))

typedef enum { INITIAL_INTERFRAME, FRAME } ProtocolState;

extern uint16_t           channels[MAX_CHANNELS];
extern uint32_t           lastSentReportTime;
extern int                uartInvert;
extern IWDG_HandleTypeDef hiwdg;

void                      PROTO_Init(int isPorReset);
void                      PROTO_Run(void);

void                      BuildAndSendReport(void);
void                      ClearChannels(void);

#ifdef USART2_ENABLED
_Bool PROTO_WaitForInterframe(uint32_t iframe, uint32_t timeout);
#endif /* ifdef USART2_ENABLED */

#if PROTO_IBUS
void PROTO_IbusReader(void);
void PROTO_IbusIa6Reader(void);
#endif /* if PROTO_IBUS*/

#if PROTO_SBUS
#    ifndef PROTO_SBUS_FRAME_LEN
#        define PROTO_SBUS_FRAME_LEN 25
#    endif /* ifndef PROTO_SBUS_FRAME_LEN */
#    ifndef PROTO_SBUS_MAX_FAILS
#        define PROTO_SBUS_MAX_FAILS 3
#    endif /* ifndef PROTO_SBUS_MAX_FAILS */
#    ifndef PROTO_SBUS_FRAME_FIRST_BYTE
#        define PROTO_SBUS_FRAME_FIRST_BYTE 0x0f
#    endif /* ifndef PROTO_SBUS_FRAME_FIRST_BYTE */
#    ifndef PROTO_SBUS_CH_BITS
#        define PROTO_SBUS_CH_BITS 11
#    endif /* ifndef PROTO_SBUS_CH_BITS */
#    ifndef PROTO_SBUS_CH_MASK
#        define PROTO_SBUS_CH_MASK ((1 << PROTO_SBUS_CH_BITS) - 1);
#    endif /* ifndef PROTO_SBUS_CH_MASK */
void PROTO_SbusReader(void);
void PROTO_SbusDecodeChannels(uint8_t *buf);
#endif /* if PROTO_SBUS */

#if PROTO_DSM
#    ifndef PROTO_DSM_FRAME_LEN
#        define PROTO_DSM_FRAME_LEN 16
#    endif /* ifndef PROTO_DSM_FRAME_LEN */
#    ifndef PROTO_DSM_MAX_FAILS
#        define PROTO_DSM_MAX_FAILS 3
#    endif /* ifndef PROTO_DSM_MAX_FAILS */
void PROTO_DsmReader(void);
#endif /* if PROTO_DSM */

#if PROTO_PPM
#    ifndef PROTO_PPM_TIMEOUT_MS
#        define PROTO_PPM_TIMEOUT_MS 100
#    endif /* ifndef PROTO_PPM_TIMEOUT_MS */
#    ifndef PROTO_PPM_MIN_PULSE
#        define PROTO_PPM_MIN_PULSE 600
#    endif /* ifndef PROTO_PPM_MIN_PULSE */
#    ifndef PROTO_PPM_MAX_PULSE
#        define PROTO_PPM_MAX_PULSE 2400
#    endif /* ifndef PROTO_PPM_MAX_PULSE */
#    ifndef PROTO_PPM_END_PULSE
#        define PROTO_PPM_END_PULSE 3000
#    endif /* ifndef PROTO_PPM_END_PULSE */
void PROTO_PpmReader(void);
#endif /* if PROTO_PPM */

#if PROTO_FPORT
#    ifndef PROTO_FPORT_FRAME_LEN
#        define PROTO_FPORT_FRAME_LEN 0xff
#    endif /* ifndef PROTO_FPORT_FRAME_LEN */
#    ifndef PROTO_FPORT_MAX_FAILS
#        define PROTO_FPORT_MAX_FAILS 4
#    endif /* ifndef PROTO_FPORT_MAX_FAILS */
#    ifndef PROTO_FPORT_MAGIC_BYTE
#        define PROTO_FPORT_MAGIC_BYTE 0x7e
#    endif /* ifndef PROTO_FPORT_MAGIC_BYTE */
#    ifndef PROTO_FPORT_ESCAPE_BYTE
#        define PROTO_FPORT_ESCAPE_BYTE 0x7d
#    endif /* ifndef PROTO_FPORT_ESCAPE_BYTE */
#    ifndef PROTO_FPORT_ESCAPE_MASK
#        define PROTO_FPORT_ESCAPE_MASK 0x20
#    endif /* ifndef PROTO_FPORT_ESCAPE_MASK */
#    ifndef PROTO_FPORT_CONTROL_FRAME
#        define PROTO_FPORT_CONTROL_FRAME 0x00
#    endif /* ifndef PROTO_FPORT_CONTROL_FRAME */
#    ifndef PROTO_FPORT_CRC_CORRECT
#        define PROTO_FPORT_CRC_CORRECT 0xff
#    endif /* ifndef PROTO_FPORT_CRC_CORRECT */
void PROTO_FportReader(void);
#endif /* if PROTO_FPORT */

#endif /* INC_PROTOCOLS_H_ */
