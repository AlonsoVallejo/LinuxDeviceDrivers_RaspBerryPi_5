obj-m += ldd_gpio.o
KDIR = /lib/modules/$(shell uname -r)/build
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all gpio_driver toggle_app

all: gpio_driver toggle_app

gpio_driver:
	make -C $(KDIR) M=$(PWD) modules

toggle_app: usr_toggle_gpio.c
	$(CC) $(CFLAGS) -o usr_toggle_gpio $<

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f usr_toggle_gpio