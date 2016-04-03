#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>

#include "vsd_device.h"
#include "../vsd_driver/vsd_ioctl.h"

static int vsd_dev = NULL;

int vsd_init()
{
    vsd_dev = open("/dev/vsd", O_RDWR);
    if (vsd_dev == -1)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int vsd_deinit()
{
    close(vsd_dev);
    return EXIT_SUCCESS;
}

int vsd_get_size(size_t *out_size)
{
    vsd_ioctl_get_size_arg_t arg;
    int ret = ioctl(vsd_dev, VSD_IOCTL_GET_SIZE, &arg);
    if (ret == -1)
        return EXIT_FAILURE;
    *out_size = arg.size;

    return EXIT_SUCCESS;
}

int vsd_set_size(size_t size)
{
    if (size < 0)
        return EXIT_FAILURE;

    vsd_ioctl_set_size_arg_t arg;
    arg.size = size;
    int ret = ioctl(vsd_dev, VSD_IOCTL_SET_SIZE, &arg);
    if (ret == -1)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

ssize_t vsd_read(char* dst, off_t offset, size_t size)
{
    int ret = lseek(vsd_dev, offset, SEEK_SET);
    if (ret == -1)
        return EXIT_FAILURE;
    return read(vsd_dev, dst, size);
}

ssize_t vsd_write(const char* src, off_t offset, size_t size)
{
    int ret = lseek(vsd_dev, offset, SEEK_SET);
    if (ret == -1)
        return EXIT_FAILURE;
    return write(vsd_dev, src, size);;
}

void* vsd_mmap(size_t offset)
{
    size_t dev_size = 0;
    vsd_get_size(&dev_size);

    return mmap(NULL, dev_size - offset, PROT_READ | PROT_WRITE,
                MAP_SHARED, vsd_dev, offset);
}

int vsd_munmap(void* addr, size_t offset)
{
    size_t dev_size = 0;
    vsd_get_size(&dev_size);

    return munmap(addr, dev_size - offset);
}
