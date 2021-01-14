#include <iostream>
#include <string.h>
#include <time.h>
#include "libusb-1.0/libusb.h"

#define USB_VID 0x0483 //USB设备的产商ID
#define USB_PID 0x5740 //USB设备的产品ID
#define EPIN_ADDR 0X81
#define EPOUT_ADDR 0X01

#define FLOAT_NUM 10

libusb_device_handle *handle_stm32f4 = NULL;
clock_t start, end;

int rx_cnt = 0;

int main()
{
    libusb_device **devs;
    int r, ret, tx;

    unsigned char buf[4 * FLOAT_NUM + 1];
    uint16_t count = 0;
    uint8_t Txbuf[4 * FLOAT_NUM] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};

    std::cout << "libusb at the start..\n"
              << std::endl;
    if (libusb_init(NULL) < 0)
        return r;

    handle_stm32f4 = libusb_open_device_with_vid_pid(NULL, USB_VID, USB_PID);
    if (handle_stm32f4 == NULL)
    {
        perror("Cannot open device\n");
    }
    else
    {
        std::cout << "Device Opened\n"
                  << std::endl;
    }

    ret = libusb_kernel_driver_active(handle_stm32f4, 0);
    if (ret == 1)
    {
        libusb_detach_kernel_driver(handle_stm32f4, 0);
        std::cout << "driver error\n"
                  << std::endl;
    }

    while (rx_cnt < 10)
    {
        count++;

        tx = libusb_bulk_transfer(handle_stm32f4, EPOUT_ADDR, Txbuf,
                                  4 * FLOAT_NUM,
                                  NULL, 1000);

        if (count >= 5000)
        {
            end = clock();
            double endtime = (double)(end - start) / (double)CLOCKS_PER_SEC;
            std::cout << "Interval time:" << endtime << "s" << std::endl;

            r = libusb_bulk_transfer(handle_stm32f4, EPIN_ADDR, buf, 4 * FLOAT_NUM, NULL, 2000);
            if (r < 0)
            {
                std::cout << "\n"
                          << libusb_error_name(r) << std::endl;
            }
            else
            {
                // std::cout << "receive:" << (unsigned)buf[0] << "*10kb bytes" << "\n"
                //         << std::endl;
                for (int i = 0; i < 4 * FLOAT_NUM; i++)
                {
                    std::cout << (unsigned)buf[i] << std::endl;
                }
                std::cout << '/n' << std::endl;
            }
            count = 0;
            rx_cnt++;

            start = clock();
        }
    }

    libusb_exit(NULL);
    std::cout << "libusb to the end.." << std::endl;
    return 0;
}