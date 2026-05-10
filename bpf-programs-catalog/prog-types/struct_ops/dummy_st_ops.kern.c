#include <linux/types.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char _license[] SEC("license") = "GPL";

SEC("struct_ops/test_1")
int BPF_PROG(test_1, struct bpf_dummy_ops_state *state)
{
	int ret;

	/* Check that 'state' nullable status is detected correctly.
	 * If 'state' argument would be assumed non-null by verifier
	 * the code below would be deleted as dead (which it shouldn't).
	 * Hide it from the compiler behind 'asm' block to avoid
	 * unnecessary optimizations.
	 */
	asm volatile (
		"if %[state] != 0 goto +2;"
		"r0 = 0xf2f3f4f5;"
		"exit;"
	::[state]"p"(state));

	ret = state->val;
	state->val = 0x5a;
	return ret;
}

SEC(".struct_ops")
struct bpf_dummy_ops dummy_1 = {
	.test_1 = (void *)test_1,
};
