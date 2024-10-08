/*
 * proto_dsm.c
 *
 *  Created on: 4.11.2018 �.
 *      Author: Cleric
 */

#include "iwdg.h"
#include "protocols.h"
#include "usart.h"

#define TAG                 "PROTO-DSM"

#define MODE_22MS_1024_DSM2 0x01
#define MODE_11MS_2048_DSM2 0x12
#define MODE_22MS_2048_DSMS 0xa2
#define MODE_11MS_2048_DSMX 0xb2

// map Spektrum channel numbers to our order
static const uint8_t CHANNEL_MAP[] = {2,     // 0 Throttle
                                      0,     // 1 Aileron
                                      1,     // 2 Elevat
                                      3,     // 3 Rudder
                                      4,     // 4 Gear
                                      5,     // 5 Aux 1
                                      6,     // 6 Aux 2
                                      7,     // 7 Aux 3
                                      8,     // 8 Aux 4
                                      9,     // 9 Aux 5
                                      0xff,  // 10 Aux 6
                                      0xff,  // 11 Aux 7,
                                      0xff,  // pad to 16 channels
                                      0xff, 0xff, 0xff};

void                 PROTO_DsmReader(void) {
    ProtocolState state = INITIAL_INTERFRAME;
    uint8_t       buf[PROTO_DSM_FRAME_LEN];
    int           num_fails = 0;
    int           i;
    int           failed;
    int           locked = 0;

    ClearChannels();

    while (1) {
        if (num_fails >= PROTO_DSM_MAX_FAILS) break;

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
                if (HAL_UART_Receive(&huart2, buf, PROTO_DSM_FRAME_LEN, FRAME_TIMEOUT) != HAL_OK)
                    // could not read frame
                    break;

                int mode = buf[1];

                if (mode != MODE_22MS_1024_DSM2 && mode != MODE_11MS_2048_DSM2 && mode != MODE_22MS_2048_DSMS &&
                    mode != MODE_11MS_2048_DSMX)
                    break;

                for (i = 2; i < PROTO_DSM_FRAME_LEN; i += 2) {
                    int      chId;
                    uint16_t chVal;

                    if (mode == MODE_22MS_1024_DSM2) {
                        // 1024 mode
                        chVal = ((buf[i] & 3) << 8) | buf[i + 1];
                        chVal <<= 1;  // make it 2048
                        chId = (buf[i] >> 2) & 0xf;
                    } else {
                        // 2048 mode
                        chVal = ((buf[i] & 7) << 8) | buf[i + 1];
                        chId  = (buf[i] >> 3) & 0xf;
                    }

                    chId = CHANNEL_MAP[chId];
                    if (chId < MAX_CHANNELS) {
                        // spektrum mapping: (341, 1707) -> (1000, 2000)
                        channels[chId] = 1000 * (chVal + 1025) / 1366;
                    }
                }

                BuildAndSendReport();

                if (!locked) {
                    Log(TAG, "locked\n");
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
