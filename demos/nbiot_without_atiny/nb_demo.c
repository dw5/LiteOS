/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Nb Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#ifndef LOSCFG_COMPONENTS_NET_AT_BC95
#error This demo needs to enable Components "Components-->Network-->Enable Network-->Enable AT-->Choose AT Device (Enable bc95)"
#endif

#include "stdio.h"
#include "nb_demo.h"

#ifdef LOSCFG_COMPONENTS_CONNECTIVITY_NB_IOT
#include "nb_iot/los_nb_api.h"
#endif

#define TELECON_IP          "119.3.250.80"
#define OCEAN_IP            "139.159.140.34"
#define SECURITY_PORT       "5684"
#define NON_SECURITY_PORT   "5683"
#define DEV_PSKID           "868744031131026"
#define DEV_PSK             "d1e1be0c05ac5b8c78ce196412f0cdb0"

VOID NBIoT_DemoEntry(VOID)
{
#if LOSCFG_DEMOS_NBIOT_DTLS
    sec_param_s sec;
    sec.setpsk = 1;
    sec.pskid = DEV_PSKID;
    sec.psk = DEV_PSK;
#endif

    printf("NB-IoT demo task start to run.\n");
#if LOSCFG_DEMOS_NBIOT_DTLS
    los_nb_init((const int8_t *)TELECON_IP, (const int8_t *)SECURITY_PORT, &sec);
#else
    los_nb_init((const int8_t *)TELECON_IP, (const int8_t *)NON_SECURITY_PORT, NULL);
#endif

#if defined(WITH_SOTA)
    NBIoT_SotaDemo();
#endif
    while (1) {
        los_nb_report("22", 2); // "22" is a random string, 2 is the string length
        LOS_TaskDelay(60000);
    }
    printf("NB-IoT demo task start to run.\n");
}
