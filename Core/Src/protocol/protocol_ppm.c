/*
 * protocol_ppm.c
 *
 *  Created on: 5.11.2018 �.
 *      Author: Cleric
 */

#include "iwdg.h"
#include "protocols.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"
#define TAG "PROTO-PPM"

void PROTO_PpmReader(void) {
    int32_t interFrame = 0, chId = 0, locked = 0, pulse = 0;
    ProtocolState state = INITIAL_INTERFRAME;
    uint32_t timeout = HAL_GetTick() + PROTO_PPM_TIMEOUT_MS;

    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_2);
    htim3.Instance->CNT = 0;
    ClearChannels();

    while (1) {
        if (HAL_GetTick() > timeout)
            break;

        ResetWatchdog();

        if (htim3.Instance->CNT > PROTO_PPM_END_PULSE) {
            if (state == FRAME && chId > 0) {
                // got to the end of the frame
                BuildAndSendReport();

                if (!locked) {
                    locked = 1;
                    Log(TAG, "locked\n");
                }

                timeout = HAL_GetTick() + PROTO_PPM_TIMEOUT_MS;
                state = INITIAL_INTERFRAME;
            }
            interFrame = 1;
        }

        if (htim3.Instance->SR & TIM_SR_CC2IF) { // check if capture interrupt flag is set
            pulse = htim3.Instance->CCR2;        // get the capture count (int flag is cleared)

            switch (state) {
            case INITIAL_INTERFRAME:
                if (interFrame) {
                    interFrame = 0;
                    chId = 0;
                    state = FRAME;
                }
                // else // pulses while NOT in interframe occur after invalid pulse length
                //   HAL_UART_Transmit_DMA(&huart2, (uint8_t*)"pi\n", 3);
                break;

            case FRAME:
                if (pulse < PROTO_PPM_MIN_PULSE || pulse > PROTO_PPM_MAX_PULSE) {
                    // invalid pulse width
                    state = INITIAL_INTERFRAME;
                    // HAL_UART_Transmit_DMA(&huart2, (uint8_t*)"pp\n", 3);
                } else {
                    if (chId < MAX_CHANNELS)
                        channels[chId++] = pulse;
                }
                break;
            }
        }
    }

    Log(TAG, "stopped..\n");
}
