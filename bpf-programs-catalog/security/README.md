* Running hornet
	- Generating `vmlinux.h`
		bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
	- Compiling the bpf program
		clang -g -O2 -Wall -target bpf -I/linux/usr/include/ -I/linux/tools/lib/ -c trivial.bpf.c -o trivial.bpf.o
	- Generating a light skeleton
		bpftool gen skeleton -L trivial.bpf.o name trivial > loader.h
	- Extracting instructions and data from light skel
		/linux/scripts/hornet/extract-skel.sh loader.h trivial.bin
	- Compile the userspace/ loader program
		clang -g -O2 -Wall -I/linux/tools/lib/ -I$/linux/usr/include -L/linux/tools/lib/bpf/ loader.c -lbpf -o loader
	- Compile the `sign-ebpf.c` in `scripts/hornet`
		gcc sign-ebpf.c -o sign-ebpf -lcrypto
	- Realized that need CONFIG_MODULE_SIG_{KEY} to have certs/*.pem file for signing
		../../linux/scripts/hornet/sign-ebpf sha256 /linux/certs/signing_key.pem /linux/certs/signing_key.x509 trivial.bin loader signed_loader

