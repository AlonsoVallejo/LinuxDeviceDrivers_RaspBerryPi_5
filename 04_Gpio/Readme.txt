# GPIO Linux Device Driver for Raspberry Pi 5: Quick Guide

This example demonstrates the creation of a GPIO Linux device driver for Raspberry Pi 5. It includes a kernel module (`ldd_gpio.c`) to control GPIO21 and a user-space application (`usr_toggle_gpio.c`) to interact with the GPIO.

---

## Overview
- **Kernel module**: `ldd_gpio.c` controls GPIO21.
- **User-space application**: `usr_toggle_gpio.c` interacts with the GPIO via the device file created by the kernel module.
- The kernel module creates a device file and class, enabling control of the GPIO from user space.

---

## Steps to Test

1. **Compile Files**
   - Use the local Makefile to compile the kernel module (`ldd_gpio.c`) and user-space application:
     ```
     make
     ```

2. **Load the Kernel Module**
   - Load the compiled kernel module (`ldd_gpio.ko`) into the Linux kernel:
     ```
     sudo insmod ldd_gpio.ko
     ```

3. **Run the User-Space Application**
   - Execute the application from user space to interact with the GPIO:
     ```
     sudo ./usr_toggle_gpio
     ```

4. **Verify GPIO Functionality**
   - GPIO21 should start toggling. Test this using an LED or appropriate hardware.

---