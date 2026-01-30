/* SPDX-License-Identifier: GPL-2.0-only */

/* vmlinux.h subset for eBPF programs in Systemd
 *
 * Some eBPF programs in Systemd requires a vmlinux.h. Usually, building such a
 * vmlinux.h requires building an entire kernel with the appropriate
 * configurations enabled, extracting BTF from it, and then extracting a
 * vmlinux.h. It greatly complicates the build system and bootstrap process for
 * Nixpkgs. So, here's a vmlinux.h subset maintained for Systemd, as is done in
 * practice by many other projects that use eBPF.
 *
 * If more eBPF programs are added to Systemd, this file may need to be
 * expanded to include extra types or fields. It is as of yet unclear how
 * Systemd plans on dealing with the types changing in an incompatible manner.
 *
 * This header file is in a large part derived from kernel sources and thus not
 * available under the MIT license. This is in effect a "patch" to Systemd
 * delivered via the build system, and the note regarding patches in the top
 * level README.md "License" section applies. */

#pragma once

/* We get as many types as possible from the ABI-stable C standard and UAPI
 * headers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <linux/types.h>
#include <linux/bpf.h>
#include <sys/types.h>

/* This UAPI header provides struct pt_regs or struct user_pt_regs for kprobe
 * support.
 *
 * For the libbpf BPF_KPROBE macro to work, this vmlinux.h file *must not*
 * define __VMLINUX_H__. This is because <bpf/bpf_tracing.h> checks for this
 * macro to determine whether we have included this UAPI header, or a
 * kernel-BTF-generated vmlinux.h. We use the UAPI header for convenience.
 *
 * See https://github.com/libbpf/libbpf/blob/v1.6.2/src/bpf_tracing.h#L85 */
#include <asm/ptrace.h>

/* The UAPI headers only have double underscore variants for these. The
 * non-double-underscore aliases are for in-kernel use. eBPF is in-kernel. */
typedef __u32 u32;
typedef __u64 u64;

/* The following are kernel-internal types. These are probably not stable ABI.
 * For structures, only used fields are included, which reduces the size of
 * this file. */

/* include/linux/types.h as of 6.19-rc1 */
/* This shouldn't really matter. The only use of this type as of writing is in
 * some BPF-LSM functions, which casts a mode parameter from u64 (they're all
 * u64 when passed to eBPF) to umode_t, and then leaves it unused. */
typedef unsigned short umode_t;

/* include/linux/ns/ns_common_types.h as of 6.19-rc1 */
struct ns_common {
        unsigned int inum;
} __attribute__((preserve_access_index));

/* include/linux/workqueue_types.h as of 6.19-rc1 */
struct work_struct {
} __attribute__((preserve_access_index));

/* include/linux/user_namespace.h as of 6.19-rc1 */
struct user_namespace {
        struct user_namespace *parent;
        struct ns_common ns;
        struct work_struct work;
} __attribute__((preserve_access_index));

/* include/linux/cred.h as of 6.19-rc1 */
struct cred {
        struct user_namespace *user_ns;
} __attribute__((preserve_access_index));

/* include/linux/sched.h as of 6.19-rc1 */
struct task_struct {
        const struct cred *cred;
} __attribute__((preserve_access_index));

/* include/linux/dcache.h as of 6.19-rc1 */
struct dentry {
        struct inode *d_inode;
} __attribute__((preserve_access_index));

/* include/linux/mount.h as of 6.19-rc1 */
struct vfsmount {
} __attribute__((preserve_access_index));

/* include/linux/path.h as of 6.19-rc1 */
struct path {
        struct vfsmount *mnt;
        struct dentry *dentry;
} __attribute__((preserve_access_index));

/* fs/mount.h as of 6.19-rc1 */
struct mnt_namespace {
        struct user_namespace *user_ns;
} __attribute__((preserve_access_index));

/* fs/mount.h as of 6.19-rc1 */
struct mount {
        struct vfsmount mnt;
        struct mnt_namespace *mnt_ns;
        int mnt_id;
} __attribute__((preserve_access_index));
