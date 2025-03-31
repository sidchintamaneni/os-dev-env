BASE_PROJ ?= $(shell pwd)
LINUX ?= ${BASE_PROJ}/linux
BPF_LINUX ?= ${BASE_PROJ}/bpf
USER_ID ?= "$(shell id -u):$(shell id -g)"
SSH_PORT ?= "62227"
NET_PORT ?= "62228"
GDB_PORT ?= "62229"
.ALWAYS:

all: vmlinux 

docker: .ALWAYS
	docker buildx build --network=host --progress=plain -t sid-runtime-osdev .

qemu-run: 
	docker run --privileged --rm \
	--device=/dev/kvm:/dev/kvm \
	-v ${BASE_PROJ}:/os-dev-env -v ${LINUX}:/linux \
	-w /linux \
	-p 127.0.0.1:${SSH_PORT}:52222 \
	-p 127.0.0.1:${NET_PORT}:52223 \
	-p 127.0.0.1:${GDB_PORT}:1234 \
	-it sid-runtime-osdev:latest \
	/os-dev-env/q-script/yifei-q -s

qemu-run-blinux: 
	docker run --privileged --rm \
	--device=/dev/kvm:/dev/kvm \
	-v ${BASE_PROJ}:/os-dev-env -v ${BPF_LINUX}:/linux \
	-w /linux \
	-p 127.0.0.1:${SSH_PORT}:52222 \
	-p 127.0.0.1:${NET_PORT}:52223 \
	-p 127.0.0.1:${GDB_PORT}:1234 \
	-it sid-runtime-osdev:latest \
	/os-dev-env/q-script/yifei-q -s

# connect running qemu by ssh
qemu-ssh:
	ssh -o "UserKnownHostsFile=/dev/null" -o "StrictHostKeyChecking=no" -t root@127.0.0.1 -p ${SSH_PORT}

bpftool: 
	docker run --rm -u ${USER_ID} -v ${LINUX}:/linux -w /linux/tools/bpf/bpftool sid-runtime-osdev make -j`nproc` bpftool

libbpf: 
	docker run --rm -u ${USER_ID} -v ${LINUX}:/linux -w /linux/tools/lib/bpf sid-runtime-osdev make -j`nproc` 

libbpf-clean: 
	docker run --rm -u ${USER_ID} -v ${LINUX}:/linux -w /linux/tools/lib/bpf sid-runtime-osdev make -j`nproc` clean

bpftool-clean:
	docker run --rm -v ${LINUX}:/linux -w /linux/tools/bpf/bpftool sid-runtime-osdev make -j`nproc` clean 

bpftool-blinux: 
	docker run --rm -u ${USER_ID} -v ${BPF_LINUX}:/linux -w /linux/tools/bpf/bpftool sid-runtime-osdev make -j`nproc` bpftool

libbpf-blinux: 
	docker run --rm -u ${USER_ID} -v ${BPF_LINUX}:/linux -w /linux/tools/lib/bpf sid-runtime-osdev make -j`nproc` 

libbpf-clean-blinux: 
	docker run --rm -u ${USER_ID} -v ${BPF_LINUX}:/linux -w /linux/tools/lib/bpf sid-runtime-osdev make -j`nproc` clean

bpftool-clean-blinux:
	docker run --rm -u ${USER_ID} -v ${BPF_LINUX}:/linux -w /linux/tools/bpf/bpftool sid-runtime-osdev make -j`nproc` clean 

vmlinux: 
	docker run --rm -u ${USER_ID} -v ${LINUX}:/linux -w /linux sid-runtime-osdev  make -j`nproc` bzImage 

vmlinux-blinux: 
	docker run --rm -u ${USER_ID} -v ${BPF_LINUX}:/linux -w /linux sid-runtime-osdev  make -j`nproc` bzImage 

headers-install: 
	docker run --rm -u ${USER_ID} -v ${LINUX}:/linux -w /linux sid-runtime-osdev  make -j`nproc` headers_install 

modules-install: 
	docker run --rm -u ${USER_ID} -v ${LINUX}:/linux -w /linux sid-runtime-osdev  make -j`nproc` modules

headers-install-blinux: 
	docker run --rm -u ${USER_ID} -v ${BPF_LINUX}:/linux -w /linux sid-runtime-osdev  make -j`nproc` headers_install 

modules-install-blinux: 
	docker run --rm -u ${USER_ID} -v ${BPF_LINUX}:/linux -w /linux sid-runtime-osdev  make -j`nproc` modules

kernel:
	docker run --rm -u ${USER_ID} -v ${LINUX}:/linux -w /linux sid-runtime-osdev  make -j`nproc` 

linux-clean:
	docker run --rm -v ${LINUX}:/linux -w /linux sid-runtime-osdev make distclean

linux-clean-blinux:
	docker run --rm -v ${BPF_LINUX}:/linux -w /linux sid-runtime-osdev make distclean

# Targets for C BPF
bpf-samples:
	docker run --rm -u ${USER_ID} -v ${LINUX}:/linux -w /linux/samples/bpf sid-runtime-osdev make -j`nproc`

bpf-samples-clean:
	docker run --rm -v ${LINUX}:/linux -w /linux/samples/bpf sid-runtime-osdev make clean

# Target to enter docker container
enter-docker:
	docker run --rm -u ${USER_ID} -v ${BASE_PROJ}:/os-dev-env -w /os-dev-env -it sid-runtime-osdev /bin/bash

