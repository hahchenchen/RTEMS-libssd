/*
 * Copyright (c) 2010
 *	Ben Gray <ben.r.gray@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Ben Gray.
 * 4. The name of the company nor the name of the author may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY BEN GRAY ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL BEN GRAY BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Power, Reset and Clock Management Module
 *
 * This is a very simple driver wrapper around the PRCM set of registers in
 * the OMAP3 chip. It allows you to turn on and off things like the functional
 * and interface clocks to the various on-chip modules.
 *
 */
#include <machine/rtems-bsd-kernel-space.h>

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <machine/rtems-bsd-param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/bus.h>
#include <sys/resource.h>
#include <sys/rman.h>

#ifndef __rtems__
#include <sys/lock.h>
#else
#include <rtems/bsd/sys/lock.h>
#endif

#include <sys/mutex.h>

#include <machine/bus.h>
#include <machine/resource.h>
#ifndef __rtems__
#include <machine/intr.h>
#endif


#include <arm/ti/ti_cpuid.h>


#include <arm/ti/ti_prcm.h>

#define CM_PER				0
#define CM_PER_USB0_CLKCTRL		(CM_PER + 0x01C)

#define CM_WKUP				0x400
#define CM_WKUP_CLKSTCTRL		(CM_WKUP + 0x000)
#define CM_WKUP_CONTROL_CLKCTRL		(CM_WKUP + 0x004)
#define CM_WKUP_GPIO0_CLKCTRL		(CM_WKUP + 0x008)
#define CM_WKUP_CM_L3_AON_CLKSTCTRL	(CM_WKUP + 0x01C)
#define CM_WKUP_CM_CLKSEL_DPLL_MPU	(CM_WKUP + 0x02C)
#define CM_WKUP_CM_IDLEST_DPLL_DISP	(CM_WKUP + 0x048)
#define CM_WKUP_CM_CLKSEL_DPLL_DISP	(CM_WKUP + 0x054)
#define CM_WKUP_CM_CLKDCOLDO_DPLL_PER	(CM_WKUP + 0x07C)
#define CM_WKUP_CM_CLKMODE_DPLL_DISP	(CM_WKUP + 0x098)
#define CM_WKUP_I2C0_CLKCTRL		(CM_WKUP + 0x0B8)
#define CM_WKUP_ADC_TSC_CLKCTRL		(CM_WKUP + 0x0BC)

static int
am335x_clk_musb0_activate(struct ti_clock_dev *clkdev);


struct ti_clock_dev ti_am335x_clk_devmap[] = {

	/* Mentor USB HS controller core clocks */
	{	.id                  = MUSB0_CLK,
		.clk_activate        = am335x_clk_musb0_activate,
		.clk_deactivate      = NULL,
		.clk_set_source      = NULL,
		.clk_accessible      = NULL,
		.clk_get_source_freq = NULL,
		.clk_set_source_freq = NULL,
	},


};

static int
am335x_clk_musb0_activate(struct ti_clock_dev *clkdev)
{
//	struct am335x_prcm_softc *sc = am335x_prcm_sc;

//	if (sc == NULL)
//		return ENXIO;

	/* set ST_DPLL_CLKDCOLDO(9) to CLK_GATED(1) */
	/* set DPLL_CLKDCOLDO_GATE_CTRL(8) to CLK_ENABLE(1)*/
	(*(volatile uint32_t *)(0x44E0047c))=0x300;
    //    prcm_write_4(CM_WKUP_CM_CLKDCOLDO_DPLL_PER, 0x300);

	/*set MODULEMODE to ENABLE(2) */
	//prcm_write_4(CM_PER_USB0_CLKCTRL, 2);
   (*(volatile uint32_t *)(0x44E0001c))=2;
	/* wait for MODULEMODE to become ENABLE(2) */
	while (((*(volatile uint32_t *)(0x44E0001c)) & 0x3) != 2)
		DELAY(10);

	/* wait for IDLEST to become Func(0) */
	while((*(volatile uint32_t *)(0x44E0001c)) & (3<<16))
		DELAY(10);

	return(0);
}

/**
 *	ti_*_clk_devmap - Array of clock devices, should be defined one per SoC 
 *
 *	This array is typically defined in one of the targeted *_prcm_clk.c
 *	files and is specific to the given SoC platform.  Each entry in the array
 *	corresponds to an individual clock device.
 */
 struct ti_clock_dev ti_omap4_clk_devmap[];
 //struct ti_clock_dev ti_am335x_clk_devmap[];

/**
 *	ti_prcm_clk_dev - returns a pointer to the clock device with given id
 *	@clk: the ID of the clock device to get
 *
 *	Simply iterates through the clk_devmap global array and returns a pointer
 *	to the clock device if found. 
 *
 *	LOCKING:
 *	None
 *
 *	RETURNS:
 *	The pointer to the clock device on success, on failure NULL is returned.
 */
static struct ti_clock_dev *
ti_prcm_clk_dev(clk_ident_t clk)
{
	struct ti_clock_dev *clk_dev;
	
	/* Find the clock within the devmap - it's a bit inefficent having a for 
	 * loop for this, but this function should only called when a driver is 
	 * being activated so IMHO not a big issue.
	 */
	clk_dev = NULL;



	switch(ti_chip()) {


	case CHIP_AM335X:
	printf("CHIP_AM335X\n");
		clk_dev = &(ti_am335x_clk_devmap[0]);
		break;

	}








	if (clk_dev == NULL)
		panic("No clock devmap found");
	printf("clk_dev->id:%x\n",clk_dev->id );
	while (clk_dev->id != INVALID_CLK_IDENT) {
		if (clk_dev->id == clk) {
			return (clk_dev);
		}
		clk_dev++;
	}

	/* Sanity check we managed to find the clock */
	printf("ti_prcm: Failed to find clock device (%d)\n", clk);
	return (NULL);
}

/**
 *	ti_prcm_clk_valid - enables a clock for a particular module
 *	@clk: identifier for the module to enable, see ti_prcm.h for a list
 *	      of possible modules.
 *	         Example: OMAP3_MODULE_MMC1_ICLK or OMAP3_MODULE_GPTIMER10_FCLK.
 *	
 *	This function can enable either a functional or interface clock.
 *
 *	The real work done to enable the clock is really done in the callback
 *	function associated with the clock, this function is simply a wrapper
 *	around that.
 *
 *	LOCKING:
 *	Internally locks the driver context.
 *
 *	RETURNS:
 *	Returns 0 on success or positive error code on failure.
 */
int
ti_prcm_clk_valid(clk_ident_t clk)
{
	int ret = 0;

	if (ti_prcm_clk_dev(clk) == NULL)
		ret = EINVAL;
	
	return (ret);
}


/**
 *	ti_prcm_clk_enable - enables a clock for a particular module
 *	@clk: identifier for the module to enable, see ti_prcm.h for a list
 *	      of possible modules.
 *	         Example: OMAP3_MODULE_MMC1_ICLK or OMAP3_MODULE_GPTIMER10_FCLK.
 *	
 *	This function can enable either a functional or interface clock.
 *
 *	The real work done to enable the clock is really done in the callback
 *	function associated with the clock, this function is simply a wrapper
 *	around that.
 *
 *	LOCKING:
 *	Internally locks the driver context.
 *
 *	RETURNS:
 *	Returns 0 on success or positive error code on failure.
 */
int
ti_prcm_clk_enable(clk_ident_t clk)
{
	struct ti_clock_dev *clk_dev;
	int ret;

	/* Find the clock within the devmap - it's a bit inefficent having a for 
	 * loop for this, but this function should only called when a driver is 
	 * being activated so IMHO not a big issue.
	 */
	clk_dev = ti_prcm_clk_dev(clk);

	/* Sanity check we managed to find the clock */
	if (clk_dev == NULL)
		return (EINVAL);

	/* Activate the clock */
	if (clk_dev->clk_activate)
		ret = clk_dev->clk_activate(clk_dev);
	else
		ret = EINVAL;

	return (ret);
}


/**
 *	ti_prcm_clk_disable - disables a clock for a particular module
 *	@clk: identifier for the module to enable, see ti_prcm.h for a list
 *	      of possible modules.
 *	         Example: OMAP3_MODULE_MMC1_ICLK or OMAP3_MODULE_GPTIMER10_FCLK.
 *	
 *	This function can enable either a functional or interface clock.
 *
 *	The real work done to enable the clock is really done in the callback
 *	function associated with the clock, this function is simply a wrapper
 *	around that.
 *
 *	LOCKING:
 *	Internally locks the driver context.
 *
 *	RETURNS:
 *	Returns 0 on success or positive error code on failure.
 */
int
ti_prcm_clk_disable(clk_ident_t clk)
{
	struct ti_clock_dev *clk_dev;
	int ret;

	/* Find the clock within the devmap - it's a bit inefficent having a for 
	 * loop for this, but this function should only called when a driver is 
	 * being activated so IMHO not a big issue.
	 */
	clk_dev = ti_prcm_clk_dev(clk);

	/* Sanity check we managed to find the clock */
	if (clk_dev == NULL)
		return (EINVAL);

	/* Activate the clock */
	if (clk_dev->clk_deactivate)
		ret = clk_dev->clk_deactivate(clk_dev);
	else
		ret = EINVAL;
	
	return (ret);
}

/**
 *	ti_prcm_clk_set_source - sets the source 
 *	@clk: identifier for the module to enable, see ti_prcm.h for a list
 *	      of possible modules.
 *	         Example: OMAP3_MODULE_MMC1_ICLK or OMAP3_MODULE_GPTIMER10_FCLK.
 *	
 *	This function can enable either a functional or interface clock.
 *
 *	The real work done to enable the clock is really done in the callback
 *	function associated with the clock, this function is simply a wrapper
 *	around that.
 *
 *	LOCKING:
 *	Internally locks the driver context.
 *
 *	RETURNS:
 *	Returns 0 on success or positive error code on failure.
 */
int
ti_prcm_clk_set_source(clk_ident_t clk, clk_src_t clksrc)
{
	struct ti_clock_dev *clk_dev;
	int ret;

	/* Find the clock within the devmap - it's a bit inefficent having a for 
	 * loop for this, but this function should only called when a driver is 
	 * being activated so IMHO not a big issue.
	 */
	clk_dev = ti_prcm_clk_dev(clk);

	/* Sanity check we managed to find the clock */
	if (clk_dev == NULL)
		return (EINVAL);

	/* Activate the clock */
	if (clk_dev->clk_set_source)
		ret = clk_dev->clk_set_source(clk_dev, clksrc);
	else
		ret = EINVAL;

	return (ret);
}


/**
 *	ti_prcm_clk_get_source_freq - gets the source clock frequency
 *	@clk: identifier for the module to enable, see ti_prcm.h for a list
 *	      of possible modules.
 *	@freq: pointer to an integer that upon return will contain the src freq
 *
 *	This function returns the frequency of the source clock.
 *
 *	The real work done to enable the clock is really done in the callback
 *	function associated with the clock, this function is simply a wrapper
 *	around that.
 *
 *	LOCKING:
 *	Internally locks the driver context.
 *
 *	RETURNS:
 *	Returns 0 on success or positive error code on failure.
 */
int
ti_prcm_clk_get_source_freq(clk_ident_t clk, unsigned int *freq)
{
	struct ti_clock_dev *clk_dev;
	int ret;

	/* Find the clock within the devmap - it's a bit inefficent having a for 
	 * loop for this, but this function should only called when a driver is 
	 * being activated so IMHO not a big issue.
	 */
	clk_dev = ti_prcm_clk_dev(clk);

	/* Sanity check we managed to find the clock */
	if (clk_dev == NULL)
		return (EINVAL);

	/* Get the source frequency of the clock */
	if (clk_dev->clk_get_source_freq)
		ret = clk_dev->clk_get_source_freq(clk_dev, freq);
	else
		ret = EINVAL;

	return (ret);
}

/**
 *	ti_prcm_clk_set_source_freq - sets the source clock frequency as close to freq as possible
 *	@clk: identifier for the module to enable, see ti_prcm.h for a list
 *	      of possible modules.
 *	@freq: requested freq
 *
 *	LOCKING:
 *	Internally locks the driver context.
 *
 *	RETURNS:
 *	Returns 0 on success or positive error code on failure.
 */
int
ti_prcm_clk_set_source_freq(clk_ident_t clk, unsigned int freq)
{
	struct ti_clock_dev *clk_dev;
	int ret;

	clk_dev = ti_prcm_clk_dev(clk);

	/* Sanity check we managed to find the clock */
	if (clk_dev == NULL)
		return (EINVAL);

	/* Get the source frequency of the clock */
	if (clk_dev->clk_set_source_freq)
		ret = clk_dev->clk_set_source_freq(clk_dev, freq);
	else
		ret = EINVAL;

	return (ret);
}
