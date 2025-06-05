#include <linux/stddef.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <linux/btf.h>
#include "filter.h"

#define BTF_INFO_ENC(kind, kind_flag, vlen) \
	((!!(kind_flag) << 31) | ((kind) << 24) | ((vlen) & BTF_MAX_VLEN))
#define BTF_TYPE_ENC(name, info, size_or_type) (name), (info), (size_or_type)
#define BTF_INT_ENC(encoding, bits_offset, nr_bits) \
	((encoding) << 24 | (bits_offset) << 16 | (nr_bits))
#define BTF_TYPE_INT_ENC(name, encoding, bits_offset, bits, sz) \
	BTF_TYPE_ENC(name, BTF_INFO_ENC(BTF_KIND_INT, 0, 0), sz), \
	BTF_INT_ENC(encoding, bits_offset, bits)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
struct args {
	__u64 log_buf;
	__u32 log_size;
	int max_entries;
	int map_fd;
	int prog_fd;
	int btf_fd;
};


static inline __u64 ptr_to_u64(const void *ptr)
{
	return (__u64) (unsigned long) ptr;
}


static int btf_load(void)
{
	struct btf_blob {
		struct btf_header btf_hdr;
		__u32 types[8];
		__u32 str;
	} raw_btf = {
		.btf_hdr = {
			.magic = BTF_MAGIC,
			.version = BTF_VERSION,
			.hdr_len = sizeof(struct btf_header),
			.type_len = sizeof(raw_btf.types),
			.str_off = offsetof(struct btf_blob, str) - offsetof(struct btf_blob, types),
			.str_len = sizeof(raw_btf.str),
		},
		.types = {
			/* long */
			BTF_TYPE_INT_ENC(0, BTF_INT_SIGNED, 0, 64, 8),  /* [1] */
			/* unsigned long */
			BTF_TYPE_INT_ENC(0, 0, 0, 64, 8),  /* [2] */
		},
	};
	static union bpf_attr btf_load_attr = {
		.btf_size = sizeof(raw_btf),
	};

	btf_load_attr.btf = (long)&raw_btf;
	return bpf_sys_bpf(BPF_BTF_LOAD, &btf_load_attr, sizeof(btf_load_attr));
}


SEC("syscall")
int bpf_prog_trigger_syscall_prog(struct args *ctx) 
{
	bpf_printk("bpf_prog_trigger_syscall_prog: \n");
	static char license[] = "GPL";
	// BPF byte code to lookup a map 
	/* store (size, dst_reg, offset, imm) */
	/* mov (dst = src) */
	/* ALU (OPCODE, DST_REG, imm) */
	/* LD MAP FD to dst*/
	/* raw instruction */
	/* return value */
	/* return */
	bpf_printk("bpf_prog_trigger_syscall_prog: update invalid return value\n");
	static struct bpf_insn insns[] = {
		BPF_ST_MEM(BPF_DW, BPF_REG_10, -(1024 * 16), 0),
		BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
		BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -8),
		BPF_LD_MAP_FD(BPF_REG_1, 0),
		BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),
		BPF_MOV64_IMM(BPF_REG_0, 0),
		BPF_EXIT_INSN(),
	};

	static union bpf_attr map_create_attr = {
		.map_type = BPF_MAP_TYPE_HASH,
		.key_size = 8,
		.value_size = 8,
		.btf_key_type_id = 1,
		.btf_value_type_id = 2,
	};

	static union bpf_attr map_update_attr = { .map_fd = 1, };
	static __u64 key = 12;
	static __u64 value = 34;
	static union bpf_attr prog_load_attr = {
		.prog_type = BPF_PROG_TYPE_TRACEPOINT,
		.insn_cnt = ARRAY_SIZE(insns),
	};
	int ret;

	bpf_printk("bpf_prog_trigger_syscall_prog: btf_load()\n");
	ret = btf_load();
	if (ret <= 0) {
		bpf_printk("bpf_prog_trigger_syscall_prog: btf_load() failed\n");
		return ret;
	}

	ctx->btf_fd = ret;
	map_create_attr.max_entries = ctx->max_entries;
	map_create_attr.btf_fd = ret;

	prog_load_attr.license = ptr_to_u64(license);
	prog_load_attr.insns = ptr_to_u64(insns);
	prog_load_attr.log_buf = ctx->log_buf;
	prog_load_attr.log_size = ctx->log_size;
	prog_load_attr.log_level = 1;

	bpf_printk("bpf_prog_trigger_syscall_prog: BPF_MAP_CREATE\n");
	ret = bpf_sys_bpf(BPF_MAP_CREATE, &map_create_attr, sizeof(map_create_attr));
	if (ret <= 0) {
		bpf_printk("bpf_prog_trigger_syscall_prog: BPF_MAP_CREATE failed\n");
		return ret;
	}
	ctx->map_fd = ret;
	insns[3].imm = ret;

	
	map_update_attr.map_fd = ret;
	map_update_attr.key = ptr_to_u64(&key);
	map_update_attr.value = ptr_to_u64(&value);
	bpf_printk("bpf_prog_trigger_syscall_prog: BPF_MAP_UPDATE_ELEM\n");
	ret = bpf_sys_bpf(BPF_MAP_UPDATE_ELEM, &map_update_attr, sizeof(map_update_attr));
	if (ret < 0) {
		bpf_printk("bpf_prog_trigger_syscall_prog: BPF_MAP_UPDATE_ELEM failed\n");
		return ret;
	}

	bpf_printk("bpf_prog_trigger_syscall_prog: BPF_PROG_LOAD\n");
	ret = bpf_sys_bpf(BPF_PROG_LOAD, &prog_load_attr, sizeof(prog_load_attr));
	if (ret <= 0) {
		bpf_printk("bpf_prog_trigger_syscall_prog: BPF_PROG_LOAD failed\n");
		bpf_printk("printing log buf\n: %s", prog_load_attr.log_buf);
		return ret;
	}
	ctx->prog_fd = ret;


	bpf_printk("bpf_prog_trigger_syscall_prog: reached the end\n");
    return 0;

}

char LISENSE[] SEC("license") = "Dual BSD/GPL";

