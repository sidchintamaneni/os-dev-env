FROM ubuntu:24.04 AS linux-builder

ENV LINUX=/linux 

# adding clang
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get upgrade -y

RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install --fix-missing -y git build-essential gcc g++ fakeroot libncurses5-dev libssl-dev ccache dwarves libelf-dev \
 cmake mold \
 libdw-dev libdwarf-dev \
 bpfcc-tools libbpfcc-dev libbpfcc \
 linux-headers-generic \
 libtinfo-dev \
 libstdc++-11-dev libstdc++-12-dev \
 bc \
 flex bison \
 rsync \
 libcap-dev libdisasm-dev binutils-dev unzip \
 pkg-config lsb-release wget software-properties-common gnupg zlib1g python3-docutils

# clang and llvm
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y clang llvm lld

# Qemu
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y qemu-kvm libvirt-clients libvirt-daemon-system bridge-utils virtinst libvirt-daemon xterm attr busybox openssh-server

# Essentials
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y iputils-ping kmod curl autoconf gdb

# Rust
RUN curl https://sh.rustup.rs -sSf | bash -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"
RUN cargo install cross

# compression
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y zstd
