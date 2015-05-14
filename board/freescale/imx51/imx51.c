/*
 * Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
 *
 * (C) Copyright 2009 Freescale Semiconductor, Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/mx51.h>
#include <asm/arch/mx51_pins.h>
#include <asm/arch/iomux.h>
#include <asm/errno.h>
#include <i2c.h>
#include "board-imx51.h"
#include <asm/arch/imx_spi.h>
#include <asm/arch/imx_spi_pmic.h>

DECLARE_GLOBAL_DATA_PTR;

static u32 system_rev;
u32	mx51_io_base_addr;
volatile u32 *esdhc_base_pointer;

u32 get_board_rev(void)
{
	return system_rev;
}

static inline void setup_soc_rev(void)
{
	int reg;
	reg = __REG(ROM_SI_REV);
	switch (reg) {
	case 0x02:
		system_rev = 0x51000 | CHIP_REV_1_1;
		break;
	case 0x10:
		system_rev = 0x51000 | CHIP_REV_2_0;
		break;
	default:
		system_rev = 0x51000 | CHIP_REV_1_0;
	}
}

static inline void set_board_rev(int rev)
{
	system_rev |= (rev & 0xF) << 8;
}

inline int is_soc_rev(int rev)
{
	return (system_rev & 0xFF) - rev;
}

int dram_init(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
	return 0;
}

static void setup_uart(void)
{
	unsigned int pad = PAD_CTL_HYS_ENABLE | PAD_CTL_PKE_ENABLE |
			 PAD_CTL_PUE_PULL | PAD_CTL_DRV_HIGH;
	mxc_request_iomux(MX51_PIN_UART1_RXD, IOMUX_CONFIG_ALT0);
	mxc_iomux_set_pad(MX51_PIN_UART1_RXD, pad | PAD_CTL_SRE_FAST);
	mxc_request_iomux(MX51_PIN_UART1_TXD, IOMUX_CONFIG_ALT0);
	mxc_iomux_set_pad(MX51_PIN_UART1_TXD, pad | PAD_CTL_SRE_FAST);
	mxc_request_iomux(MX51_PIN_UART1_RTS, IOMUX_CONFIG_ALT0);
	mxc_iomux_set_pad(MX51_PIN_UART1_RTS, pad);
	mxc_request_iomux(MX51_PIN_UART1_CTS, IOMUX_CONFIG_ALT0);
	mxc_iomux_set_pad(MX51_PIN_UART1_CTS, pad);
}

void setup_nfc(void)
{
	/* Enable NFC IOMUX */
	mxc_request_iomux(MX51_PIN_NANDF_CS0, IOMUX_CONFIG_ALT0);
	mxc_request_iomux(MX51_PIN_NANDF_CS1, IOMUX_CONFIG_ALT0);
	mxc_request_iomux(MX51_PIN_NANDF_CS2, IOMUX_CONFIG_ALT0);
	mxc_request_iomux(MX51_PIN_NANDF_CS3, IOMUX_CONFIG_ALT0);
	mxc_request_iomux(MX51_PIN_NANDF_CS4, IOMUX_CONFIG_ALT0);
	mxc_request_iomux(MX51_PIN_NANDF_CS5, IOMUX_CONFIG_ALT0);
	mxc_request_iomux(MX51_PIN_NANDF_CS6, IOMUX_CONFIG_ALT0);
	mxc_request_iomux(MX51_PIN_NANDF_CS7, IOMUX_CONFIG_ALT0);
}

static void setup_expio(void)
{
	u32 reg;
	/* CS5 setup */
	mxc_request_iomux(MX51_PIN_EIM_CS5, IOMUX_CONFIG_ALT0);
	writel(0x00410089, WEIM_BASE_ADDR + 0x78 + CSGCR1);
	writel(0x00000002, WEIM_BASE_ADDR + 0x78 + CSGCR2);
	/* RWSC=50, RADVA=2, RADVN=6, OEA=0, OEN=0, RCSA=0, RCSN=0 */
	writel(0x32260000, WEIM_BASE_ADDR + 0x78 + CSRCR1);
	/* APR = 0 */
	writel(0x00000000, WEIM_BASE_ADDR + 0x78 + CSRCR2);
	/* WAL=0, WBED=1, WWSC=50, WADVA=2, WADVN=6, WEA=0, WEN=0,
	 * WCSA=0, WCSN=0
	 */
	writel(0x72080F00, WEIM_BASE_ADDR + 0x78 + CSWCR1);
	if ((readw(CS5_BASE_ADDR + PBC_ID_AAAA) == 0xAAAA) &&
	    (readw(CS5_BASE_ADDR + PBC_ID_5555) == 0x5555)) {
		if (is_soc_rev(CHIP_REV_2_0) < 0) {
			reg = readl(CCM_BASE_ADDR + CLKCTL_CBCDR);
			reg = (reg & (~0x70000)) | 0x30000;
			writel(reg, CCM_BASE_ADDR + CLKCTL_CBCDR);
			/* make sure divider effective */
			while (readl(CCM_BASE_ADDR + CLKCTL_CDHIPR) != 0)
				;
			writel(0x0, CCM_BASE_ADDR + CLKCTL_CCDR);
		}
		mx51_io_base_addr = CS5_BASE_ADDR;
	} else {
		/* CS1 */
		writel(0x00410089, WEIM_BASE_ADDR + 0x18 + CSGCR1);
		writel(0x00000002, WEIM_BASE_ADDR + 0x18 + CSGCR2);
		/*  RWSC=50, RADVA=2, RADVN=6, OEA=0, OEN=0, RCSA=0, RCSN=0 */
		writel(0x32260000, WEIM_BASE_ADDR + 0x18 + CSRCR1);
		/* APR=0 */
		writel(0x00000000, WEIM_BASE_ADDR + 0x18 + CSRCR2);
		/* WAL=0, WBED=1, WWSC=50, WADVA=2, WADVN=6, WEA=0,
		 * WEN=0, WCSA=0, WCSN=0
		 */
		writel(0x72080F00, WEIM_BASE_ADDR + 0x18 + CSWCR1);
		mx51_io_base_addr = CS1_BASE_ADDR;
	}

	/* Reset interrupt status reg */
	writew(0x1F, mx51_io_base_addr + PBC_INT_REST);
	writew(0x00, mx51_io_base_addr + PBC_INT_REST);
	writew(0xFFFF, mx51_io_base_addr + PBC_INT_MASK);

	/* Reset the XUART and Ethernet controllers */
	reg = readw(mx51_io_base_addr + PBC_SW_RESET);
	reg |= 0x9;
	writew(reg, mx51_io_base_addr + PBC_SW_RESET);
	reg &= ~0x9;
	writew(reg, mx51_io_base_addr + PBC_SW_RESET);
}

void spi_io_init(struct imx_spi_dev_t *dev)
{
	switch (dev->base) {
	case CSPI1_BASE_ADDR:
		/* 000: Select mux mode: ALT0 mux port: MOSI of instance: ecspi1 */
		mxc_request_iomux(MX51_PIN_CSPI1_MOSI, IOMUX_CONFIG_ALT0);
		mxc_iomux_set_pad(MX51_PIN_CSPI1_MOSI, 0x105);

		/* 000: Select mux mode: ALT0 mux port: MISO of instance: ecspi1. */
		mxc_request_iomux(MX51_PIN_CSPI1_MISO, IOMUX_CONFIG_ALT0);
		mxc_iomux_set_pad(MX51_PIN_CSPI1_MISO, 0x105);

		if (dev->ss == 0) {
			/* de-select SS1 of instance: ecspi1. */
			mxc_request_iomux(MX51_PIN_CSPI1_SS1, IOMUX_CONFIG_ALT3);
			mxc_iomux_set_pad(MX51_PIN_CSPI1_SS1, 0x85);
			/* 000: Select mux mode: ALT0 mux port: SS0 of instance: ecspi1. */
			mxc_request_iomux(MX51_PIN_CSPI1_SS0, IOMUX_CONFIG_ALT0);
			mxc_iomux_set_pad(MX51_PIN_CSPI1_SS0, 0x185);
		} else if (dev->ss == 1) {
			/* de-select SS0 of instance: ecspi1. */
			mxc_request_iomux(MX51_PIN_CSPI1_SS0, IOMUX_CONFIG_ALT3);
			mxc_iomux_set_pad(MX51_PIN_CSPI1_SS0, 0x85);
			/* 000: Select mux mode: ALT0 mux port: SS1 of instance: ecspi1. */
			mxc_request_iomux(MX51_PIN_CSPI1_SS1, IOMUX_CONFIG_ALT0);
			mxc_iomux_set_pad(MX51_PIN_CSPI1_SS1, 0x105);
		}

		/* 000: Select mux mode: ALT0 mux port: RDY of instance: ecspi1. */
		mxc_request_iomux(MX51_PIN_CSPI1_RDY, IOMUX_CONFIG_ALT0);
		mxc_iomux_set_pad(MX51_PIN_CSPI1_RDY, 0x180);

		/* 000: Select mux mode: ALT0 mux port: SCLK of instance: ecspi1. */
		mxc_request_iomux(MX51_PIN_CSPI1_SCLK, IOMUX_CONFIG_ALT0);
		mxc_iomux_set_pad(MX51_PIN_CSPI1_SCLK, 0x105);
		break;
	case CSPI2_BASE_ADDR:
	default:
		break;
	}
}

static void setup_fec(void)
{
	/*FEC_MDIO*/
	writel(0x3, IOMUXC_BASE_ADDR + 0x0D4);
	writel(0x1FD, IOMUXC_BASE_ADDR + 0x0468);
	writel(0x0, IOMUXC_BASE_ADDR + 0x0954);

	/*FEC_MDC*/
	writel(0x2, IOMUXC_BASE_ADDR + 0x13C);
	writel(0x2004, IOMUXC_BASE_ADDR + 0x0524);

	/* FEC RDATA[3] */
	writel(0x3, IOMUXC_BASE_ADDR + 0x0EC);
	writel(0x180, IOMUXC_BASE_ADDR + 0x0480);
	writel(0x0, IOMUXC_BASE_ADDR + 0x0964);

	/* FEC RDATA[2] */
	writel(0x3, IOMUXC_BASE_ADDR + 0x0E8);
	writel(0x180, IOMUXC_BASE_ADDR + 0x047C);
	writel(0x0, IOMUXC_BASE_ADDR + 0x0960);

	/* FEC RDATA[1] */
	writel(0x3, IOMUXC_BASE_ADDR + 0x0d8);
	writel(0x180, IOMUXC_BASE_ADDR + 0x046C);
	writel(0x0, IOMUXC_BASE_ADDR + 0x095C);

	/* FEC RDATA[0] */
	writel(0x2, IOMUXC_BASE_ADDR + 0x016C);
	writel(0x2180, IOMUXC_BASE_ADDR + 0x0554);
	writel(0x0, IOMUXC_BASE_ADDR + 0x0958);

	/* FEC TDATA[3] */
	writel(0x2, IOMUXC_BASE_ADDR + 0x148);
	writel(0x2004, IOMUXC_BASE_ADDR + 0x0530);

	/* FEC TDATA[2] */
	writel(0x2, IOMUXC_BASE_ADDR + 0x144);
	writel(0x2004, IOMUXC_BASE_ADDR + 0x052C);

	/* FEC TDATA[1] */
	writel(0x2, IOMUXC_BASE_ADDR + 0x140);
	writel(0x2004, IOMUXC_BASE_ADDR + 0x0528);

	/* FEC TDATA[0] */
	writel(0x2, IOMUXC_BASE_ADDR + 0x0170);
	writel(0x2004, IOMUXC_BASE_ADDR + 0x0558);

	/* FEC TX_EN */
	writel(0x1, IOMUXC_BASE_ADDR + 0x014C);
	writel(0x2004, IOMUXC_BASE_ADDR + 0x0534);

	/* FEC TX_ER */
	writel(0x2, IOMUXC_BASE_ADDR + 0x0138);
	writel(0x2004, IOMUXC_BASE_ADDR + 0x0520);

	/* FEC TX_CLK */
	writel(0x1, IOMUXC_BASE_ADDR + 0x0150);
	writel(0x2180, IOMUXC_BASE_ADDR + 0x0538);
	writel(0x0, IOMUXC_BASE_ADDR + 0x0974);

	/* FEC COL */
	writel(0x1, IOMUXC_BASE_ADDR + 0x0124);
	writel(0x2180, IOMUXC_BASE_ADDR + 0x0500);
	writel(0x0, IOMUXC_BASE_ADDR + 0x094c);

	/* FEC RX_CLK */
	writel(0x1, IOMUXC_BASE_ADDR + 0x0128);
	writel(0x2180, IOMUXC_BASE_ADDR + 0x0504);
	writel(0x0, IOMUXC_BASE_ADDR + 0x0968);

	/* FEC CRS */
	writel(0x3, IOMUXC_BASE_ADDR + 0x0f4);
	writel(0x180, IOMUXC_BASE_ADDR + 0x0488);
	writel(0x0, IOMUXC_BASE_ADDR + 0x0950);

	/* FEC RX_ER */
	writel(0x3, IOMUXC_BASE_ADDR + 0x0f0);
	writel(0x180, IOMUXC_BASE_ADDR + 0x0484);
	writel(0x0, IOMUXC_BASE_ADDR + 0x0970);

	/* FEC RX_DV */
	writel(0x2, IOMUXC_BASE_ADDR + 0x164);
	writel(0x2180, IOMUXC_BASE_ADDR + 0x054C);
	writel(0x0, IOMUXC_BASE_ADDR + 0x096C);
}

static void power_init(void)
{
	struct spi_slave *slave;
	unsigned int val;
	unsigned int reg;

	slave = spi_pmic_probe();

	/* power up the system first */
	pmic_reg(slave, 34, 0x00200000, 1);

	if (mxc_get_clock(MXC_FEC_CLK) > 800000000) {
		/* Set core voltage to 1.175V */
		val = pmic_reg(slave, 24, 0, 0);
		val = (val & (~0x1F)) | 0x17;
		pmic_reg(slave, 24, val, 1);
	}

	/* Setup VCC (SW2) to 1.225 */
	val = pmic_reg(slave, 25, 0, 0);
	val = (val & (~0x1F)) | 0x19;
	pmic_reg(slave, 25, val, 1);

	/* Setup 1V2_DIG1 (SW3) to 1.2 */
	val = pmic_reg(slave, 26, 0, 0);
	val = (val & (~0x1F)) | 0x18;
	pmic_reg(slave, 25, val, 1);

	/* Set VDIG to 1.65V, VGEN3 to 1.8V, VCAM to 2.5V */
	val = pmic_reg(slave, 30, 0, 0);
	val &= ~0x34030;
	val |= 0x10020;
	pmic_reg(slave, 30, val, 1);

	/* Set VVIDEO to 2.775V, VAUDIO to 3V, VSD to 3.15V */
	val = pmic_reg(slave, 31, 0, 0);
	val &= ~0x1FC;
	val |= 0x1F4;
	pmic_reg(slave, 31, val, 1);

	/* Configure VGEN3 and VCAM regulators to use external PNP */
	val = 0x208;
	pmic_reg(slave, 33, val, 1);
	udelay(200);

	reg = readl(GPIO2_BASE_ADDR + 0x0);
	reg &= ~0x4000;  /* Lower reset line */
	writel(reg, GPIO2_BASE_ADDR + 0x0);

	reg = readl(GPIO2_BASE_ADDR + 0x4);
	reg |= 0x4000;  /* configure GPIO lines as output */
	writel(reg, GPIO2_BASE_ADDR + 0x4);

	/* Reset the ethernet controller over GPIO */
	writel(0x1, IOMUXC_BASE_ADDR + 0x0AC);

	/* Enable VGEN3, VCAM, VAUDIO, VVIDEO, VSD regulators */
	val = 0x49249;
	pmic_reg(slave, 33, val, 1);

	udelay(500);

	reg = readl(GPIO2_BASE_ADDR + 0x0);
	reg |= 0x4000;
	writel(reg, GPIO2_BASE_ADDR + 0x0);

	/* Setup the FEC after enabling the regulators */
	setup_fec();

	spi_pmic_free(slave);
}

#ifdef CONFIG_NET_MULTI

#if defined(CONFIG_DRIVER_SMC911X)
extern int smc911x_initialize(bd_t *bis);
#endif
int board_eth_init(bd_t *bis)
{
	int rc = -ENODEV;

#if defined(CONFIG_DRIVER_SMC911X)
	rc = smc911x_initialize(bis);
#endif

	return rc;
}
#endif

#ifdef CONFIG_FSL_MMC

int sdhc_init(void)
{
	u32 interface_esdhc = 0;
	s32 status = 0;

	interface_esdhc = (readl(SRC_BASE_ADDR + 0x4) & (0x00180000)) >> 19;

	switch (interface_esdhc) {
	case 0:

		esdhc_base_pointer = (volatile u32 *)MMC_SDHC1_BASE_ADDR;

		mxc_request_iomux(MX51_PIN_SD1_CMD,
			  IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION);
		mxc_request_iomux(MX51_PIN_SD1_CLK,
			  IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION);

		mxc_request_iomux(MX51_PIN_SD1_DATA0,
				IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION);
		mxc_request_iomux(MX51_PIN_SD1_DATA1,
				IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION);
		mxc_request_iomux(MX51_PIN_SD1_DATA2,
				IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION);
		mxc_request_iomux(MX51_PIN_SD1_DATA3,
				IOMUX_CONFIG_ALT0 | IOMUX_CONFIG_SION);
		mxc_iomux_set_pad(MX51_PIN_SD1_CMD,
				PAD_CTL_DRV_MAX | PAD_CTL_DRV_VOT_HIGH |
				PAD_CTL_HYS_ENABLE | PAD_CTL_47K_PU |
				PAD_CTL_PUE_PULL |
				PAD_CTL_PKE_ENABLE | PAD_CTL_SRE_FAST);
		mxc_iomux_set_pad(MX51_PIN_SD1_CLK,
				PAD_CTL_DRV_MAX | PAD_CTL_DRV_VOT_HIGH |
				PAD_CTL_HYS_NONE | PAD_CTL_47K_PU |
				PAD_CTL_PUE_PULL |
				PAD_CTL_PKE_ENABLE | PAD_CTL_SRE_FAST);
		mxc_iomux_set_pad(MX51_PIN_SD1_DATA0,
				PAD_CTL_DRV_MAX | PAD_CTL_DRV_VOT_HIGH |
				PAD_CTL_HYS_ENABLE | PAD_CTL_47K_PU |
				PAD_CTL_PUE_PULL |
				PAD_CTL_PKE_ENABLE | PAD_CTL_SRE_FAST);
		mxc_iomux_set_pad(MX51_PIN_SD1_DATA1,
				PAD_CTL_DRV_MAX | PAD_CTL_DRV_VOT_HIGH |
				PAD_CTL_HYS_ENABLE | PAD_CTL_47K_PU |
				PAD_CTL_PUE_PULL |
				PAD_CTL_PKE_ENABLE | PAD_CTL_SRE_FAST);
		mxc_iomux_set_pad(MX51_PIN_SD1_DATA2,
				PAD_CTL_DRV_MAX | PAD_CTL_DRV_VOT_HIGH |
				PAD_CTL_HYS_ENABLE | PAD_CTL_47K_PU |
				PAD_CTL_PUE_PULL |
				PAD_CTL_PKE_ENABLE | PAD_CTL_SRE_FAST);
		mxc_iomux_set_pad(MX51_PIN_SD1_DATA3,
				PAD_CTL_DRV_MAX | PAD_CTL_DRV_VOT_HIGH |
				PAD_CTL_HYS_ENABLE | PAD_CTL_100K_PD |
				PAD_CTL_PUE_PULL |
				PAD_CTL_PKE_ENABLE | PAD_CTL_SRE_FAST);
		break;
	case 1:
		status = 1;
		break;
	case 2:
		status = 1;
		break;
	case 3:
		status = 1;
		break;
	default:
		status = 1;
		break;
	}

	return status = 1;
}

#endif

int board_init(void)
{
	setup_soc_rev();

	gd->bd->bi_arch_number = MACH_TYPE_MX51_3DS;	/* board id for linux */
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	setup_uart();
	setup_nfc();
	setup_expio();
	return 0;
}

#ifdef BOARD_LATE_INIT
int board_late_init(void)
{
	power_init();
	return 0;
}
#endif

int checkboard(void)
{
	printf("Board: MX51 3STACK [");
	switch (__REG(SRC_BASE_ADDR + 0x8)) {
	case 0x0001:
		printf("POR");
		break;
	case 0x0009:
		printf("RST");
		break;
	case 0x0010:
	case 0x0011:
		printf("WDOG");
		break;
	default:
		printf("unknown");
	}
	printf("]\n");
	return 0;
}

