# Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.

## platform relative ##
LITEOS_ARM_ARCH :=
LITEOS_CPU_TYPE :=
## c as cxx ld options ##
LITEOS_ASOPTS :=
LITEOS_COPTS_BASE :=
LITEOS_COPTS_EXTRA :=
LITEOS_COPTS_EXTRA_INTERWORK :=
LITEOS_COPTS_DEBUG :=
LITEOS_COPTS_NODEBUG :=
LITEOS_INTERWORK :=
LITEOS_CXXOPTS :=
LITEOS_CXXOPTS_BASE :=
LITEOS_LD_OPTS :=
LITEOS_GCOV_OPTS :=
## dynload ld options ##
LITEOS_DYNLOADOPTS :=
## macro define ##
LITEOS_CMACRO :=
LITEOS_CMACRO_TEST :=
LITEOS_CXXMACRO :=
## head file path and ld path ##
LITEOS_PLATFORM_INCLUDE :=
LITEOS_CXXINCLUDE :=
LITEOS_LD_PATH :=
LITEOS_LD_SCRIPT :=
LITEOS_MK_PATH :=
## c as cxx ld flags ##
LITEOS_ASFLAGS :=
LITEOS_CFLAGS :=
LITEOS_CFLAGS_INTERWORK :=
LITEOS_LDFLAGS :=
LITEOS_CXXFLAGS :=
## depended lib ##
LITEOS_BASELIB :=
LITEOS_LIBDEP :=
## directory ##
LIB_BIGODIR :=
LIB_SUBDIRS :=
##compiler##
LITEOS_COMPILER_CXXLIB_PATH :=
LITEOS_COMPILER_GCCLIB_PATH :=
LITEOS_COMPILER_GCC_INCLUDE :=

-include $(LITEOSTOPDIR)/.config

OS ?= $(shell uname -s)
# Introduced compiler binaries which might different from gcc compilers.
# Those compilers need to implement like $(CC) $(GPP) like gcc does.
# Meanwhile Introduced compiler includes and lib paths
# Export : $(LITEOS_COMPILER_GCC_INCLUDE)
#          $(LITEOS_CXXINCLUDE)
#          $(LITEOS_COMPILER_GCCLIB_PATH)
#          $(LITEOS_COMPILER_CXXLIB_PATH)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
include $(LITEOSTOPDIR)/build/mk/compiler_xtensa.mk
else
# Supported GCC Compilers
# Also support "make CROSS_COMPILE=" to use third party compilers.
include $(LITEOSTOPDIR)/build/mk/compiler_gcc.mk
endif

HIDE    := @
MKDIR    = mkdir
RM       = -rm -rf
ECHO     = echo
ARFLAGS := cr
PYTHON  := python

OS       ?= $(shell uname -s)
OBJ_MKDIR = if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi

# Generate file that contains link targets.
# Also support old style sub module make directly using "LITEOS_BASELIB +="
# which is not recommented.
include $(LITEOSTOPDIR)/build/mk/gen_link_targets.mk

## variable define ##
OUT  = $(LITEOSTOPDIR)/out/$(LITEOS_PLATFORM)
BUILD  = $(OUT)/obj
MK_PATH  = $(LITEOSTOPDIR)/build/mk
LITEOS_SCRIPTPATH  ?= $(LITEOSTOPDIR)/tools/scripts
LITEOS_LIB_BIGODIR  = $(OUT)/lib/obj
LOSCFG_ENTRY_SRC    = $(LITEOSTOPDIR)/targets/bsp/common/los_config.c

LITEOS_MENUCONFIG_H = $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/menuconfig.h
LITEOS_PLATFORM_MENUCONFIG_H = $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/menuconfig.h

### include variable
MODULE = $(MK_PATH)/module.mk
MODULE_LIB = $(MK_PATH)/module_lib.mk

ifeq ($(LOSCFG_COMPILER_HIMIX_32), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
else ifeq ($(LOSCFG_COMPILER_HIMIX100_64), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
else ifeq ($(LOSCFG_COMPILER_HCC_64), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
else ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
LITEOS_CMACRO      += -D__COMPILER_XTENSA__
endif

LITEOS_CMACRO      += -D__LITEOS__ -DSECUREC_IN_KERNEL=0 -D_ALL_SOURCE
LITEOS_BASELIB     += -lgcc

ifneq ($(findstring $(LOSCFG_COMPILER_ARM_NONE_EABI)$(LOSCFG_COMPILER_RISCV_UNKNOWN)$(LOSCFG_COMPILER_XTENSA_32), y), )
else
LITEOS_BASELIB     += -lgcc_eh
endif
AS_OBJS_LIBC_FLAGS  = -D__ASSEMBLY__
WARNING_AS_ERROR   := -Wall -Werror

ifeq ($(LOSCFG_KERNEL_DYNLOAD), y)
-include $(LITEOSTOPDIR)/build/mk/dynload_ld.mk
endif

include $(LITEOSTOPDIR)/arch/cpu.mk
include $(LITEOSTOPDIR)/targets/targets.mk

# Export APIs from these module.
include $(LITEOSTOPDIR)/lib/api.mk
include $(LITEOSTOPDIR)/compat/api.mk


LIB_SUBDIRS += kernel compat lib

LITEOS_KERNEL_INCLUDE := -I $(LITEOSTOPDIR)/kernel/include

############################# Tools && Debug Option Begin ##############################

ifeq ($(LOSCFG_3RDPARTY), y)
    -include $(LITEOSTOPDIR)/3rdParty/3rdParty.mk
endif

ifeq ($(LOSCFG_PLATFORM_OSAPPINIT), y)

else
    -include $(LITEOSTOPDIR)/test/test.mk
endif

ifeq ($(LOSCFG_NULL_ADDRESS_PROTECT), y)
    LITEOS_CMACRO += -DLOSCFG_NULL_ADDRESS_PROTECT
endif

############################# Tools && Debug Option End #################################

## Optimize Options
ifeq ($(LOSCFG_COMPILER_OPTIMIZE_NONE), y)
    LITEOS_COPTS_OPTIMIZE = -O0
else ifeq ($(LOSCFG_COMPILER_OPTIMIZE_SPEED), y)
    LITEOS_COPTS_OPTIMIZE = -O2
else ifeq ($(LOSCFG_COMPILER_OPTIMIZE_SIZE), y)
    LITEOS_COPTS_OPTIMIZE = -Os
endif

ifeq ($(LOSCFG_COMPILE_DEBUG), y)
    LITEOS_COPTS_OPTION  = -g -gdwarf-2
endif

LITEOS_COPTS_DEBUG  += $(LITEOS_COPTS_OPTION) $(LITEOS_COPTS_OPTIMIZE)
LITEOS_INTERWORK    += $(LITEOS_COPTS_OPTION) $(LITEOS_COPTS_OPTIMIZE)
LITEOS_CXXOPTS_BASE += $(LITEOS_COPTS_OPTION) $(LITEOS_COPTS_OPTIMIZE)
LITEOS_ASOPTS       += $(LITEOS_COPTS_OPTION)

## Security Options
ifeq ($(LOSCFG_CC_STACKPROTECTOR), y)
    LITEOS_SSP = -fstack-protector --param ssp-buffer-size=4
else ifeq ($(LOSCFG_CC_STACKPROTECTOR_STRONG), y)
    LITEOS_SSP = -fstack-protector-strong
else ifeq ($(LOSCFG_CC_STACKPROTECTOR_ALL), y)
    LITEOS_SSP = -fstack-protector-all
endif

# TODO : THIS feature whether support or not depands on if its C++ libs
#        are compiled with liteos which should be controlled with KCONFIGS.
LITEOS_CMACRO     += -DLOSCFG_KERNEL_CPP_EXCEPTIONS_SUPPORT
LITEOS_CXXMACRO   += -DLOSCFG_KERNEL_CPP_EXCEPTIONS_SUPPORT

LITEOS_COMMON_OPTS = -fno-pic -fno-builtin \
                     -ffunction-sections -fdata-sections $(WARNING_AS_ERROR) $(LITEOS_SSP)

ifeq ($(LOSCFG_LIB_LIBC), y)
    LITEOS_COMMON_OPTS += -nostdinc -nostdlib
endif

LITEOS_COPTS_BASE += $(LITEOS_COMMON_OPTS)

# Anonymous structs and unions are supported on c11; while gcc supports those features as extension
# which is turn on for default. As for Clang, if choose c99, -std=gnu99 should be used.
ifeq ($(LOSCFG_COMPILER_CLANG), y)
LITEOS_COPTS_BASE += -std=gnu99
else
LITEOS_COPTS_BASE += -std=c99
endif

# -Wpointer-arith will treat the size of a void or of a function as 1.
# -Wstrict-prototypes will warn if a function is defined without specifying the argument types.
LITEOS_COPTS_BASE += -Wpointer-arith -Wstrict-prototypes -fno-exceptions

ifeq ($(LOSCFG_COMPILER_GCC), y)
LITEOS_COPTS_BASE += -fno-aggressive-loop-optimizations
endif

# clang support -fno-omit-frame-pointer
ifneq ($(LOSCFG_COMPILER_XTENSA_32), y)
LITEOS_COPTS_BASE += -fno-omit-frame-pointer -Winvalid-pch
endif

ifeq ($(LOSCFG_COMPILER_ARM_NONE_EABI), y)
LITEOS_COPTS_EXTRA += -Wno-unused-value -Wno-unused-function
endif

ifneq ($(LOSCFG_ARCH_RISCV), y)
LITEOS_COPTS_BASE += -fno-short-enums
endif

LITEOS_CXXOPTS_BASE += -std=c++11 -nostdinc++ -fexceptions -fpermissive -fno-use-cxa-atexit -frtti \
                       $(LITEOS_COMMON_OPTS)

ifneq ($(LOSCFG_COMPILER_XTENSA_32), y)
LITEOS_CXXOPTS_BASE += -fno-omit-frame-pointer -Winvalid-pch
endif

ifeq ($(LOSCFG_COMPILER_RISCV_UNKNOWN), y)
LITEOS_COPTS_BASE += -msave-restore -fno-schedule-insns -fno-inline-small-functions \
                         -fno-inline-functions-called-once  -fldm-stm-optimize \
                         -mabi=ilp32 -falign-functions=2 -fno-optimize-strlen -freorder-blocks-algorithm=simple \
                         -Wa,-enable-c-lbu-sb -mtune=size -fno-strict-aliasing -msmall-data-limit=0 -fno-short-enums \
                         -fno-common -freg-struct-return -freg-struct-return
LITEOS_CMACRO += -DLOS_COMPILE_LDM
LITEOS_ASOPTS += -DLOS_COMPILE_LDM
endif

ifeq ($(LOSCFG_LLTREPORT), y)
# -fprofile-arcs may introduce false alarm on 'maybe-uninitialized'
LITEOS_GCOV_OPTS := -fprofile-arcs -ftest-coverage -Wno-maybe-uninitialized
LITEOS_BASELIB += -lgcov
endif

LITEOS_LD_OPTS += -nostartfiles -static --gc-sections

# determine libgcc.a for fpu, use thumb as default
ifeq ($(LOSCFG_ARCH_ARM_CORTEX_M), y)
ifeq ($(LOSCFG_ARCH_FPU_DISABLE), y)
GCCLIBA := $(LITEOS_COMPILER_GCCLIB_PATH)/thumb/v7e-m+fp/softfp
else
GCCLIBA := $(LITEOS_COMPILER_GCCLIB_PATH)/thumb/v7e-m+fp/hard
endif

LITEOS_LD_PATH += -L$(GCCLIBA)
endif

LITEOS_LD_OPTS += $(LITEOS_DYNLOADOPTS)

LITEOS_LD_PATH += -L$(LITEOS_SCRIPTPATH)/ld -L$(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM) -L$(OUT)/lib -L$(LITEOS_LIB_BIGODIR) -L$(LITEOSTOPDIR)/tools/build
LITEOS_LD_PATH += -L$(LITEOS_COMPILER_GCCLIB_PATH) -L$(LITEOS_COMPILER_CXXLIB_PATH)

ifeq ($(LOSCFG_VENDOR), y)
LITEOS_LD_PATH +=  -L$(OUT)/lib/rdk \
                   -L$(OUT)/lib/sdk \
                   -L$(OUT)/lib/main_server
endif

ifeq ($(LOSCFG_USING_BOARD_LD), y)
LITEOS_LD_SCRIPT := -T$(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/liteos.ld
else
LITEOS_LD_SCRIPT := -T$(LITEOSTOPDIR)/liteos.ld
endif

# temporary
LITEOS_PLATFORM_INCLUDE += \
        -I $(LITEOSTOPDIR)/kernel/base/include
LITEOS_CXXINCLUDE += \
        $(LITEOS_COMPILER_GCC_INCLUDE) \
        $(LITEOS_NET_INCLUDE) \
        -I $(LITEOSTOPDIR)/kernel/base/include \
        -I $(LITEOSTOPDIR)/compat/posix/include \
        -I $(LITEOSTOPDIR)/lib/libc/include \
        -I $(LITEOSTOPDIR)/fs/include \
        -I $(LITEOSTOPDIR)/kernel/include \
        $(LITEOS_LIBC_INCLUDE)

LOSCFG_TOOLS_DEBUG_INCLUDE := $(LITEOS_SHELL_INCLUDE) $(LITEOS_UART_INCLUDE)

LITEOS_COPTS_NODEBUG    := $(LITEOS_NODEBUG) $(LITEOS_COPTS_BASE) $(LITEOS_COPTS_EXTRA)
LITEOS_COPTS_INTERWORK  := $(LITEOS_INTERWORK) $(LITEOS_COPTS_BASE) $(LITEOS_COPTS_EXTRA_INTERWORK)
LITEOS_BASE_INCLUDE     := $(LITEOS_KERNEL_INCLUDE) $(LITEOS_PLATFORM_INCLUDE) \
                           $(LITEOS_LIB_INCLUDE) $(LITEOS_FS_INCLUDE) \
                           $(LITEOS_EXTKERNEL_INCLUDE) \
                           $(LITEOS_COMPAT_INCLUDE) $(LITEOS_DRIVERS_INCLUDE) \
                           $(LOSCFG_TOOLS_DEBUG_INCLUDE) $(LITEOS_NET_INCLUDE)
LITEOS_CFLAGS_INTERWORK := $(LITEOS_COPTS_INTERWORK) $(LITEOS_CMACRO) \
                           $(LITEOS_CMACRO_TEST)     $(LITEOS_IMAGE_MACRO) \
                           $(LITEOS_BASE_INCLUDE)
LITEOS_CFLAGS_NODEBUG   := $(LITEOS_COPTS_NODEBUG) $(LITEOS_CMACRO) \
                           $(LITEOS_CMACRO_TEST)   $(LITEOS_IMAGE_MACRO) \
                           $(LITEOS_BASE_INCLUDE)
