obj-m += ldd_pwm0.o
KDIR = /lib/modules/$(shell uname -r)/build
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all pwm0_driver pwm0_app_sysfs pwm0_app_dev

all: pwm0_driver pwm0_app_sysfs pwm0_app_dev

pwm0_driver:
	make -C $(KDIR) M=$(PWD) modules

pwm0_app_sysfs: app_fade_pwm0_sysfs.c
	$(CC) $(CFLAGS) -o app_fade_pwm0_sysfs $<

pwm0_app_dev: app_fade_pwm0_devdrv.c
	$(CC) $(CFLAGS) -o app_fade_pwm0_devdrv $<

dto: pwm0-rb5-overlay.dts
	dtc -@ -I dts -O dtb -o pwm0-rb5.dtbo pwm0-rb5-overlay.dts

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f app_fade_pwm0_sysfs
	rm -f app_fade_pwm0_devdrv