This is an example of creation a SPI linux device driver in Raspberry PI5.

This example creates a kernel module to read the SPI-0 of the RaspBerry PI5. 
Using the very basic to read the id of the SPI RF module NRF24L01, why? because it was the only SPI module I have at this moment.

The kernel modules uses a device tree overlay (spi0-rb5-overlay.dts) to enable the SPI0 ports of RaspBerry. 

CS0 of SPI0 is probably already occupied by spidev, so we also have to disable that in a separate fragment

Steps to test this kernel module.

1.- Compile the kernel module and app code by using the local makefile: "make"
2.- Compile the device tree overlay by using local makefile: "make dto"
3.- load the device tree overlay .dtbo to linux: "sudo dtovleray spi0-rb5.dtbo"
4.- verify that overlay was correct uploader: "dtoverlay -a | grep spi", it shall be marked " * spi0-rb5 "
5.- verify that spidev0.0 was dissable: "ls -l /dev/spi*". The spidev0.0 shall not appear.
6.- load kernel module: "sudo insmod ldd_pwm0.ko", it shall create a file in "/sys/bus/spi/drivers/spi0_nrf24_drvd"
7.- be happy