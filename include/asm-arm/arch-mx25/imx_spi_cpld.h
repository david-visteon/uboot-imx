/*
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _IMX_SPI_CPLD_H_
#define _IMX_SPI_CPLD_H_

#include <linux/types.h>

extern struct spi_slave *spi_cpld_probe();
extern void spi_cpld_free(struct spi_slave *slave);
extern unsigned int cpld_reg_xfer(unsigned int reg, unsigned int val,
				  unsigned int read);

#endif				/* _IMX_SPI_CPLD_H_ */
