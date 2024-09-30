/*
 * protocol_ibus_ia6.c
 *
 *  Created on: 4.11.2018 �.
 *      Author: Cleric
 */

#include "iwdg.h"
#include "protocols.h"
#include "usart.h"
#define TAG "PROTO-IBUS_IA6"

#ifndef PROTO_IBUS_IA6_FRAME_LEN
#    define PROTO_IBUS_IA6_FRAME_LEN 31
#endif /* ifndef PROTO_IBUS_IA6_FRAME_LEN */
#ifndef PROTO_IBUS_IA6_MAX_FAILS
#    define PROTO_IBUS_IA6_MAX_FAILS 3
#endif /* ifndef PROTO_IBUS_IA6_MAX_FAILS */
#ifndef PROTO_IBUS_IA6_IBUS_MAGIC
#    define PROTO_IBUS_IA6_IBUS_MAGIC 0x55
#endif /* ifndef PROTO_IBUS_IA6_IBUS_MAGIC */

void PROTO_IbusIa6Reader(void) {
    ProtocolState state = INITIAL_INTERFRAME;
    uint8_t       buf[PROTO_IBUS_IA6_FRAME_LEN];
    int           num_fails = 0;
    uint16_t      checksum  = 0;
    int           idx       = 0;
    int           i;
    int           failed;
    int           locked = 0;

    while (1) {
        if (num_fails >= PROTO_IBUS_IA6_MAX_FAILS) break;

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
                if (HAL_UART_Receive(&huart2, buf, PROTO_IBUS_IA6_FRAME_LEN, FRAME_TIMEOUT) != HAL_OK) break;

                if (buf[0] != PROTO_IBUS_IA6_IBUS_MAGIC) break;

                // init checksum
                checksum = 0;

                // read channels
                idx      = 1;
                i        = 0;
                while (idx < PROTO_IBUS_IA6_FRAME_LEN - 2 /*exclude checksum*/) {
                    uint16_t chVal = READ_UINT16(buf, idx);
                    if (i < MAX_CHANNELS) {
                        channels[i] = chVal & 0xfff;
                    }
                    checksum += chVal;

                    idx += 2;
                    i++;
                }

                // validate checksum
                if (checksum == READ_UINT16(buf, idx)) {
                    // checksum valid
                    BuildAndSendReport();
                    if (!locked) {
                        Log(TAG, "locked (i6l)\n");
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
