/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _HAL_CONFIG_H_
#define _HAL_CONFIG_H_

// Auto generated. Don't edit it manually!

#define CONFIG_SOC_8910
/* #undef CONFIG_SOC_6760 */
/* #undef CONFIG_SOC_8955 */
/* #undef CONFIG_SOC_8909 */

#define CONFIG_CPU_ARM
#define CONFIG_CPU_ARMV7A
#define CONFIG_CPU_ARM_CA5
/* #undef CONFIG_CPU_ARMV7M */
/* #undef CONFIG_CPU_ARM_CM4F */

/* #undef CONFIG_CPU_MIPS */
/* #undef CONFIG_CPU_MIPS_XCPU */
/* #undef CONFIG_CPU_MIPS_XCPU2 */

#define CONFIG_NOR_FLASH
#define CONFIG_USE_PSRAM
/* #undef CONFIG_USE_DDR */
/* #undef CONFIG_RUN_ON_FPGA */

/* #undef CONFIG_RAM_4M */
#define CONFIG_RAM_16M
/* #undef CONFIG_RAM_32M */
/* #undef CONFIG_RAM_64M */

/* #undef CONFIG_FLASH_4M */
#define CONFIG_FLASH_8M
/* #undef CONFIG_FLASH_16M */

/* #undef CONFIG_SUPPORT_EXT_FLASH */

#define CONFIG_CACHE_LINE_SIZE 32

#define CONFIG_FLASH_SIZE 0x800000
#define CONFIG_RAM_SIZE 0x1000000

#define CONFIG_UIMAGE_HEADER_SIZE 0x40

// Flash layout
#define CONFIG_NOR_PHY_ADDRESS 0x60000000
#define CONFIG_NOR_EXT_PHY_ADDRESS 0x70000000
#define CONFIG_BOOT_FLASH_OFFSET 0x0
#define CONFIG_BOOT_FLASH_SIZE 0x10000
#define CONFIG_APP_FLASH_OFFSET 0x10000
#define CONFIG_APP_FLASH_SIZE 0x330000
#define CONFIG_FS_MODEM_FLASH_OFFSET 0x4a0000
#define CONFIG_FS_MODEM_FLASH_SIZE 0x340000
#define CONFIG_FS_SYS_FLASH_OFFSET 0x340000
#define CONFIG_FS_SYS_FLASH_SIZE 0x160000
#define CONFIG_FS_FACTORY_FLASH_OFFSET 0x7e0000
#define CONFIG_FS_FACTORY_FLASH_SIZE 0x20000

/* #undef CONFIG_CALIB_FLASH_OFFSET */
/* #undef CONFIG_CALIB_GSM_FLASH_OFFSET */
/* #undef CONFIG_CALIB_NB_FLASH_OFFSET */
/* #undef CONFIG_FACTORY_FLASH_OFFSET */

#define CONFIG_BOOT_FLASH_ADDRESS 0x60000000
#define CONFIG_APP_FLASH_ADDRESS 0x60010000
#define CONFIG_FS_MODEM_FLASH_ADDRESS 0x604a0000

// Directory layout
#define CONFIG_FS_MODEM_MOUNT_POINT "/modem"
#define CONFIG_FS_SYS_MOUNT_POINT "/"
#define CONFIG_FS_FACTORY_MOUNT_POINT "/factory"
#define CONFIG_FS_MODEM_NVM_DIR "/modemnvm"
#define CONFIG_FS_AP_NVM_DIR "/nvm"
#define CONFIG_FS_FOTA_DATA_DIR "/fota"

#define CONFIG_ROM_LOAD_SRAM_OFFSET 0xc0
#define CONFIG_ROM_LOAD_SIZE 0xbf40

#define CONFIG_RAM_PHY_ADDRESS 0x80000000
#define CONFIG_APP_RAM_OFFSET 0xC00000
#define CONFIG_APP_RAM_SIZE 0x400000

#define CONFIG_SRAM_PHY_ADDRESS 0x800000
#define CONFIG_APP_SRAM_OFFSET 0x100
#define CONFIG_APP_SRAM_SIZE 0x2f00

// Function key
/* #undef CONFIG_PWRKEY_WAKEUP_PSM */
/* #undef CONFIG_PWRKEY_POWERUP */
#define CONFIG_WAKEUP_PIN_POWERUP on

#ifdef CONFIG_SOC_8910
    #define CONFIG_APP_SRAM_SHMEM_OFFSET 0x3000
    #define CONFIG_APP_SRAM_SHMEM_SIZE 0x1000

    #define CONFIG_FIQ_STACK_TOP 0x800
    #define CONFIG_IRQ_STACK_TOP 0x0
    #define CONFIG_SVC_STACK_TOP 0x200
    #define CONFIG_ABT_STACK_TOP 0x800
    #define CONFIG_UND_STACK_TOP 0x800
    #define CONFIG_SYS_STACK_TOP 0x800
    #define CONFIG_STACK_BOTTOM 0xc00

    #define CONFIG_DEFAULT_MEMPLL_FREQ 800000000
    #define CONFIG_DEFAULT_CPUPLL_FREQ 1000000000
    #define CONFIG_DEFAULT_CPU_FREQ 500000000
    #define CONFIG_DEFAULT_SYSAXI_FREQ 400000000
    #define CONFIG_DEFAULT_SYSAHB_FREQ 200000000
    #define CONFIG_DEFAULT_MEMBUS_FREQ 200000000
    #define CONFIG_DEFAULT_SPIFLASH_DEV_FREQ 200000000
    #define CONFIG_DEFAULT_SPIFLASH_CTRL_FREQ 200000000
    #define CONFIG_DEFAULT_SPIFLASH_INTF_FREQ 100000000
    #define CONFIG_TST_H_GROUND on
    #define CONFIG_BOARD_KPLED_USED_FOR_RF_SWITCH on
/* #undef CONFIG_GPIO4_FORCE_PULL_UP */
#elif defined(CONFIG_SOC_8955)
    #define CONFIG_SRAM_PHY_ADDRESS 0x800000
/* #undef CONFIG_SRAM_SIZE */
/* #undef CONFIG_BBSRAM_PHY_ADDRESS */
/* #undef CONFIG_BBSRAM_SIZE */
/* #undef CONFIG_DPRAM_PHY_ADDRESS */
/* #undef CONFIG_DPRAM_SIZE */

/* #undef CONFIG_BBSRAM_RESERVED_PATCH_SIZE */
/* #undef CONFIG_DPRAM_RESERVED_MBX_SIZE */

/* #undef CONFIG_BCPU_FLASH_OFFSET */
/* #undef CONFIG_BCPU_RAM_OFFSET */
/* #undef CONFIG_BCPU_SRAM_OFFSET */

/* #undef CONFIG_SYSPLL_FREQ */
/* #undef CONFIG_DEFAULT_SYS_CLK_FREQ */
    #define CONFIG_DEFAULT_SPIFLASH_INTF_FREQ 100000000
/* #undef CONFIG_DEFAULT_BB_CLK_FREQ */

/* #undef CONFIG_DEFAULT_VCORE_DCDC_MODE */
/* #undef CONFIG_VCORE_DCDC_HIGH */
/* #undef CONFIG_VCORE_DCDC_MEDIUM */
/* #undef CONFIG_VCORE_DCDC_LOW */
/* #undef CONFIG_VCORE_DCDC_LP */
/* #undef CONFIG_VCORE_LDO_HIGH */
/* #undef CONFIG_VCORE_LDO_MEDIUM */
/* #undef CONFIG_VCORE_LDO_LOW */
/* #undef CONFIG_VCORE_LDO_LP */
/* #undef CONFIG_MEM_CLK_HIGH_FREQ */
/* #undef CONFIG_MEM_CLK_LOW_FREQ */
#elif defined(CONFIG_SOC_8909)
    #define CONFIG_SRAM_PHY_ADDRESS 0x800000
/* #undef CONFIG_SRAM_SIZE */
/* #undef CONFIG_BBSRAM_PHY_ADDRESS */
/* #undef CONFIG_BBSRAM_SIZE */
/* #undef CONFIG_NBSRAM_PHY_ADDRESS */
/* #undef CONFIG_NBSRAM_SIZE */
/* #undef CONFIG_DPRAM_PHY_ADDRESS */
/* #undef CONFIG_DPRAM_SIZE */

/* #undef CONFIG_BBSRAM_RESERVED_PATCH_SIZE */
/* #undef CONFIG_NBSRAM_RESERVED_PATCH_SIZE */
/* #undef CONFIG_DPRAM_RESERVED_MBX_SIZE */
/* #undef CONFIG_NBSRAM_PATCH_SIZE */
/* #undef CONFIG_NBSRAM_STACK_SIZE */

/* #undef CONFIG_BCPU_FLASH_OFFSET */
/* #undef CONFIG_BCPU_RAM_OFFSET */
/* #undef CONFIG_BCPU_SRAM_OFFSET */

/* #undef CONFIG_8909_DUALMODE */
/* #undef CONFIG_CRYSTAL_WITHOUT_OSCILLATOR */

/* #undef CONFIG_DEFAULT_MCUPLL_FREQ */
/* #undef CONFIG_DEFAULT_SYS_CLK_FREQ */
/* #undef CONFIG_DEFAULT_BB_CLK_FREQ */
/* #undef CONFIG_DEFAULT_RFDIG_CLK_FREQ */
    #define CONFIG_DEFAULT_MEMBUS_FREQ 200000000
    #define CONFIG_DEFAULT_SPIFLASH_INTF_FREQ 100000000

/* #undef CONFIG_DEFAULT_VCORE_DCDC_MODE */
/* #undef CONFIG_VCORE_DCDC_HIGH */
/* #undef CONFIG_VCORE_DCDC_MEDIUM */
/* #undef CONFIG_VCORE_DCDC_LOW */
/* #undef CONFIG_VCORE_DCDC_LP */
/* #undef CONFIG_VCORE_LDO_HIGH */
/* #undef CONFIG_VCORE_LDO_MEDIUM */
/* #undef CONFIG_VCORE_LDO_LOW */
/* #undef CONFIG_VCORE_LDO_LP */

/* #undef CONFIG_RF_LDO_MODE */
#endif

/* #undef CONFIG_BLUE_SCREEN_STACK_SIZE */
/* #undef CONFIG_IRQ_STACK_SIZE */
/* #undef CONFIG_IRQ_STACK_LEVEL_SIZE */
/* #undef CONFIG_IRQ_NEST_MAX_COUNT */

/* #undef CONFIG_HAL_SIM_COUNT */

#define CONFIG_GSM_SUPPORT
#define CONFIG_LTE_NBIOT_SUPPORT

#define CONFIG_BOARD_ENTER_CALIB_BY_GPIO
#define CONFIG_BOARD_ENTER_CALIB_GPIO 13

#define CONFIG_BOARD_SUPPORT_SIM1_DETECT
#define CONFIG_BOARD_SIM1_DETECT_GPIO 4
/* #undef CONFIG_BOARD_SUPPORT_SIM2_DETECT */
/* #undef CONFIG_BOARD_SIM2_DETECT_GPIO */
/* #undef CONFIG_GPIO_IRQ_WAKEUP_EN */

#endif
