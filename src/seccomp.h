/*
 * Logswan 2.1.0
 * Copyright (c) 2015-2019, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2019-10-26
 *
 * Logswan is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 */

#ifndef SECCOMP_H
#define SECCOMP_H

#include <stddef.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>

#define LOGSWAN_ALLOW_SYSCALL(syscall) \
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_##syscall, 0, 1), \
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

static struct sock_filter filter[] = {
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),

	LOGSWAN_ALLOW_SYSCALL(brk),
	LOGSWAN_ALLOW_SYSCALL(close),
	LOGSWAN_ALLOW_SYSCALL(dup),
	LOGSWAN_ALLOW_SYSCALL(exit_group),
	LOGSWAN_ALLOW_SYSCALL(fcntl),
	LOGSWAN_ALLOW_SYSCALL(fstat),
	LOGSWAN_ALLOW_SYSCALL(ioctl),
	LOGSWAN_ALLOW_SYSCALL(lseek),
#if defined(__NR_open)
	LOGSWAN_ALLOW_SYSCALL(open),
#endif
	LOGSWAN_ALLOW_SYSCALL(openat),
	LOGSWAN_ALLOW_SYSCALL(mmap),
	LOGSWAN_ALLOW_SYSCALL(munmap),
	LOGSWAN_ALLOW_SYSCALL(read),
	LOGSWAN_ALLOW_SYSCALL(write),
	LOGSWAN_ALLOW_SYSCALL(writev),

	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL)
};

struct sock_fprog logswan = {
	.len = sizeof(filter)/sizeof(filter[0]),
	.filter = filter
};

#endif /* SECCOMP_H */
