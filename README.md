***This repository is cloned and modified from rosalab***

Updating the folder structure of the repository (branch -> main) based 
on the recent commit ignoring submodules.

```
├── bpf-programs-catalog
│   ├── LSM-BPF
│   │   └── README.md
│   ├── networking
│   │   ├── congestion-control
│   │   │   └── README.md
│   │   ├── tc
│   │   │   └── README.md
│   │   └── xdp
│   │       └── README.md
│   ├── README.md
│   ├── research
│   │   ├── deadlock-research
│   │   │   └── README.md
│   │   ├── function-patching-related
│   │   │   └── README.md
│   │   ├── generic-features
│   │   │   └── README.md
│   │   └── stackoverflow-research
│   │       └── README.md
│   └── sched-ext
│       └── README.md
├── docker
│   └── docker-linux-builder
│       ├── Dockerfile
│       └── Makefile
├── kernel-dev
│   └── patches
│       └── README.md
├── kernel-modules
│   └── char-dev
│       ├── driver
│       │   ├── chardev.c
│       │   ├── chardev.h
│       │   └── Makefile
│       └── ulib
│           ├── init.c
│           ├── Makefile
│           └── syscall.h
├── linux-config
├── Makefile
├── q-script
│   └── yifei-q
├── README.md
└── security-research
    └── dirty-pagetables
        └── README.md

```
#### How to use the repository

I will update the README after I finished working BPF program
catalog.

