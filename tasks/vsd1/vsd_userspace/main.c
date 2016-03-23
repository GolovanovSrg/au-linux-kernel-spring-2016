#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

#include "../vsd_driver/vsd_ioctl.h"

static void print_usage(const char* program)
{
  std::cout << "Usage: " << program <<
    " [ size_get | size_set SIZE_IN_BYTES ]" << std::endl;
}

static int get_size(int dev)
{
    vsd_ioctl_get_size_arg_t arg;
    int ret = ioctl(dev, VSD_IOCTL_GET_SIZE, &arg);
    if (ret)
    {
        std::cerr << "Error: ioctl return " << ret << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Device buffer size: " << arg.size << std::endl;
    return EXIT_SUCCESS;
}

static int set_size(int dev, long size)
{
    if (size < 0)
    {
        std::cerr << "Error: bad size " << size << std::endl;
        return EXIT_FAILURE;
    }

    vsd_ioctl_set_size_arg_t arg;
    arg.size = size;

    int ret = ioctl(dev, VSD_IOCTL_SET_SIZE, &arg);
    if (ret)
    {
        std::cerr << "Error: ioctl return " << ret << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Device buffer size: " << size << std::endl;
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    int dev = open("/dev/vsd", O_RDONLY);
    if (dev == -1)
    {
        std::cerr << "Error: /dev/vsd is not open" << std::endl;
        return EXIT_FAILURE;
    }

    int ret = EXIT_FAILURE;
    std::string cmd = argv[1];

    if (cmd == "size_get")
    {
        ret = get_size(dev);
    }
    if (cmd == "size_set" && argc == 3)
    {
        std::string size = argv[2];
        ret = set_size(dev, std::stol(size));
    }
    else
    {
        print_usage(argv[0]);
    }

    return ret;
}
