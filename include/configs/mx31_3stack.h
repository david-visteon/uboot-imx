/*
 * Copyright 2008-2009 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * Configuration settings for the MX31 3Stack Freescale board.
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

#include <asm/arch/mx31-regs.h>

 /* High Level Configuration Options */
#define CONFIG_ARM1136		1	/* This is an arm1136 CPU core */
#define CONFIG_MX31		1	/* in a mx31 */
#define CONFIG_MX31_HCLK_FREQ	26000000
#define CONFIG_MX31_CLK32	32768

#define CONFIG_MX31_NAND

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_SKIP_RELOCATE_UBOOT

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1

/*
 * Size of malloc() pool
 */
#define CFG_MALLOC_LEN		(CFG_ENV_SIZE + 128 * 1024)
#define CFG_GBL_DATA_SIZE	128

/*
 * Hardware drivers
 */

#define CONFIG_MX31_UART	1
#define CFG_MX31_UART1		1

#define CONFIG_HARD_SPI		1
#define CONFIG_MXC_SPI		1
#define CONFIG_MXC_SPI_IFACE	1

#define CONFIG_RTC_MC13783	1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX	1
#define CONFIG_BAUDRATE		115200
#define CFG_BAUDRATE_TABLE	{9600, 19200, 38400, 57600, 115200}

/***********************************************************
 * Command definition
 ***********************************************************/

#include <config_cmd_default.h>

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_SPI
#define CONFIG_CMD_DATE
#define CONFIG_CMD_NAND
#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY	3

#define CONFIG_LOADADDR		0x80800000	/* loadaddr env var */

#define	CONFIG_EXTRA_ENV_SETTINGS				\
	"netdev=eth0\0"						\
	"uboot=u-boot.bin\0"					\
	"kernel=uImage\0"					\
	"loadaddr=0x80010000\0"					\
	"tftp_server=10.192.225.58\0"		\
	"serverip=10.192.225.211\0"			\
	"nfsroot=/tools/rootfs/rootfs-2.6.24\0"			\
	"bootargs_base=setenv bootargs console=ttymxc0,115200\0"	\
	"bootargs_nfs=setenv bootargs ${bootargs} root=/dev/nfs "	\
		"ip=dhcp nfsroot=${serverip}:${nfsroot} rw\0"	\
	"bootcmd=run bootcmd_net\0"					\
	"bootcmd_net=run bootargs_base bootargs_nfs; "			\
		"tftpboot ${loadaddr} ${tftp_server}:${kernel}; bootm\0"

/* configure for smc91xx debug board ethernet */
#define CONFIG_NET_MULTI 1
#define CONFIG_DRIVER_SMC911X          1
#define CONFIG_DRIVER_SMC911X_BASE     CS5_BASE
#define CONFIG_DRIVER_SMC911X_32_BIT   1

#define CONFIG_ARP_TIMEOUT	200UL

/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP		/* undef to save memory */
#define CFG_PROMPT		"=> "
#define CFG_CBSIZE		256		/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CFG_PBSIZE		(CFG_CBSIZE + sizeof(CFG_PROMPT) + 16)
#define CFG_MAXARGS		16		/* max number of command args */
#define CFG_BARGSIZE		CFG_CBSIZE	/* Boot Argument Buffer Size */

#define CFG_MEMTEST_START	0		/* memtest works on */
#define CFG_MEMTEST_END		0x10000

#undef	CFG_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define CFG_LOAD_ADDR		CONFIG_LOADADDR

#define CFG_HZ			CONFIG_MX31_CLK32

#define CONFIG_CMDLINE_EDITING	1

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
#define PHYS_SDRAM_1		CSD0_BASE
#define PHYS_SDRAM_1_SIZE	(128 * 1024 * 1024)

/*
 * TODO: NAND Flash configure
 */

#define CFG_NO_FLASH
#define NAND_MAX_CHIPS         1
#define CFG_MAX_NAND_DEVICE    1
#define CFG_NAND_BASE          0x40000000

#define	CFG_ENV_IS_IN_NAND	1
#define CFG_ENV_OFFSET		0x40000	/* 2nd block */
#define CFG_ENV_SIZE		(128*1024)

/*
 * JFFS2 partitions TODO:
 */
#undef CONFIG_JFFS2_CMDLINE
#define CONFIG_JFFS2_DEV	"nand0"

#endif /* __CONFIG_H */
