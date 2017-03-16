/**
 * @file
 *
 * @ingroup rtems_bsd_rtems
 *
 * @brief TODO.
 */

/*
 * Copyright (c) 2013, 2016 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
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

#include <machine/rtems-bsd-kernel-space.h>
#include <machine/rtems-bsd-thread.h>

#include <sys/types.h>
#include <sys/kernel.h>
#include <rtems/bsd/sys/lock.h>
#include <sys/mutex.h>

#undef printf
#define RTEMS_BSD_PROGRAM_NO_STRDUP_WRAP
#include <machine/rtems-bsd-program.h>

struct rtems_bsd_program_control *
rtems_bsd_program_get_control_or_null(void)
{
	struct thread *td;

	td = rtems_bsd_get_curthread_or_null();

	if (td != NULL) {
		return (td->td_prog_ctrl);
	} else {
		return (NULL);
	}
}

int
rtems_bsd_program_set_control(struct rtems_bsd_program_control *prog_ctrl)
{
	struct thread *td;
	int error;
//printf("rtems_bsd_program_set_control func\n");
	td = rtems_bsd_get_curthread_or_null();

	if (td != NULL) {
		if (td->td_prog_ctrl == NULL) {
			error = 0;
		} else {
			error = EBUSY;
		}

		td->td_prog_ctrl = prog_ctrl;
	} else {
		error = ENOMEM;
	}

	return (error);
}

static struct mtx program_mtx;

MTX_SYSINIT(rtems_bsd_program, &program_mtx, "BSD program", MTX_DEF);

void
rtems_bsd_program_lock(void)
{
	mtx_lock(&program_mtx);
}

void
rtems_bsd_program_unlock(void)
{
	mtx_unlock(&program_mtx);
}
