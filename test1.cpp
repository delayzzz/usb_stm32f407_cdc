#include <iostream>
#include <string.h> 
#include <time.h>
#include "libusb-1.0/libusb.h"

#define USB_VID 0x0483 //USB设备的产商ID
#define USB_PID 0x5740 //USB设备的产品ID
#define EPIN_ADDR 0X81
#define EPOUT_ADDR 0X01

libusb_device_handle *handle_stm32f4 = NULL;
clock_t start,end;

int rx_cnt = 0;

int main()
{
    libusb_device **devs;
    int r, ret, tx;
    
    unsigned char buf[100];
    uint16_t count = 0;
    unsigned char Txbuf[] = "linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!";

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

    while (rx_cnt<10)
    {
        count ++;
        
        tx = libusb_bulk_transfer(handle_stm32f4, EPOUT_ADDR, Txbuf,
                                    strlen("linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!linux test!"),
                                    NULL, 1000);

        if(count >= 2000)
        {
            end = clock();
	        double endtime=(double)(end-start)/(double)CLOCKS_PER_SEC;
            std::cout << "Total time:" << endtime << "s" << std::endl;

            r = libusb_bulk_transfer(handle_stm32f4, EPIN_ADDR, buf, 1, NULL, 2000);
            if (r < 0)
            {
                std::cout << "\n"
                        << libusb_error_name(r) << std::endl;
            }
            else
            {
                std::cout << "receive:" << (unsigned)buf[0] << "*10kb bytes" << "\n"
                        << std::endl;
            }
            count = 0;
            rx_cnt ++;
            
            start = clock();
        }
    }

    libusb_exit(NULL);
    std::cout << "libusb to the end.." << std::endl;
    return 0;
}