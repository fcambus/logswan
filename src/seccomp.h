/*
 * Logswan 2.1.5
 * Copyright (c) 2015-2020, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2020-06-23
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

#if defined(__x86_64__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_X86_64
#elif defined(__aarch64__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_AARCH64
#else
#error "Seccomp is only supported on amd64 and aarch64 architectures."
#endif

#define LOGSWAN_SYSCALL_ALLOW(syscall) \
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_##syscall, 0, 1), \
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

static struct sock_filter filter[] = {
	/* Validate architecture */
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, arch)),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SECCOMP_AUDIT_ARCH, 1, 0),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),

	/* Load syscall */
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),

	LOGSWAN_SYSCALL_ALLOW(brk),
	LOGSWAN_SYSCALL_ALLOW(close),
	LOGSWAN_SYSCALL_ALLOW(dup),
	LOGSWAN_SYSCALL_ALLOW(exit_group),
	LOGSWAN_SYSCALL_ALLOW(fcntl),
	LOGSWAN_SYSCALL_ALLOW(fstat),
	LOGSWAN_SYSCALL_ALLOW(ioctl),
	LOGSWAN_SYSCALL_ALLOW(lseek),
#if defined(__NR_open)
	LOGSWAN_SYSCALL_ALLOW(open),
#endif
	LOGSWAN_SYSCALL_ALLOW(openat),
	LOGSWAN_SYSCALL_ALLOW(mmap),
	LOGSWAN_SYSCALL_ALLOW(munmap),
	LOGSWAN_SYSCALL_ALLOW(read),
	LOGSWAN_SYSCALL_ALLOW(write),
	LOGSWAN_SYSCALL_ALLOW(writev),

	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL)
};

struct sock_fprog logswan = {
	.len = sizeof(filter)/sizeof(filter[0]),
	.filter = filter
};

#endif /* SECCOMP_H */
