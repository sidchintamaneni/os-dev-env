
all:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
	clang -g -O2 -Wall -target bpf -I/linux/usr/include/ -I/linux/tools/lib/ -c trivial.bpf.c -o trivial.bpf.o
	bpftool gen skeleton -L trivial.bpf.o name trivial > loader.h
	/linux/scripts/hornet/extract-skel.sh loader.h trivial.bin
	clang -g -O2 -Wall -I/linux/tools/lib/ -I$/linux/usr/include -L/linux/tools/lib/bpf/ loader.c -lbpf -o loader
	/linux/scripts/hornet/sign-ebpf sha256 /linux/certs/signing_key.pem /linux/certs/signing_key.x509 trivial.bin loader signed_loader
	chmod +x signed_loader
