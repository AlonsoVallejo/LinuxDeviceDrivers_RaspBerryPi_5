PWM Linux Device Driver for Raspberry Pi 5: Comprehensive Guide

This project demonstrates how to create and use a kernel module to control PWM0 via GPIO12 on the Raspberry Pi 5. 
It includes kernel module development, device tree overlay setup, and user-space applications to manipulate PWM functionality.

---

Overview
1. Objective: Control PWM0 using GPIO12 on the Raspberry Pi 5.
2. Key Components:
   - Kernel module: `ldd_pwm0.c` initializes PWM0 with a defined period and duty cycle. It creates a device platform file to adjust PWM0 from user-space applications.
   - Device tree overlay: `pwm0-rb5-overlay.dts` enables PWM0 via GPIO12.
   - User-space applications:
     - `app_fade_pwm0_devdrv.c`: Fades PWM0 by setting duty cycle values (0–100).
     - `app_fade_pwm0_sysyfs.c`: Controls PWM0 parameters through sysfs. Requires load DTO before run the app. 
   - Shell script: `testSyspwm.sh` (adjust based on created pwmchip).

---

Steps to Implement and Test

1. Compile Required Files
   - Compile the kernel module and application code using the local Makefile:
     ```
     make
     ```
   - Compile the device tree overlay:
     ```
     make dto
     ```

2. Load the Device Tree Overlay
   - Add the compiled device tree overlay (`pwm0-rb5.dtbo`) to the Raspberry Pi's Linux kernel:
     ```
     sudo dtoverlay pwm0-rb5.dtbo
     ```
   - Verify the overlay was correctly loaded:
     ```
     dtoverlay -a | grep pwm
     ```
     - It should appear as: `* pwm0-rb5`.

3. Load the Kernel Module
   - Load the kernel module (`ldd_pwm0.ko`) to initialize PWM0:
     ```
     sudo insmod ldd_pwm0.ko
     ```
   - After loading, a platform file should be created in `/proc/device-tree/pwm0_pin12_led`.

4. Run User-Space Application
   - Execute the application (`app_fade_pwm0_devdrv`) to control and fade the PWM duty cycle:
     ```
     sudo ./app_fade_pwm0_devdrv
     ```
   - GPIO12 will produce a fading PWM signal. Test the signal using an LED or oscilloscope.

5. Alternative PWM Control
   - Use the sysfs-based application (`app_fade_pwm0_sysyfs.c`) to control PWM parameters from user space.

6. Adjust Additional Files
   - Update `testSyspwm.sh` and `app_fade_pwm0_sysyfs.c` based on the pwmchip generated after loading the device tree overlay.

---

Notes
- Hardware Requirements: Ensure GPIO12 is connected appropriately to an LED or oscilloscope for testing the PWM signal.
- Testing: Use `app_fade_pwm0_devdrv` for dynamic fading tests and `app_fade_pwm0_sysyfs.c` for direct parameter manipulation.
- Safety: Double-check connections and module functionality to prevent hardware issues.
