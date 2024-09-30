/*
 * protocol_fport.c
 *
 *  Created on: 30.11.2019
 *      Author: Cleric
 */

#include "iwdg.h"
#include "protocols.h"
#include "usart.h"
#define TAG "PROTO-FPORT"

static int readEscapedStream(uint8_t *buf, int num) {
    int ret = 0, i = -1;
    while (++i < num) {
        uint8_t byte;

        ret = HAL_UART_Receive(&huart2, &byte, 1, FRAME_TIMEOUT);
        if (ret != HAL_OK) break;
        if (byte == PROTO_FPORT_ESCAPE_BYTE) {
            // escape byte, we need one more
            ret = HAL_UART_Receive(&huart2, &byte, 1, FRAME_TIMEOUT);
            if (ret != HAL_OK) break;
            buf[i] = byte ^ PROTO_FPORT_ESCAPE_MASK;
        } else {
            buf[i] = byte;
        }
    }

    return ret == HAL_OK;
}

void PROTO_FportReader(void) {
    ProtocolState     state = INITIAL_INTERFRAME;
    uint8_t           buf[PROTO_FPORT_FRAME_LEN];
    int               num_fails = 0;
    int               failed;
    HAL_StatusTypeDef ret;
    int               locked = 0;

    while (1) {
        if (num_fails >= PROTO_FPORT_MAX_FAILS) break;

        ResetWatchdog();

        // assume failed by default
        failed = 1;

        switch (state) {
            case INITIAL_INTERFRAME:
                if (PROTO_WaitForInterframe(INTERFRAME_MS, FRAME_TIMEOUT)) {
                    // found interframe
                    state  = FRAME;
                    failed = 0;
                }
                break;

            case FRAME:
                // get first byte
                ret = HAL_UART_Receive(&huart2, buf, 1, FRAME_TIMEOUT);
                if (ret != HAL_OK) break;

                // wrong first magic byte
                if (buf[0] != PROTO_FPORT_MAGIC_BYTE) break;

                // get the length byte
                if (!readEscapedStream(&buf[1], 1)) break;

                int data_len = buf[1];
                int i        = 1;
                data_len += 4;  // full frame length: add the first magic byte + len + crc + final magic byte

                // get the remaining frame
                if (!readEscapedStream(&buf[2], data_len - 2)) break;

                // check end magic byte
                if (buf[data_len - 1] != PROTO_FPORT_MAGIC_BYTE) break;

                if (buf[2] == PROTO_FPORT_CONTROL_FRAME) {
                    // check crc
                    uint16_t crc = 0;
                    for (i = 1; i < data_len - 1; i++) {
                        crc += buf[i];
                    }
                    crc = (uint8_t)((crc & 0xff) + (crc >> 8));
                    if (crc != PROTO_FPORT_CRC_CORRECT) break;

                    PROTO_SbusDecodeChannels(&buf[3]);
                    BuildAndSendReport();

                    if (!locked) {
                        Logf(TAG, "Locked. (invert: %s)", (uartInvert ? "fpil\n" : "fpl\n"));
                        locked = 1;
                    }

                    num_fails = 0;
                } else {
                    // if not control frame increment num_fails.
                    // It is not really a failure, but this way we can detect if we are receving frames
                    // but never a control frame Normally there should one control and one telemetry
                    // frame
                    num_fails++;
                }

                failed = 0;

                break;
        }

        if (failed) {
            Logf(TAG, "(%d) failed. trying again..\n", num_fails);
            num_fails++;
            state = INITIAL_INTERFRAME;
        }
    }
    Log(TAG, "stopped..\n");
}
