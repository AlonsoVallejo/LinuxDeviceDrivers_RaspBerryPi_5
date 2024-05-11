This is an example of creation a PWM linux device driver in Raspberry PI5.

This example creates a kernel module to control the PWM0 using GPIO12 of the RaspBerry PI5.
The kernel module "ldd_pwm0.c" inits the PWM0 with define period and dutty cycle. In the kernel
module is being created a device platform file to control the PWM0 dutty cycle from user space app
app_fade_pwm0_devdrv.c.
The kernel modules uses a device tree overlay (pwm0-rb5-overlay.dts) to enable the PWM0 in the raspberry pi 5 by using GPIO12.

It was created another app code (app_fade_pwm0_sysyfs.c) to control the PWM0 parameters from user space using the sysfs files.

The app code app_fade_pwm0_devdrv.c fades the PWM0 sending values from 0-100 referencing to dutty cycle.

app_fade_pwm0_sysyfs.c and testSyspwm.sh needs to be ajusted according the pwmchip that was created after loading the device tree overlay.

Steps to test this kernel module and app code.

1.- Compile the kernel module and app code by using the local makefile: "make"
2.- Compile the device tree overlay by using local makefile: "make dto"
3.- load the device tree overlay .dtbo to linux: "sudo dtovleray pwm0-rb5.dtbo"
4.- verify that overlay was correct uploader: "dtoverlay -a | grep pwm", it shall be marked " * pwm0-rb5 "
5.- load kernel module: "sudo insmod ldd_pwm0.ko", it shall create a file in "/proc/device-tree/pwm0_pin12_led"
6.- run app code app_fade_pwm0_devdrv : "sudo ./app_fade_pwm0_devdrv"
7.- the GPIO12 shall start fading a PWM, test it using LED o oscilloscope.
