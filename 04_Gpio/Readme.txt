This is an example of creation a GPIO linux devive driver in Raspberry PI5.

This example creates a kernel module ldd_gpio.c to control the GPIO21 of the RbPi5.
The kernel creates device file and class in order to control the GPIO from user space (usr_toggle_gpio.c).

Steps to test it.


1.- Compile kernel module ldd_gpio.c and app code using local makefile -> "make".
2.- Load Kernel module ldd_gpio.ko -> "sudo insmod ldd_gpio.ko"
3.- run the app code from user space -> sudo ./usr_toggle_gpio
4.- The GPIO21 shall start toggling.
