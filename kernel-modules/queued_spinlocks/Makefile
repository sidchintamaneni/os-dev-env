obj-m := qspinlock.o
qspinlock-objs :=  qspinlock_exp.o sqspinlock.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

