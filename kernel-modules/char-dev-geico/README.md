# Char Device Driver 

A basic character device driver implementation that supports `read`, `write`,
`llseek`, and `ioctl` system calls.

- For each opened file descriptor an internal buffer is created. so technically
  if the process is terminated the internal buffers related to that process are
  cleaned up. 
- Internal buffers are thread-safe
- Internal buffer size is 1024 bytes.

## Supported System Calls

### read 
```
read(int fd, void* buf, int count)
``` 

Reads data from the device's internal buffer into user space.

- The driver maintains the previous read/write offset position internally
- If `count` exceeds the available buffer size, the return length will be
  `BUFFER_SIZE - current_offset`
  - Example: If current offset is 100 and count is 1024, the returned bytes
    will be 924

**returns:** Number of bytes read on success, negative value on failure

### write 
```
write(int fd, void* buf, int count) 
``` 

Writes data from user space to the device's internal buffer.

- Maintains the previous read/write offset position
- If `count` exceeds the buffer size, the write operation overwrites the buffer
  contents from the start
- **returns:** Number of bytes written on success, negative value on failure

### lseek 

```
lseek(int fd, off_t offset, int whence)
``` 

Adjusts the read/write offset position within the internal buffer.

- returns `-EINVAL` if the requested position is:
  - Beyond the internal buffer size
  - Less than 0
- For available `whence` options, refer to the man pages

**returns:** New position on success, negative value on failure

### ioctl 

``` 
ioctl(int fd, int args) 
``` 

Custom syscall implementation to enable the option clean up the internal buffer
upon read

**Args:**
- `CHARDEV_IOC_CLEAR_ON_READ`
- `CHARDEV_IOC_NO_CLEAR`

(See `tests/chardev_ioctl.h` for definitions)

**returns:** 0 on success, negative value on failure

## Building and Loading the Driver 

Tested on Linux kernel v6.3.0

```
cd drivers
make insmod chardev.ko
bash rmmod chardev
```

## Testing To run the test suite: 

```
cd tests 
make chmod +x run_tests.sh
make run_tests 
```

### Test Cases Overview
1. **fd.test.c**
   - File descriptor functionality tests

2. **ioctl1.test.c**
   - IOCTL command functionality tests
   - Verifies both supported arguments

3. **lseek1.test.c**
   - Offset modification between read/write operations
   - Variable buffer size tests

4. **read-write1.test.c**
   - Basic read and write syscall functionality

5. **read-write2-multithreading.test.c**
   - Read/write synchronization verification
   - Multithreaded operation tests

6. **read-write3.test.c**
   - Write syscall overwrite functionality tests
