/*
 * Copyright (c) 2013-2015 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * Copyright (c) 2016 Chris Johns <chrisj@rtems.org> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if !defined(BSP_NEXUS_DEVICES_h)
#define BSP_NEXUS_DEVICES_h

#include <bsp.h>

#include <rtems/bsd/bsd.h>
#include <machine/rtems-bsd-nexus-bus.h>


#if defined(LIBBSP_ARM_REALVIEW_PBX_A9_BSP_H)

#include <bsp/irq.h>

static const rtems_bsd_device_resource smc0_res[] = {
	{
		.type = RTEMS_BSD_RES_MEMORY,
		.start_request = 0,
		.start_actual = 0x4e000000
	}, {
		.type = RTEMS_BSD_RES_IRQ,
		.start_request = 0,
		.start_actual = RVPBXA9_IRQ_ETHERNET
	}
};


RTEMS_BSD_DEFINE_NEXUS_DEVICE(smc, 0, RTEMS_ARRAY_SIZE(smc0_res),
    &smc0_res[0]);

#elif defined(LIBBSP_ARM_BEAGLE_BSP_H)

     static const rtems_bsd_device_resource cpsw0_res[] = {
        {
                .type = RTEMS_BSD_RES_MEMORY,
                .start_request = 0,
                .start_actual = 0x4a100000 /*Memory map address of the ethernet*/
        }, {
                .type = RTEMS_BSD_RES_IRQ,
                .start_request = 0,
                .start_actual = 0x28
        },
           {
                .type = RTEMS_BSD_RES_IRQ,
                .start_request = 1,
                .start_actual = 0x29
        },
           {
                .type = RTEMS_BSD_RES_IRQ,
                .start_request = 2,
                .start_actual = 0x2a
        },
           {
                .type = RTEMS_BSD_RES_IRQ,
                .start_request = 3,
                .start_actual = 0x2b
        }
};

RTEMS_BSD_DEFINE_NEXUS_DEVICE(cpsw, 0, RTEMS_ARRAY_SIZE(cpsw0_res), &cpsw0_res[0]);

SYSINIT_DRIVER_REFERENCE(smscphy, miibus);


    static const rtems_bsd_device_resource musbotg_res1[] = {
        {
                .type = RTEMS_BSD_RES_MEMORY,
                .start_request = 0,
                .start_actual = 0x47401c00 /*Memory map address of the ethernet*/
        },
        {
                .type = RTEMS_BSD_RES_MEMORY,
                .start_request = 1,
                .start_actual = 0x47401800 /*Memory map address of the ethernet*/
        },
        {
                .type = RTEMS_BSD_RES_IRQ,
                .start_request = 0,
                .start_actual = 0x12
        }
};

RTEMS_BSD_DEFINE_NEXUS_DEVICE(musbotg,1 , RTEMS_ARRAY_SIZE(musbotg_res1), &musbotg_res1[0]);




    static const rtems_bsd_device_resource musbotg_res[] = {
        {
                .type = RTEMS_BSD_RES_MEMORY,
                .start_request = 0,
                .start_actual = 0x47401400 /*Memory map address of the ethernet*/
        },
        {
                .type = RTEMS_BSD_RES_MEMORY,
                .start_request = 1,
                .start_actual = 0x47401000 /*Memory map address of the ethernet*/
        },
        {
                .type = RTEMS_BSD_RES_IRQ,
                .start_request = 0,
                .start_actual = 0x11
        }

};

RTEMS_BSD_DEFINE_NEXUS_DEVICE(musbotg,0 , RTEMS_ARRAY_SIZE(musbotg_res), &musbotg_res[0]);



    static const rtems_bsd_device_resource usbss[] = {
        {
                .type = RTEMS_BSD_RES_MEMORY,
                .start_request = 0,
                .start_actual = 0x47400000 /*Memory map address of the ethernet*/
        }
};


RTEMS_BSD_DEFINE_NEXUS_DEVICE(usbss,0 , RTEMS_ARRAY_SIZE(usbss), &usbss[0]);



    static const rtems_bsd_device_resource am335x_prcm[] = {
        {
                .type = RTEMS_BSD_RES_MEMORY,
                .start_request = 0,
                .start_actual = 0x44E00000 /*Memory map address of the ethernet*/
        }
};


RTEMS_BSD_DEFINE_NEXUS_DEVICE(am335x_prcm,0 , RTEMS_ARRAY_SIZE(am335x_prcm), &am335x_prcm[0]);



    static const rtems_bsd_device_resource ti_scm[] = {
        {
                .type = RTEMS_BSD_RES_MEMORY,
                .start_request = 0,
                .start_actual = 0x44e10000 /*Memory map address of the ethernet*/
        }
};


RTEMS_BSD_DEFINE_NEXUS_DEVICE(ti_scm,0 , RTEMS_ARRAY_SIZE(ti_scm), &ti_scm[0]);



//RTEMS_BSD_DEFINE_NEXUS_DEVICE(pci, 0, 0, NULL);

//SYSINIT_DRIVER_REFERENCE(usbus, usbus);
//SYSINIT_DRIVER_REFERENCE(umass, nexus);
 //SYSINIT_DRIVER_REFERENCE(uhub, usbus);
SYSINIT_DRIVER_REFERENCE(uhub, uhub);
SYSINIT_DRIVER_REFERENCE(usbus, musbotg);
RTEMS_BSD_DRIVER_USB;
RTEMS_BSD_DRIVER_USB_MASS;
//SYSINIT_DRIVER_REFERENCE(umass, uhub);
//SYSINIT_DRIVER_REFERENCE(usbss, nexus);
//SYSINIT_DRIVER_REFERENCE(musbotg, usbss);

#elif defined(LIBBSP_M68K_GENMCF548X_BSP_H)

RTEMS_BSD_DRIVER_FEC;

#elif defined(LIBBSP_ARM_XILINX_ZYNQ_BSP_H)

#include <bsp/irq.h>

RTEMS_BSD_DRIVER_XILINX_ZYNQ_SLCR0;
RTEMS_BSD_DRIVER_XILINX_ZYNQ_CGEM0(ZYNQ_IRQ_ETHERNET_0);
RTEMS_BSD_DRIVER_E1000PHY;

#elif defined(LIBBSP_ARM_ATSAM_BSP_H)

RTEMS_BSD_DRIVER_USB;
RTEMS_BSD_DRIVER_USB_MASS;

#elif defined(LIBBSP_ARM_ALTERA_CYCLONE_V_BSP_H)

#include <bsp/socal/hps.h>
#include <bsp/irq.h>

RTEMS_BSD_DRIVER_DWC0((unsigned long) ALT_EMAC1_ADDR,
		      ALT_INT_INTERRUPT_EMAC1_IRQ);
RTEMS_BSD_DRIVER_MIPHY;
RTEMS_BSD_DRIVER_DWCOTG0((unsigned long) ALT_USB1_ADDR,
			 ALT_INT_INTERRUPT_USB1_IRQ);
RTEMS_BSD_DRIVER_DWC_MMC;
RTEMS_BSD_DRIVER_MMC;
RTEMS_BSD_DRIVER_USB;
RTEMS_BSD_DRIVER_USB_MASS;

#elif defined(LIBBSP_ARM_LPC24XX_BSP_H)

RTEMS_BSD_DEFINE_NEXUS_DEVICE(ohci, 0, 0, NULL);
SYSINIT_DRIVER_REFERENCE(usbus, ohci);
RTEMS_BSD_DRIVER_USB;
RTEMS_BSD_DRIVER_USB_MASS;

#elif defined(LIBBSP_I386_PC386_BSP_H)

RTEMS_BSD_DRIVER_PC_LEGACY;
RTEMS_BSD_DRIVER_PCI_LEM;
RTEMS_BSD_DRIVER_PCI_IGB;
RTEMS_BSD_DRIVER_PCI_EM;
RTEMS_BSD_DRIVER_PCI_RE;
RTEMS_BSD_DRIVER_REPHY;

#elif defined(LIBBSP_POWERPC_QORIQ_BSP_H)

#if !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT)

#include <bsp/irq.h>

RTEMS_BSD_DRIVER_TSEC(0xffeb0000,
		      QORIQ_IRQ_ETSEC_TX_1,
		      QORIQ_IRQ_ETSEC_RX_1,
		      QORIQ_IRQ_ETSEC_ER_1);

#endif /* !QORIQ_CHIP_IS_T_VARIANT(QORIQ_CHIP_VARIANT) */

#endif

#endif
