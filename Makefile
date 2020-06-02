#*********************************************************
#  @file    Makefile
#  @brief   ML302 OpenCPU Makefile
#  Copyright (c) 2019 China Mobile IOT.
#  All rights reserved.
#  created by XieGangLiang 2019/09/29
#*******************************************************

TOP_PATH = $(shell pwd)
include tools/config.mk
empty:=
space:=$(empty) 
CC := tools/private/gcc-arm-none-eabi/bin/arm-none-eabi-gcc
CXX := tools/private/gcc-arm-none-eabi/bin/arm-none-eabi-g++
BINMAKER := python tools/private/elf2flash.py
CFLAGS :=  -std=gnu11   -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mno-unaligned-access -g -Os  -ffunction-sections -fdata-sections -w
INC :=  -I. 
include lib/module.mk

LDFLAGS := -w -std=c++11 -fno-exceptions -fno-rtti -fno-threadsafe-statics 
LDFLAGS +=   prebuilt/components/appstart/app_start.c.obj prebuilt/components/appstart/app_keypad.c.obj  
LDFLAGS += -T prebuilt/components/appstart/flashrun.ld.obj -Wl,-Map=firmware/$(strip $(HWVER))/8910DM_ML302.map 
LDFLAGS +=-nostdlib -Wl,--gc-sections -Wl,--start-group prebuilt/libatr.a prebuilt/libats.a prebuilt/libdiag.a prebuilt/libnvm.a prebuilt/libkernel.a prebuilt/libosi_lib.a prebuilt/libdriver.a prebuilt/libml.a prebuilt/libbdev.a prebuilt/libfs.a prebuilt/libfsmount.a prebuilt/libnet.a prebuilt/libhal.a prebuilt/libmal.a prebuilt/libcfw.a prebuilt/libfupdate.a prebuilt/libservice.a prebuilt/libapploader.a prebuilt/libaudio.a prebuilt/libtts.a prebuilt/components/newlib/armca5/libc.a tools/private/gcc-arm-none-eabi/lib/gcc/arm-none-eabi/7.2.1/thumb/v7-ar/fpv3/hard/libgcc.a -Wl,--end-group prebuilt/libnanopb.a prebuilt/libmbedtls.a prebuilt/libaworker.a prebuilt/liblwip.a prebuilt/libota.a prebuilt/libgps.a prebuilt/libfatfs.a prebuilt/libjson.a prebuilt/libfatfs_core.a prebuilt/libatr.a prebuilt/libats.a prebuilt/libdiag.a prebuilt/libkernel.a prebuilt/libdriver.a prebuilt/libml.a prebuilt/libfs.a prebuilt/libnet.a prebuilt/libcfw.a prebuilt/libnanopb.a prebuilt/libmbedtls.a prebuilt/libaworker.a prebuilt/liblwip.a prebuilt/libota.a prebuilt/libgps.a prebuilt/libfatfs.a prebuilt/libaisound.a prebuilt/libjson.a prebuilt/libfatfs_core.a prebuilt/libfupdate.a prebuilt/libtts.a prebuilt/libnvm.a prebuilt/libaudio.a prebuilt/libosi_lib.a prebuilt/libservice.a 
LDFLAGS +=prebuilt/components/newlib/armca5/libc.a 
LDFLAGS +=tools/private/gcc-arm-none-eabi/lib/gcc/arm-none-eabi/7.2.1/thumb/v7-ar/fpv3/hard/libgcc.a 
LDFLAGS +=prebuilt/libfsmount.a prebuilt/libsffs.a prebuilt/libbdev.a prebuilt/librpc.a prebuilt/libhal.a prebuilt/libcalclib.a prebuilt/Aisound.a
LDFLAGS +=prebuilt/components/cmd_engine.o


SRC_DIRS := applications

include oc/module.mk
include applications/module.mk

ifeq ($(onemo_fota_on),y)
CFLAGS += -DONEMO_FOTA_SUPPORT
endif

ifeq ($(onemo_gnss_on),y)
CFLAGS += -DONEMO_GNSS_SUPPORT
endif


INC +=  $(foreach dir,$(SRC_DIRS),-I$(dir)$(space) )
OBJS = $(foreach dir,$(SRC_DIRS),$(patsubst %.c,out/%.o,$(wildcard $(dir)/*.c)))
OBJS += $(foreach dir,$(SRC_DIRS),$(patsubst %.cpp,out/%.o,$(wildcard $(dir)/*.cpp)))
SRC-C-DEPS = $(foreach dir,$(SRC_DIRS),$(patsubst %.c,out/%.o.d,$(wildcard $(dir)/*.c)))
SRC-C-DEPS +=  $(foreach dir,$(SRC_DIRS),$(patsubst %.cpp,out/%.o.d,$(wildcard $(dir)/*.cpp)))
all: check $(OBJS) 
	@echo "linking $(HWVER) libs"
	@$(CXX) -Wl,--no-whole-archive $(OBJS) $(LDFLAGS)   -o firmware/$(strip $(HWVER))/8910DM_ML302.elf
	@tools/private/dtools mkuimage --name DEVEL-gfed18f29-dirty firmware/$(strip $(HWVER))/8910DM_ML302.elf firmware/$(strip $(HWVER))/8910DM_ML302.img
	@tools/private/vlrsign/vlrsign --pw 12345678 --pn test --ha Blake2 --img firmware/$(strip $(HWVER))/8910DM_ML302.img --out prebuilt/pacfile/8910DM_ML302.sign.img
	@tools/private/python3/python3 tools/private/pacgen.py pac-gen  tools/private/cat1_UIS8910DM_L04_SingleSim.json  firmware/$(strip $(HWVER))/ML302_OpenCPU.pac

check:
	@do_check.bat $(subst /,\,$(SRC_DIRS)) 
out/%.o:%.c 
	@echo 'compile $<...'''
	@$(CC) $(CFLAGS) $(INC)  -c $< -o $@ -MD -MF $(dir $@)$(notdir $@).d -MT $@
out/%.o:%.cpp 
	@echo 'compile $<...'''
	@$(CXX) $(CFLAGS) $(INC) -c $< -o $@ -MD -MF $(dir $@)$(notdir $@).d -MT $@	
clean:
	@clean.bat
	@echo clean ends
    
-include $(SRC-C-DEPS)