/*
 * Copyright (C) 2010-2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the MX6Q SABRE-AI Freescale board.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <asm/arch/mx6.h>

 /* High Level Configuration Options */
#define CONFIG_ARMV7	/* This is armv7 Cortex-A9 CPU core */
#define CONFIG_MXC
#define CONFIG_MX6Q
#define CONFIG_MX6Q_ARM2
#define CONFIG_FLASH_HEADER
#define CONFIG_FLASH_HEADER_OFFSET 0x400
#define CONFIG_MX6_CLK32	   32768

#define CONFIG_SKIP_RELOCATE_UBOOT

#define CONFIG_ARCH_CPU_INIT
#undef CONFIG_ARCH_MMU /* disable MMU first */
#define CONFIG_L2_OFF  /* disable L2 cache first*/

#define CONFIG_MX6_HCLK_FREQ	24000000

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_SYS_64BIT_VSPRINTF

#define BOARD_LATE_INIT

#define CONFIG_CMDLINE_TAG	/* enable passing of ATAGs */
//#define CONFIG_SERIAL_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
//#define CONFIG_MXC_GPIO

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(28 * 1024)
//#define CONFIG_SYS_MALLOC_LEN		(2 * 1024 * 1024)
/* size in bytes reserved for initial data */
#define CONFIG_SYS_GBL_DATA_SIZE	128

/*
 * Hardware drivers
 */
#define CONFIG_MXC_UART
#define CONFIG_UART_BASE_ADDR   UART2_BASE_ADDR


/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{9600, 19200, 38400, 57600, 115200}

#define CONFIG_CMD_MMC
/***********************************************************
 * Command definition
 ***********************************************************/
//add from old sx5 cfg
#define CONFIG_CMD_BDI		        /* bdinfo			*/
#define CONFIG_CMD_BOOTD	        /* bootd			*/
#define CONFIG_CMD_CONSOLE	  /* coninfo			*/
#define CONFIG_CMD_RUN		        /* run command in env variable	*/

/* Enable below configure when supporting nand */
#define CONFIG_CMD_ENV  

#define CONFIG_REF_CLK_FREQ CONFIG_MX6_HCLK_FREQ

#undef CONFIG_CMD_IMLS

//#define CONFIG_ENV_IS_EMBEDDED
#define CONFIG_CMD_IMI		/* iminfo	*/	
#define CONFIG_CMD_XIMG		/* Load part of Multi Image	*/

#define CONFIG_CMD_SPI
#define CONFIG_CMD_SF

#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_MTD_DEVICE		1
#define CONFIG_SYS_MAX_FLASH_SECT	128
#define CONFIG_FLASH_SHOW_PROGRESS

#ifdef  CONFIG_CMD_SF
	#define CONFIG_FSL_SF					1
	#define CONFIG_SPI_FLASH_IMX_M25PXX		1
	#define CONFIG_SPI_FLASH_MACRONIX		1
	#define CONFIG_SPI_FLASH_CS             0
	#define CONFIG_IMX_ECSPI
	#define IMX_CSPI_VER_2_3                1
	#define MAX_SPI_BYTES                   (64 * 4)
#endif

#define CONFIG_CMD_MEMORY 
#define CONFIG_CMD_IMXOTP
#define CONFIG_IMX_OTP


#define CONFIG_BOOTDELAY           2

#define CONFIG_PRIME	"FEC0"

#define CONFIG_LOADADDR		0x10800000	/* loadaddr env var */
#define CONFIG_RD_LOADADDR	0x12000000

#define	CONFIG_EXTRA_ENV_SETTINGS					\
		"uboot=u-boot.bin\0"			\
		"kernel=uImage\0"				\
		"initrd=uramdisk\0"				\
		"bootargs_base=setenv bootargs initrd=0x12000040,0x800000 console=ttymxc1,115200 \0" \
		"bootargs_mmc=setenv bootargs ${bootargs} \0"     \
		"bootcmd_mmc=run bootargs_base bootargs_mmc; "   \
		"mmc dev 0; "	\
		"mmc read ${loadaddr} 0x800 0x2000; "	\
		"mmc read ${rd_loadaddr} 0x2800 0x4000; " \
		"bootm\0"	\
		"bootcmd=run bootcmd_mmc\0"                             


#define CONFIG_ARP_TIMEOUT	200UL

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_PROMPT		      "MX6Q SX5 U-Boot > "
#define CONFIG_AUTO_COMPLETE

#define CONFIG_SYS_CBSIZE		256	/* Console I/O Buffer Size */

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE         (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
#define CONFIG_SYS_BARGSIZE      CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */

#define CONFIG_SYS_MEMTEST_START	      0x10000000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x10010000

#undef	CONFIG_SYS_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_HZ			1000

#define CONFIG_CMDLINE_EDITING

#define CONFIG_CMD_SAVEENV 


/*
 * SATA Configs
 */
#ifdef CONFIG_CMD_SATA
	#define CONFIG_DWC_AHSATA
	#define CONFIG_SYS_SATA_MAX_DEVICE	1
	#define CONFIG_DWC_AHSATA_PORT_ID	0
	#define CONFIG_DWC_AHSATA_BASE_ADDR	SATA_ARB_BASE_ADDR
	#define CONFIG_LBA48
	#define CONFIG_LIBATA

	#define CONFIG_DOS_PARTITION	1
	#define CONFIG_CMD_FAT		1
	#define CONFIG_CMD_EXT2		1
#endif


/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128 * 1024)	/* regular stack */

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1		CSD0_DDR_BASE_ADDR

//#define CONFIG_DDR_32BIT_CHECK

#if defined(CONFIG_DDR_32BIT_CHECK)
#define PHYS_SDRAM_1_SIZE	(1u * 512 * 1024 * 1024)
#else
#define PHYS_SDRAM_1_SIZE	(1u * 1024 * 1024 * 1024)
#endif


#define iomem_valid_addr(addr, size) \
	(addr >= PHYS_SDRAM_1 && addr <= (PHYS_SDRAM_1 + PHYS_SDRAM_1_SIZE))

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#ifndef CONFIG_CMD_WEIMNOR
	#define CONFIG_SYS_NO_FLASH
#endif
/* Monitor at beginning of flash */
#define CONFIG_FSL_ENV_IN_MMC
/* #define CONFIG_FSL_ENV_IN_NAND */
/* #define CONFIG_FSL_ENV_IN_SATA */

#define CONFIG_ENV_SECT_SIZE    (8 * 1024)
#define CONFIG_ENV_SIZE         CONFIG_ENV_SECT_SIZE


#if defined(CONFIG_FSL_ENV_IN_NAND)
	#define CONFIG_ENV_IS_IN_NAND 1
	#define CONFIG_ENV_OFFSET	0x100000
#elif defined(CONFIG_FSL_ENV_IN_MMC)
	#define CONFIG_ENV_IS_IN_MMC	1
	//#define CONFIG_ENV_OFFSET	(768 * 1024)
	//#define CONFIG_ENV_OFFSET	(1024 * 1024)
	#define CONFIG_ENV_OFFSET	(512 * 1024)
#elif defined(CONFIG_FSL_ENV_IN_SATA)
	#define CONFIG_ENV_IS_IN_SATA   1
	#define CONFIG_SATA_ENV_DEV     0
	#define CONFIG_ENV_OFFSET       (768 * 1024)
#elif defined(CONFIG_FSL_ENV_IN_SF)
	#define CONFIG_ENV_IS_IN_SPI_FLASH	1
	#define CONFIG_ENV_SPI_CS		1
	#define CONFIG_ENV_OFFSET       0x40000
#else

#define CONFIG_ENV_IS_NOWHERE	  1

#endif


#ifdef CONFIG_SPLASH_SCREEN	
	 // Framebuffer and LCD	 
	#define CONFIG_LCD
	#define CONFIG_IPU_V3H
	#define CONFIG_VIDEO_MX5
	#define CONFIG_IPU_CLKRATE	260000000
	#define CONFIG_SYS_CONSOLE_ENV_OVERWRITE
	#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
	#define CONFIG_SYS_CONSOLE_IS_IN_ENV
	#define LCD_BPP		LCD_COLOR16
	#define CONFIG_CMD_BMP
	#define CONFIG_BMP_8BPP
	#define CONFIG_FB_BASE	(TEXT_BASE + 0x300000)
	#define CONFIG_SPLASH_SCREEN_ALIGN
	#define CONFIG_SYS_WHITE_ON_BLACK
#endif

// MMC Configs
#define CONFIG_PARTITIONS
#ifdef CONFIG_CMD_MMC
	#define CONFIG_MMC
	#define CONFIG_GENERIC_MMC
	#define CONFIG_IMX_MMC
	#define CONFIG_SYS_FSL_USDHC_NUM        4
	#define CONFIG_SYS_FSL_ESDHC_ADDR       0
	#define CONFIG_SYS_MMC_ENV_DEV           0
	#define CONFIG_DOS_PARTITION	1
	#define CONFIG_CMD_FAT		1
	#define CONFIG_CMD_EXT2		1

	// detect whether SD1, 2, 3, or 4 is boot device 
	//#define CONFIG_DYNAMIC_MMC_DEVNO

	// SD3 and SD4 are 8 bit 
	#define CONFIG_MMC_8BIT_PORTS   0xC
	// Setup target delay in DDR mode for each SD port
	#define CONFIG_GET_DDR_TARGET_DELAY
#endif

/*
 * OCOTP Configs
 */
#ifdef CONFIG_CMD_IMXOTP
	#define CONFIG_IMX_OTP
	#define IMX_OTP_BASE			OCOTP_BASE_ADDR
	#define IMX_OTP_ADDR_MAX		0x7F
	#define IMX_OTP_DATA_ERROR_VAL	0xBADABADA
#endif

#endif				/* __CONFIG_H */
