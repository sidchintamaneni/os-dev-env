
struct nop {
	int invalid;
};

#define CHARDEV_IOCTL_BASE 'C'
#define CHARDEV_IOWR(nr, type)  _IOWR(CHARDEV_IOCTL_BASE, nr, type)

#define CHARDEV_IOCTL_INIT CHARDEV_IOWR(0x01, struct nop)
