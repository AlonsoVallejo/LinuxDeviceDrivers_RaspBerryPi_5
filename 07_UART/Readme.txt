This is an example of creation a serial port linux device driver in Raspberry PI5.

This example creates a kernel module to read the UART0 of the RaspBerry PI5. 

The kernel modules uses a device tree overlay (uart0-rb5-overlay.dts) to enable the UART0 ports TX(GPIO14) and RX(GPIO15). 

It was tested using the bluetooth device HC05 connected to the RaspBerry to send the data.

Steps to test this kernel module.

1.- Compile the kernel module and app code by using the local makefile: "make"
2.- Compile the device tree overlay by using local makefile: "make dto"
3.- Copy the device tree overlay .dtbo to linux overlays: "sudo cp uart0-rb5.dtbo /boot/firmware/overlays/"
4.- Edit the /boot/firmware/config.txt and add the "dtoverlay=uart0-rb5".
5.- Edit "RaspBerry Pi Configuration" to enable the serial port. StartLogo/Preferences/RaspBerry Pi Configuration/interfaces/Serial Port->Enable
6.- Reboot the RaspBerry.
7.- Open serial terminal connected to the serial port of the RaspBerry.
8.- load kernel module: "sudo insmod ldd_uart.ko" and it shall appear "Send data: " in external terminal.
9.- write abything in the serial terminal connected to the RaspBerry.
10.- Using command "dmesg" in RaspBerry, it shall appear the data received from serial port terminal.
11.- be happy