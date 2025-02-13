

#### BPF Private Stacks

- While going through the private stacks [kernel
  patch](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=c1bc51f85cd6)
  I have noticed that for each BPF program a
  per-cpu private stack is initialized during JIT
  time. 

** What does this means? **
- This means that when a single BPF program is
  nested, the nested BPF program will reuse the
  same stack - resulting in some sort of
  corruption


#### Todo 

[x] Make sure my understanding is right by writing
a test program
    - Per prog stack is fine, because of per prog active
    variable prevents the nested program from executing.
    
