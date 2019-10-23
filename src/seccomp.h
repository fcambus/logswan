/*
 * Logswan 2.1.0
 * Copyright (c) 2015-2019, Frederic Cambus
 * https://www.logswan.org
 *
 * Created:      2015-05-31
 * Last Updated: 2019-10-23
 *
 * Logswan is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 */

#include <stddef.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>

static struct sock_filter filter[] = {
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),

	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_brk, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_close, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_dup, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_exit_group, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_fcntl, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_fstat, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_ioctl, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_lseek, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_open, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_openat, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_mmap, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_munmap, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_read, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_write, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SYS_writev, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW),

	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL)
};

struct sock_fprog logswan = {
	.len = sizeof(filter)/sizeof(filter[0]),
	.filter = filter
};
