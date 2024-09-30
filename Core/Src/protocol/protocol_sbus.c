/*
 * protocol_sbus.c
 *
 *  Created on: 2.11.2018 �.
 *      Author: Cleric
 */

#include "iwdg.h"
#include "protocols.h"
#include "usart.h"
#define TAG "PROTO-SBUS"

void PROTO_SbusDecodeChannels(uint8_t *buf) {
    int inputbits          = 0;
    int inputbitsavailable = 0;

    for (int i = 0; i < MAX_CHANNELS; i++) {
        while (inputbitsavailable < PROTO_SBUS_CH_BITS) {
            inputbits |= *buf++ << inputbitsavailable;
            inputbitsavailable += 8;
        }

        int v       = inputbits & PROTO_SBUS_CH_MASK;

        // OpenTX sends channel data with in its own values. We can prescale them to the standard
        // 1000 - 2000 range. Thanks to @fape for providing the raw data: min   mid   max 172   992
        // 1811

        // http://www.wolframalpha.com/input/?i=linear+fit+%7B172,+1000%7D,+%7B1811,+2000%7D,+%7B992,+1500%7D
        // slightly adjusted to give better results in integer math
        channels[i] = (610127 * v + 895364000) / 1000000;

        inputbitsavailable -= PROTO_SBUS_CH_BITS;
        inputbits >>= PROTO_SBUS_CH_BITS;
    }
}

void PROTO_SbusReader(void) {
    ProtocolState     state = INITIAL_INTERFRAME;
    uint8_t           buf[PROTO_SBUS_FRAME_LEN];
    int               num_fails = 0;
    int               failed;
    HAL_StatusTypeDef ret;
    int               locked = 0;

    while (1) {
        if (num_fails >= PROTO_SBUS_MAX_FAILS) break;

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
                ret = HAL_UART_Receive(&huart2, buf, PROTO_SBUS_FRAME_LEN, FRAME_TIMEOUT);
                if (ret != HAL_OK) break;

                // first and last byte have special values
                if (buf[0] != PROTO_SBUS_FRAME_FIRST_BYTE)
                    // wrong values
                    break;

                // frame looks ok. Decode channels
                PROTO_SbusDecodeChannels(&buf[1]);
                BuildAndSendReport();

                if (!locked) {
                    Logf(TAG, "locked (%s)\n", (uartInvert ? "sbil" : "sbl"));
                    locked = 1;
                }

                failed    = 0;
                num_fails = 0;

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
