/*
 * (C) Copyright 2008-2009 Freescale Semiconductor, Inc.

 * (C) Copyright 2000-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>

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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* #define DEBUG */

#include <common.h>

#if defined(CONFIG_ENV_IS_IN_MMC) /* Environment is in MMC Flash */

#include <command.h>
#include <environment.h>
#include <linux/stddef.h>
#include <malloc.h>
#include <mmc.h>

#if defined(CONFIG_CMD_ENV) && defined(CONFIG_CMD_MMC)
#define CMD_SAVEENV
#elif defined(CONFIG_ENV_OFFSET_REDUND)
#error Cannot use CONFIG_ENV_OFFSET_REDUND without CONFIG_CMD_ENV & CONFIG_CMD_MMC
#endif

#if defined(CONFIG_ENV_SIZE_REDUND) && (CONFIG_ENV_SIZE_REDUND < CONFIG_ENV_SIZE)
#error CONFIG_ENV_SIZE_REDUND should not be less then CONFIG_ENV_SIZE
#endif

#ifdef CONFIG_INFERNO
#error CONFIG_INFERNO not supported yet
#endif

/* references to names in env_common.c */
extern uchar default_environment[];
extern int default_environment_size;

char *env_name_spec = "MMC";

#ifdef ENV_IS_EMBEDDED
extern uchar environment[];
env_t *env_ptr = (env_t *)(&environment[0]);
#else /* ! ENV_IS_EMBEDDED */
env_t *env_ptr;
#endif /* ENV_IS_EMBEDDED */

/* local functions */
#if !defined(ENV_IS_EMBEDDED)
static void use_default(void);
#endif

DECLARE_GLOBAL_DATA_PTR;

uchar env_get_char_spec(int index)
{
	return *((uchar *)(gd->env_addr + index));
}


/* this is called before nand_init()
 * so we can't read Nand to validate env data.
 * Mark it OK for now. env_relocate() in env_common.c
 * will call our relocate function which will does
 * the real validation.
 *
 * When using a NAND boot image (like sequoia_nand), the environment
 * can be embedded or attached to the U-Boot image in NAND flash. This way
 * the SPL loads not only the U-Boot image from NAND but also the
 * environment.
 */
int env_init(void)
{
#if defined(CONFIG_IS_EMBEDDED)
	size_t total;
	int crc1_ok = 0, crc2_ok = 0;
	env_t *tmp_env1, *tmp_env2;

	total = CONFIG_ENV_SIZE;

	tmp_env1 = env_ptr;
	tmp_env2 = (env_t *)((ulong)env_ptr + CONFIG_ENV_SIZE);

	crc1_ok = (crc32(0, tmp_env1->data, ENV_SIZE) == tmp_env1->crc);
	crc2_ok = (crc32(0, tmp_env2->data, ENV_SIZE) == tmp_env2->crc);

	if (!crc1_ok && !crc2_ok)
		gd->env_valid = 0;
	else if (crc1_ok && !crc2_ok)
		gd->env_valid = 1;
	else if (!crc1_ok && crc2_ok)
		gd->env_valid = 2;
	else {
		/* both ok - check serial */
		if (tmp_env1->flags == 255 && tmp_env2->flags == 0)
			gd->env_valid = 2;
		else if (tmp_env2->flags == 255 && tmp_env1->flags == 0)
			gd->env_valid = 1;
		else if (tmp_env1->flags > tmp_env2->flags)
			gd->env_valid = 1;
		else if (tmp_env2->flags > tmp_env1->flags)
			gd->env_valid = 2;
		else /* flags are equal - almost impossible */
			gd->env_valid = 1;
	}

	if (gd->env_valid == 1)
		env_ptr = tmp_env1;
	else if (gd->env_valid == 2)
		env_ptr = tmp_env2;

#else /* ENV_IS_EMBEDDED */
	gd->env_addr  = (ulong)&default_environment[0];
	gd->env_valid = 1;

#endif /* ENV_IS_EMBEDDED */

	return 0;
}

#ifdef CMD_SAVEENV
/*
 * The legacy NAND code saved the environment in the first NAND device i.e.,
 * nand_dev_desc + 0. This is also the behaviour using the new NAND code.
 */
#ifdef CONFIG_ENV_OFFSET_REDUND
int saveenv(void)
{
	size_t total;
	int ret = 0;

	env_ptr->flags++;
	total = CONFIG_ENV_SIZE;

	if (gd->env_valid == 1) {
		puts("Writing to redundant MMC... ");
		ret = mmc_write((u_char *)env_ptr,
				CONFIG_ENV_OFFSET_REDUND, total);
	} else {
		puts("Writing to MMC... ");
		ret = mmc_write((u_char *)env_ptr,
				CONFIG_ENV_OFFSET, total);
	}
	if (ret || total != CONFIG_ENV_SIZE) {
		puts("failed\n");
		return 1;
	}

	puts("done\n");
	gd->env_valid = (gd->env_valid == 2 ? 1 : 2);
	return ret;
}
#else /* ! CONFIG_ENV_OFFSET_REDUND */
int saveenv(void)
{
	size_t total;
	int ret = 0;

	puts("Writing to MMC... ");
	total = CONFIG_ENV_SIZE;
	ret = mmc_write((u_char *)env_ptr, CONFIG_ENV_OFFSET, total);
	if (ret || total != CONFIG_ENV_SIZE) {
		puts("failed\n");
		return 1;
	}

	puts("done\n");
	return ret;
}
#endif /* CONFIG_ENV_OFFSET_REDUND */
#endif /* CMD_SAVEENV */

#ifdef CONFIG_ENV_OFFSET_REDUND
void env_relocate_spec(void)
{
#if !defined(ENV_IS_EMBEDDED)
	size_t total;
	int crc1_ok = 0, crc2_ok = 0;
	env_t *tmp_env1 = NULL, *tmp_env2 = NULL;

	puts("Initialing MMC card... \n");

	if (mmc_init(1) != 0) {
		puts("No MMC card found\n");
		goto use_default;
	}

	total = CONFIG_ENV_SIZE;

	tmp_env1 = (env_t *)malloc(CONFIG_ENV_SIZE);
	if (!tmp_env1) {
		puts("Not enough memory!\n");
		goto use_default;
	}
	memset(tmp_env1, 0, CONFIG_ENV_SIZE);

	tmp_env2 = (env_t *)malloc(CONFIG_ENV_SIZE);
	if (!tmp_env2) {
		puts("Not enough memory!\n");
		goto use_default;
	}
	memset(tmp_env2, 0, CONFIG_ENV_SIZE);

	puts("Loading environment from mmc... ");
	if (mmc_read(CONFIG_ENV_OFFSET, (uchar *)tmp_env1, total)) {
		puts("failed\n");
		goto use_default;
	}
	puts("done\n");

	puts("Loading redundant environment from mmc... ");
	if (mmc_read(CONFIG_ENV_OFFSET_REDUND, (uchar *)tmp_env2, total)) {
		puts("failed\n");
		goto use_default;
	}
	puts("done\n");

	crc1_ok = (crc32(0, tmp_env1->data, ENV_SIZE) == tmp_env1->crc);
	crc2_ok = (crc32(0, tmp_env2->data, ENV_SIZE) == tmp_env2->crc);

	if (!crc1_ok && !crc2_ok)
		goto use_default;
	else if (crc1_ok && !crc2_ok)
		gd->env_valid = 1;
	else if (!crc1_ok && crc2_ok)
		gd->env_valid = 2;
	else {
		/* both ok - check serial */
		if (tmp_env1->flags == 255 && tmp_env2->flags == 0)
			gd->env_valid = 2;
		else if (tmp_env2->flags == 255 && tmp_env1->flags == 0)
			gd->env_valid = 1;
		else if (tmp_env1->flags > tmp_env2->flags)
			gd->env_valid = 1;
		else if (tmp_env2->flags > tmp_env1->flags)
			gd->env_valid = 2;
		else /* flags are equal - almost impossible */
			gd->env_valid = 1;
	}

	free(env_ptr);
	if (gd->env_valid == 1) {
		env_ptr = tmp_env1;
		free(tmp_env2);
	} else {
		env_ptr = tmp_env2;
		free(tmp_env1);
	}

	return;

use_default:
	if (tmp_env1)
		free(tmp_env1);
	if (tmp_env2)
		free(tmp_env2);
	return use_default();

#endif /* ! ENV_IS_EMBEDDED */
}
#else /* ! CONFIG_ENV_OFFSET_REDUND */
/*
 * The legacy NAND code saved the environment in the first NAND device i.e.,
 * nand_dev_desc + 0. This is also the behaviour using the new NAND code.
 */
void env_relocate_spec(void)
{
#if !defined(ENV_IS_EMBEDDED)
	size_t total;
	int ret;

	if (mmc_init(1) != 0) {
		puts("No MMC card found\n");
		return;
	}

	total = CONFIG_ENV_SIZE;
	ret = mmc_read(CONFIG_ENV_OFFSET, (u_char *)env_ptr, total);
	if (ret || total != CONFIG_ENV_SIZE)
		return use_default();

	if (crc32(0, env_ptr->data, ENV_SIZE) != env_ptr->crc)
		return use_default();
#endif /* ! ENV_IS_EMBEDDED */
}
#endif /* CONFIG_ENV_OFFSET_REDUND */

#if !defined(ENV_IS_EMBEDDED)
static void use_default()
{
	puts("*** Warning - bad CRC or MMC Card, using default environment\n\n");

	if (default_environment_size > CONFIG_ENV_SIZE) {
		puts("*** Error - default environment is too large\n\n");
		return;
	}

	memset(env_ptr, 0, sizeof(env_t));
	memcpy(env_ptr->data,
		default_environment,
		default_environment_size);
	env_ptr->crc = crc32(0, env_ptr->data, ENV_SIZE);
	gd->env_valid = 1;
}
#endif

#endif /* CONFIG_ENV_IS_IN_MMC */
