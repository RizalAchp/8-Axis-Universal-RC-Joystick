/*
 * protocol_ibus.c
 *
 *  Created on: 3.11.2018 �.
 *      Author: Cleric
 */

#include "iwdg.h"
#include "protocols.h"
#include "usart.h"

#define TAG "PROTO-IBUS"

#ifndef PROTO_IBUS_FRAME_LEN
#    define PROTO_IBUS_FRAME_LEN 0xff
#endif /* ifndef PROTO_IBUS_FRAME_LEN */
#ifndef PROTO_IBUS_MAX_FAILS
#    define PROTO_IBUS_MAX_FAILS 3
#endif /* ifndef PROTO_IBUS_MAX_FAILS */
#ifndef PROTO_IBUS_CHANNELS_COMMAND
#    define PROTO_IBUS_CHANNELS_COMMAND 0x40
#endif /* ifndef PROTO_IBUS_CHANNELS_COMMAND */

void PROTO_IbusReader(void) {
    ProtocolState     state = INITIAL_INTERFRAME;
    uint8_t           buf[PROTO_IBUS_FRAME_LEN];
    int               num_fails = 0;
    int               len       = 0;
    uint16_t          checksum  = 0;
    int               idx       = 0;
    int               i;
    int               failed;
    HAL_StatusTypeDef ret;
    int               locked = 0;

    while (1) {
        if (num_fails >= PROTO_IBUS_MAX_FAILS) break;

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
                len = 0;

                // get first byte of frame - length
                // if(HAL_UART_Receive(huart, buf, 1, FRAME_TIMEOUT) == HAL_OK)
                //  len = buf[0];
                ret = HAL_UART_Receive(&huart2, buf, 1, FRAME_TIMEOUT);
                if (ret == HAL_OK) len = buf[0];

                if (len > 3 && !(len & 1) /*only even lengths are valid*/) {
                    // load the remaining bytes of the frame
                    // if(HAL_UART_Receive(huart, buf, len-1, FRAME_TIMEOUT) != HAL_OK)
                    ret = HAL_UART_Receive(&huart2, buf, len - 1, FRAME_TIMEOUT);
                    if (ret != HAL_OK) break;
                } else break;

                // init checksum
                checksum = 0xffff;
                checksum -= len;

                // command byte
                if (buf[0] != PROTO_IBUS_CHANNELS_COMMAND)
                    // only command 0x40 is known
                    break;
                checksum -= buf[0];

                // read channels
                idx = 1;
                i   = 0;
                while (idx < len - 3 /*exclude checksum and command byte*/) {
                    if (i < MAX_CHANNELS) {
                        channels[i] = READ_UINT16(buf, idx) & 0xfff;
                    }
                    checksum -= buf[idx];
                    checksum -= buf[idx + 1];

                    idx += 2;
                    i++;
                }

                // validate checksum
                if (checksum == READ_UINT16(buf, idx)) {
                    // checksum valid
                    BuildAndSendReport();

                    if (!locked) {
                        Log(TAG, "Locked (ibl)\n");
                        locked = 1;
                    }

                    failed    = 0;
                    num_fails = 0;
                }

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
