/*
 * (c) Copyright 2009 Freescale Semiconductor
 *
 * (c) 2007 Pengutronix, Sascha Hauer <s.hauer@pengutronix.de>
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
#include <asm/errno.h>
#include <asm/arch/mx25.h>
#include <asm/arch/mx25-regs.h>
#include <asm/arch/mx25_pins.h>
#include <asm/arch/iomux.h>
#include <asm/arch/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

static u32 system_rev;

u32 get_board_rev(void)
{
	return system_rev;
}

static inline void setup_soc_rev(void)
{
	int reg;
	reg = __REG(IIM_BASE + IIM_SREV);
	if (!reg) {
		reg = __REG(ROMPATCH_REV);
		reg <<= 4;
	} else
		reg += CHIP_REV_1_0;
	system_rev = 0x25000 + (reg & 0xFF);
}

int dram_init(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return 0;
}

int board_init(void)
{
	int pad;
	u8 reg[4];

	setup_soc_rev();

	/* setup pins for UART1 */
	/* UART 1 IOMUX Configs */
	mxc_request_iomux(MX25_PIN_UART1_RXD, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_UART1_TXD, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_UART1_RTS, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_UART1_CTS, MUX_CONFIG_FUNC);
	mxc_iomux_set_pad(MX25_PIN_UART1_RXD,
			PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE |
			PAD_CTL_PUE_PUD | PAD_CTL_100K_PU);
	mxc_iomux_set_pad(MX25_PIN_UART1_TXD,
			PAD_CTL_PUE_PUD | PAD_CTL_100K_PD);
	mxc_iomux_set_pad(MX25_PIN_UART1_RTS,
			PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE |
			PAD_CTL_PUE_PUD | PAD_CTL_100K_PU);
	mxc_iomux_set_pad(MX25_PIN_UART1_CTS,
			PAD_CTL_PUE_PUD | PAD_CTL_100K_PD);

	/* setup pins for FEC */
	mxc_request_iomux(MX25_PIN_FEC_TX_CLK, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_FEC_RX_DV, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_FEC_RDATA0, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_FEC_TDATA0, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_FEC_TX_EN, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_FEC_MDC, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_FEC_MDIO, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_FEC_RDATA1, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_FEC_TDATA1, MUX_CONFIG_FUNC);
	mxc_request_iomux(MX25_PIN_POWER_FAIL, MUX_CONFIG_FUNC); /* PHY INT */

#define FEC_PAD_CTL1 (PAD_CTL_HYS_SCHMITZ | PAD_CTL_PUE_PUD | \
			PAD_CTL_PKE_ENABLE)
#define FEC_PAD_CTL2 (PAD_CTL_PUE_PUD)

	mxc_iomux_set_pad(MX25_PIN_FEC_TX_CLK, FEC_PAD_CTL1);
	mxc_iomux_set_pad(MX25_PIN_FEC_RX_DV, FEC_PAD_CTL1);
	mxc_iomux_set_pad(MX25_PIN_FEC_RDATA0, FEC_PAD_CTL1);
	mxc_iomux_set_pad(MX25_PIN_FEC_TDATA0, FEC_PAD_CTL2);
	mxc_iomux_set_pad(MX25_PIN_FEC_TX_EN, FEC_PAD_CTL2);
	mxc_iomux_set_pad(MX25_PIN_FEC_MDC, FEC_PAD_CTL2);
	mxc_iomux_set_pad(MX25_PIN_FEC_MDIO, FEC_PAD_CTL1 | PAD_CTL_22K_PU);
	mxc_iomux_set_pad(MX25_PIN_FEC_RDATA1, FEC_PAD_CTL1);
	mxc_iomux_set_pad(MX25_PIN_FEC_TDATA1, FEC_PAD_CTL2);
	mxc_iomux_set_pad(MX25_PIN_POWER_FAIL, FEC_PAD_CTL1);

	/*
	 * Set up the FEC_RESET_B and FEC_ENABLE GPIO pins.
	 * Assert FEC_RESET_B, then power up the PHY by asserting
	 * FEC_ENABLE, at the same time lifting FEC_RESET_B.
	 *
	 * FEC_RESET_B: gpio2[3] is ALT 5 mode of pin D12
	 * FEC_ENABLE_B: gpio4[8] is ALT 5 mode of pin A17
	 */
	mxc_request_iomux(MX25_PIN_A17, MUX_CONFIG_ALT5); /* FEC_EN */
	mxc_request_iomux(MX25_PIN_D12, MUX_CONFIG_ALT5); /* FEC_RESET_B */

	mxc_iomux_set_pad(MX25_PIN_A17, PAD_CTL_ODE_OpenDrain);
	mxc_iomux_set_pad(MX25_PIN_D12, 0);

	mxc_set_gpio_direction(MX25_PIN_A17, 0); /* FEC_EN */
	mxc_set_gpio_direction(MX25_PIN_D12, 0); /* FEC_RESET_B */

	/* drop PHY power */
	mxc_set_gpio_dataout(MX25_PIN_A17, 0);	/* FEC_EN */

	/* assert reset */
	mxc_set_gpio_dataout(MX25_PIN_D12, 0);	/* FEC_RESET_B */
	udelay(2);		/* spec says 1us min */

	/* turn on PHY power and lift reset */
	mxc_set_gpio_dataout(MX25_PIN_A17, 1);	/* FEC_EN */
	mxc_set_gpio_dataout(MX25_PIN_D12, 1);	/* FEC_RESET_B */

#define I2C_PAD_CTL (PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE | \
		PAD_CTL_PUE_PUD | PAD_CTL_100K_PU | PAD_CTL_ODE_OpenDrain)

	mxc_request_iomux(MX25_PIN_I2C1_CLK, MUX_CONFIG_SION);
	mxc_request_iomux(MX25_PIN_I2C1_DAT, MUX_CONFIG_SION);
	mxc_iomux_set_pad(MX25_PIN_I2C1_CLK, 0x1E8);
	mxc_iomux_set_pad(MX25_PIN_I2C1_DAT, 0x1E8);

	gd->bd->bi_arch_number = MACH_TYPE_MX25_3DS;    /* board id for linux */
	gd->bd->bi_boot_params = 0x80000100;    /* address of boot parameters */

	return 0;

#undef FEC_PAD_CTL1
#undef FEC_PAD_CTL2
#undef I2C_PAD_CTL
}

#ifdef BOARD_LATE_INIT
int board_late_init(void)
{
	u8 reg[4];

	/* Turn PMIC On*/
	reg[0] = 0x09;
	i2c_write(0x54, 0x02, 1, reg, 1);
}
#endif


int checkboard(void)
{
	printf("Board: i.MX25 MAX PDK (3DS)\n");
	return 0;
}

int board_eth_init(bd_t *bis)
{
	int rc = -ENODEV;
#if defined(CONFIG_DRIVER_SMC911X)
	rc = smc911x_initialize(bis);
#endif
	return rc;
}

