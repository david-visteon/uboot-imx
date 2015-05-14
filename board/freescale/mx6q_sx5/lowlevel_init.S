/*
 * Copyright (C) 2010-2012 Freescale Semiconductor, Inc.
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

#include <config.h>
#include <asm/arch/mx6.h>

/*
 Disable L2Cache because ROM turn it on when uboot use plug-in.
 If L2Cache is on default, there are cache coherence problem if kernel have
 not config L2Cache.
*/
.macro init_l2cc
    ldr     r1, =0xa02000
    ldr     r0, =0x0
    str     r0, [r1, #0x100]
.endm /* init_l2cc */

/* invalidate the D-CACHE */
.macro inv_dcache
    mov     r0,#0
    mcr     p15,2,r0,c0,c0,0  /* cache size selection register, select dcache */
    mrc     p15,1,r0,c0,c0,0  /* cache size ID register */
    mov     r0,r0,ASR #13
    ldr     r3,=0xfff
    and     r0,r0,r3
    cmp     r0,#0x7f
    moveq   r6,#0x1000
    beq     size_done
    cmp     r0,#0xff
    moveq   r6,#0x2000
    movne   r6,#0x4000

size_done:
    mov     r2,#0
    mov     r3,#0x40000000
    mov     r4,#0x80000000
    mov     r5,#0xc0000000

d_inv_loop:
    mcr     p15,0,r2,c7,c6,2  /* invalidate dcache by set / way */
    mcr     p15,0,r3,c7,c6,2  /* invalidate dcache by set / way */
    mcr     p15,0,r4,c7,c6,2  /* invalidate dcache by set / way */
    mcr     p15,0,r5,c7,c6,2  /* invalidate dcache by set / way */
    add     r2,r2,#0x20
    add     r3,r3,#0x20
    add     r4,r4,#0x20
    add     r5,r5,#0x20

    cmp     r2,r6
    bne     d_inv_loop
.endm

/* AIPS setup - Only setup MPROTx registers.
 * The PACR default values are good.*/
.macro init_aips
	/*
	 * Set all MPROTx to be non-bufferable, trusted for R/W,
	 * not forced to user-mode.
	 */
	ldr r0, =AIPS1_ON_BASE_ADDR
	ldr r1, =0x77777777
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r1, =0x0
	str r1, [r0, #0x40]
	str r1, [r0, #0x44]
	str r1, [r0, #0x48]
	str r1, [r0, #0x4C]
	str r1, [r0, #0x50]

	ldr r0, =AIPS2_ON_BASE_ADDR
	ldr r1, =0x77777777
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r1, =0x0
	str r1, [r0, #0x40]
	str r1, [r0, #0x44]
	str r1, [r0, #0x48]
	str r1, [r0, #0x4C]
	str r1, [r0, #0x50]
.endm /* init_aips */

.macro setup_pll pll, freq
.endm

.macro init_clock

/* PLL1, PLL2, and PLL3 are enabled by ROM */
#ifdef CONFIG_PLL3
	/* enable PLL3 for UART */
	ldr r0, ANATOP_BASE_ADDR_W

	/* power up PLL */
	ldr r1, [r0, #ANATOP_USB1]
	orr r1, r1, #0x1000
	str r1, [r0, #ANATOP_USB1]

	/* enable PLL */
	ldr r1, [r0, #ANATOP_USB1]
	orr r1, r1, #0x2000
	str r1, [r0, #ANATOP_USB1]

	/* wait PLL lock */
100:
	ldr r1, [r0, #ANATOP_USB1]
	mov r1, r1, lsr #31
	cmp r1, #0x1
	bne 100b

	/* clear bypass bit */
	ldr r1, [r0, #ANATOP_USB1]
	and r1, r1, #0xfffeffff
	str r1, [r0, #ANATOP_USB1]
#endif

/* the following was added for the Volans rev B board */

#ifdef CONFIG_PLL7
    /* enable PLL7 for USB - host 1, 2, & 3 - 480 mhz */
    ldr r0, ANATOP_BASE_ADDR_W

    /* power up PLL */
    ldr r1, [r0, #ANATOP_USB2]
    orr r1, r1, #0x1000
    str r1, [r0, #ANATOP_USB2]

    /* enable PLL */
    ldr r1, [r0, #ANATOP_USB2]
    orr r1, r1, #0x2000
    str r1, [r0, #ANATOP_USB2]

    /* wait PLL lock */
100:
    ldr r1, [r0, #ANATOP_USB2]
    mov r1, r1, lsr #31
    cmp r1, #0x1
    bne 100b

    /* clear bypass bit */
    ldr r1, [r0, #ANATOP_USB2]
    and r1, r1, #0xfffeffff
    str r1, [r0, #ANATOP_USB2]
#endif
//#endif  // #if 0 -------------------------------------------------

        /* Restore the default values in the Gate registers */
        ldr r0, CCM_BASE_ADDR_W
        ldr r1, =0xffc03fff
        str r1, [r0, #CLKCTL_CCGR0]
        ldr r1, =0xfffcfcff
        str r1, [r0, #CLKCTL_CCGR1]
        ldr r1, =0xfc3fffcc
        str r1, [r0, #CLKCTL_CCGR2]
        ldr r1, =0xfff033c0
        str r1, [r0, #CLKCTL_CCGR3]
        ldr r1, =0xfffffffc
        str r1, [r0, #CLKCTL_CCGR4]
        ldr r1, =0xffff3fcf
        str r1, [r0, #CLKCTL_CCGR5]
        ldr r1, =0xffffffff
        str r1, [r0, #CLKCTL_CCGR6]
        ldr r1, =0xffffffff
        str r1, [r0, #CLKCTL_CCGR7]

        /* initialize the serial clock multiplexer register 1 */
        ldr r1, =0x10f0a800
        str r1, [r0, #CLKCTL_CSCMR1]

        /* initialize the serial clock multiplexer register 2 */
        ldr r1, =0x02a12f02
        str r1, [r0, #CLKCTL_CSCMR2]

        /* configure ssi1 clock divider register */
        ldr r1, =0x0e5a025a
        str r1, [r0, #CLKCTL_CS1CDR]

        /* configure ssi2 clock divider register */
        /*ldr r1, =0x0106365a*/
        ldr r1, =0x7305a
        str r1, [r0, #CLKCTL_CS2CDR]

        /* initialize the HSC clock divider register */
/*        ldr r1, =0x0002a953
        str r1, [r0, #CLKCTL_CHSCCDR]
*/
        ldr r1, =0x0002a153
        str r1, [r0, #CLKCTL_CSCDR2]

        /* initialize the clock output source register */
        ldr r1, =0x012e008e
        str r1, [r0, #CLKCTL_CCOSR]

        /* configure the general purpose registers        */
        /* IOMUX_GPR1 -- set up the LVDS mode register */
        ldr r0, =0x020e0000
        ldr r1, =0x48780005         /* JEIDA, 24 bit, and LVDS channel 1 ==> DI1 */
        str r1, [r0, #0x04]

        /* IOMUX_GPR2 -- set up the LVDS mode register */
        ldr r1, =0x00000021         /* JEIDA, 24 bit, and LVDS channel 1 ==> DI1 */
        str r1, [r0, #0x08]

        /* IOMUX_GPR3 -- set up for LVDS Mux control register settings */
        ldr r1, =0x00000080         /* IPU1_DI1, IPU2_DI1, & HDMI_OUT  */
        str r1, [r0, #0x0c]

.endm

.section ".text.init", "x"

.globl lowlevel_init
lowlevel_init:

	inv_dcache

	init_l2cc

	init_aips

	init_clock

	mov pc, lr

/* Board level setting value */
ANATOP_BASE_ADDR_W:		.word ANATOP_BASE_ADDR
CCM_BASE_ADDR_W:		.word CCM_BASE_ADDR
