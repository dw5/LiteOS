/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Cpup HeadFile
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#ifndef _LOS_CPUP_PRI_H
#define _LOS_CPUP_PRI_H

#include "los_cpup.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
* @ingroup los_cpup
* Number of historical running time records
*/
#define OS_CPUP_HISTORY_RECORD_NUM   11

/**
 * @ingroup los_cpup
 * Count the CPU usage structures of a task.
 */
typedef struct {
    UINT32 id;                                            /**< Task ID */
    UINT16 status;                                        /**< Task status */
    UINT64 allTime;                                       /**< Total running time */
    UINT64 startTime;                                     /**< Time before a task is invoked */
    UINT64 historyTime[OS_CPUP_HISTORY_RECORD_NUM + 1];   /**< Historical running time, the last one saves zero */
} OsCpupCB;

extern OsCpupCB *g_cpup;

extern UINT32 OsCpupInit(VOID);
extern VOID OsSetCpuCycle(UINT64 startCycles);
extern UINT64 OsGetCpuCycle(VOID);
extern VOID OsTaskCycleStart(VOID);
extern VOID OsTaskCycleEnd(VOID);
extern VOID OsTaskCycleEndStart(const LosTaskCB *newTask);
#ifdef LOSCFG_CPUP_INCLUDE_IRQ
VOID OsCpupIrqStart(VOID);
VOID OsCpupIrqEnd(UINT32);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_CPUP_PRI_H */
