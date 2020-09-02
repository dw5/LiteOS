/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Aarch64 Hw Task Implementation
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

#include "los_hwi.h"
#include "los_hw_pri.h"
#include "los_task_pri.h"
#include "los_memory.h"
#include "los_spinlock.h"
#include "los_hw_cache_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* support cpu vendors */
CpuVendor g_cpuTable[] = {
    /* armv8-a */
    { 0xd03, "Cortex-A53" },
    /* armv8.2-a */
    { 0xd05, "Cortex-A55" },
    { 0, NULL }
};

/* logical cpu mapping */
UINT64 g_cpuMap[LOSCFG_KERNEL_CORE_NUM] = {
    [0 ... LOSCFG_KERNEL_CORE_NUM - 1] = (UINT64)(-1)
};

VOID OsTaskExit(VOID)
{
    __asm__ __volatile__("svc  #0");
}

VOID *OsTaskStackInit(UINT32 taskID, UINT32 stackSize, VOID *topStack)
{
    UINT32 index;
    TaskContext *taskContext = NULL;

    OsStackInit(topStack, stackSize);
    taskContext = (TaskContext *)(((UINTPTR)topStack + stackSize) - sizeof(TaskContext));

    /* initialize the task context */
    taskContext->NZCV = 0;
    taskContext->PC = (UINTPTR)OsTaskEntry;
    taskContext->LR = (UINTPTR)OsTaskExit;  /* LR should be kept, to distinguish it's THUMB or ARM instruction */
    taskContext->X[0] = taskID;             /* R0 */

    /* 0xAAA0000000000001LL : reg initialed magic word  */
    for (index = 1; index < GEN_REGS_NUM; index++) {
        taskContext->X[index] = 0xAAA0000000000001LL + index;  /* R1 -R29 */
    }

#ifdef LOSCFG_INTERWORK_THUMB
    taskContext->regSPSR = PSR_MODE_SVC_THUMB; /* CPSR (Enable IRQ and FIQ interrupts, THUMNB-mode) */
#else
    taskContext->regSPSR = 0x5;                /* CPSR (Enable IRQ and FIQ interrupts, EL1h) */
#endif

#ifdef LOSCFG_AARCH64_FPU
    /* 0xAAA0000000000001LL : float reg initialed magic word */
    for (index = 0; index < FP_REGS_NUM; index++) {
        taskContext->Q[index] = 0xAAA0000000000001LL + index;  /* Q1 -Q31 */
    }

    taskContext->regFPCR = 0;
    taskContext->regFPSR = 0;
#endif

    return (VOID *)taskContext;
}

VOID sev(VOID)
{
    __asm__ __volatile__ ("sev" : : : "memory");
}

VOID wfe(VOID)
{
    __asm__ __volatile__ ("wfe" : : : "memory");
}

VOID wfi(VOID)
{
    __asm__ __volatile__ ("wfi" : : : "memory");
}

VOID dmb(VOID)
{
    __asm__ __volatile__("dmb ishst" : : : "memory");
}

VOID dsb(VOID)
{
    __asm__ __volatile__("dsb sy" : : : "memory");
}

VOID isb(VOID)
{
    __asm__ __volatile__("isb" : : : "memory");
}

VOID flush_icache(VOID)
{
    __asm__ __volatile__("ic ialluis");
    __asm__ __volatile__("dsb ish" : : : "memory");
}

VOID flush_dcache(UINTPTR start, UINTPTR end)
{
    arm64_clean_cache_range(start, end);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */