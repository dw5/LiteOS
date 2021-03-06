/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Demo Entry Implementation
 * Author: Huawei LiteOS Team
 * Create: 2020-12-10
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

#include "los_typedef.h"
#include "los_task.h"

#ifdef CONFIG_FEATURE_FOTA
#include "ota_port.h"
#endif /* CONFIG_FEATURE_FOTA */

#ifdef LOSCFG_COMPONENTS_NET_LWIP
#include "sys_init.h"
#endif /* LOSCFG_COMPONENTS_NET_LWIP */

#ifdef LOSCFG_COMPONNETS_NET_AT
#include "nb_iot/los_nb_api.h"
#include "at_frame/at_api.h"
#endif /* LOSCFG_COMPONNETS_NET_AT */

#ifdef LOSCFG_COMPONENTS_NET_AT_ESP8266
#include "esp8266.h"
#endif /* LOSCFG_COMPONENTS_NET_AT_ESP8266 */

#ifdef LOSCFG_COMPONENTS_NET_AT_BG36
#include "bg36.h"
#endif /* LOSCFG_COMPONENTS_NET_AT_BG36 */

#ifdef LOSCFG_COMPONENTS_NET_AT_SIM900A
#include "sim900a.h"
#endif /* LOSCFG_COMPONENTS_NET_AT_SIM900A */

#ifdef LOSCFG_COMPONENTS_NET_AT_BC95
#include "bc95.h"
#endif /* LOSCFG_COMPONENTS_NET_AT_BC95 */

#ifdef LOSCFG_GUI_ENABLE
#include "lvgl_demo.h"
#endif /* LOSCFG_GUI_ENABLE */

#ifdef LOSCFG_DEMOS_AGENT_TINY_MQTT
#include "flash_adaptor.h"
#include "agent_tiny_mqtt_demo.h"
#endif /* LOSCFG_DEMOS_AGENT_TINY_MQTT */

#ifdef LOSCFG_DEMOS_AGENT_TINY_LWM2M
#include "agent_tiny_lwm2m_demo.h"
#endif /* LOSCFG_DEMOS_AGENT_TINY_MQTT */

#ifdef LOSCFG_DEMOS_SENSORHUB
#include "sensorhub_demo.h"
#endif /* LOSCFG_DEMOS_AGENT_TINY_LWM2M */

#ifdef LOSCFG_DEMOS_KERNEL
#include "los_inspect_entry.h"
#include "los_demo_entry.h"
#endif /* LOSCFG_DEMOS_KERNEL */

#ifdef LOSCFG_DEMOS_DTLS_SERVER
#include "test_dtls_server.h"
#endif /* LOSCFG_DEMOS_DTLS_SERVER */

#ifdef LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY
#include "nb_demo.h"
#endif /* LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY */

#ifdef LOSCFG_DEMOS_LMS
#include "lms_demo.h"
#endif /* LOSCFG_DEMOS_LMS */

#ifdef LOSCFG_DEMOS_TRACE
#include "trace_demo.h"
#endif /* LOSCFG_DEMOS_TRACE */

#ifdef LOSCFG_SHELL
#include "shell.h"
#include "shcmd.h"
#endif /* LOSCFG_SHELL */

#ifdef LOSCFG_DEMOS_IPV6_CLIENT
#include "client_demo.h"
#endif /* LOSCFG_DEMOS_IPV6_CLIENT */

#ifdef LOSCFG_DEMOS_FS
#include "fs_demo.h"
#endif

#ifdef LOSCFG_DEMOS_AI
#include "ai_demo.h"
#endif

#ifdef LOSCFG_DEMOS_ICONV
#include "iconv_demo.h"
#endif

#ifdef LOSCFG_DEMOS_INIPARSER
#include "iniparser_demo.h"
#endif /* LOSCFG_DEMOS_INIPARSER */

#ifdef LOSCFG_DEMOS_JSON_C
#include "json_c_demo.h"
#endif

#ifdef LOSCFG_DEMOS_LIBPNG
#include "libpng_demo.h"
#endif

#ifdef LOSCFG_DEMOS_BIDIREFERENCE
#include "bidireference_demo.h"
#endif

#ifdef LOSCFG_DEMOS_FREETYPE
#include "freetype_demo.h"
#endif

#ifdef LOSCFG_DEMOS_LUA
#include "lua_demo.h"
#endif

#ifdef LOSCFG_DEMOS_OPUS
#include "opus_demo.h"
#endif

#ifdef LOSCFG_DEMOS_LIBXML2
#include "libxml2_demo.h"
#endif

#ifdef LOSCFG_COMPONENTS_NETWORK
#define USER_TASK_PRIORITY          2
#define AGENT_DEMO_TASK_SIZE        0x1000
#define AGENT_DEMO_TASK_MQTT_SIZE   0x2000
STATIC UINT32 g_atinyTaskId;
#endif

#ifdef LOSCFG_COMPONENTS_NETWORK
STATIC VOID AtinyDemoTaskEntry(VOID)
{
#ifdef LOSCFG_COMPONENTS_NET_LWIP
    hieth_hw_init();
    net_init();
#endif

#ifdef LOSCFG_COMPONNETS_NET_AT
#ifdef LOSCFG_COMPONENTS_NET_AT_ESP8266
    Esp8266Register();
#elif defined LOSCFG_COMPONENTS_NET_AT_BG36
    Bg36Register();
#elif defined LOSCFG_COMPONENTS_NET_AT_SIM900A
    Sim900aRegister();
#elif defined LOSCFG_COMPONENTS_NET_AT_BC95
    Bc95Register();
#endif

#ifdef LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY
    NBIoT_DemoEntry();
#endif
#endif

#ifdef LOSCFG_DEMOS_AGENT_TINY_MQTT
    flash_adaptor_init();
    demo_param_s demoParam = {
        .init = NULL,
        .write_flash_info = flash_adaptor_write_mqtt_info,
        .read_flash_info = flash_adaptor_read_mqtt_info
    };
    AgentTinyDemoInit(&demoParam);
#endif

#ifndef LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY
#ifdef CONFIG_FEATURE_FOTA
    hal_init_ota();
#endif
#ifdef LOSCFG_DEMOS_AGENT_TINY_MQTT
    AgentTinyMqttDemoEntry();
#endif

#ifdef LOSCFG_DEMOS_AGENT_TINY_LWM2M
    AgentTinyLwm2mDemoEntry();
#endif
#endif
}
#endif

#ifdef LOSCFG_COMPONENTS_NETWORK
VOID AgenttinyDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }

    taskInitParam.usTaskPrio = USER_TASK_PRIORITY;
    taskInitParam.pcName = "AgenttinyDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)AtinyDemoTaskEntry;
#if defined(CONFIG_FEATURE_FOTA) || defined(LOSCFG_COMPONENTS_CONNECTIVITY_MQTT)
    /* fota use mbedtls bignum to verify signature consuming more stack */
    taskInitParam.uwStackSize = AGENT_DEMO_TASK_MQTT_SIZE;
#else
    taskInitParam.uwStackSize = AGENT_DEMO_TASK_SIZE;
#endif
    ret = LOS_TaskCreate(&g_atinyTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        PRINT_ERR("Create agenttiny demo task failed.\n");
    }
}
#endif

VOID DemoEntry(VOID)
{
    printf("Hello, welcome to liteos demo!\n");

#ifdef LOSCFG_DEMOS_AI
    AiDemoTask();
#endif

#ifdef LOSCFG_GUI_ENABLE
    LvglDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LMS
    LmsDemoTask();
#endif

#ifdef LOSCFG_DEMOS_TRACE
    TraceDemoTask();
#endif

#ifdef LOSCFG_COMPONENTS_NETWORK
    AgenttinyDemoTask();
#endif

#ifdef LOSCFG_DEMOS_IPV6_CLIENT
    Ipv6DemoTask();
#endif

#ifdef LOSCFG_DEMOS_KERNEL_ENTRY
    KernelDemoTask();
#elif defined(LOSCFG_DEMOS_KERNEL_INSPECT_ENTRY)
    KernelInspectDemoTask();
#endif

#ifdef LOSCFG_DEMOS_FS
    FsDemoTask();
#endif

#ifdef LOSCFG_DEMOS_DTLS_SERVER
    DtlsServerDemoTask();
#endif

#ifdef LOSCFG_DEMOS_SENSORHUB
    SensorHubDemoTask();
#endif

#ifdef LOSCFG_DEMOS_ICONV
    IconvDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LIBPNG
    LibpngDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LUA
    LuaDemoTask();
#endif

#ifdef LOSCFG_DEMOS_BIDIREFERENCE
    BidiRefDemoTask();
#endif

#ifdef LOSCFG_DEMOS_OPUS
    OpusDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LIBXML2
    Libxml2DemoTask();
#endif

#ifdef LOSCFG_DEMOS_INIPARSER
    IniparserDemoTask();
#endif

#ifdef LOSCFG_DEMOS_JSON_C
    JsonDemoTask();
#endif

#ifdef LOSCFG_DEMOS_FREETYPE
    FreeTypeDemoTask();
#endif

#ifdef LOSCFG_SHELL
    (VOID)ShellQueueCreat();
    if (OsShellInit(0) != LOS_OK) {
        PRINT_ERR("Shell init failed.\n");
    }
#endif
}
