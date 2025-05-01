# BPF struct_ops

* Step1:
    - Build and load kernel module to create a test stuct ops.
* Step2:
    - Load a BPF program to replace the function pointers in
      test stuct ops

#### Steps1:

`make modules`
`insmod kmod_st_ops.ko`

- struct ops kernel modules are not supported by BPF yet
    - Added to TODO list for feature requirements
