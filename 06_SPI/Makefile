obj-m += ldd_spi.o
KDIR = /lib/modules/$(shell uname -r)/build
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all spi_driver 

all: spi_driver 

spi_driver:
	make -C $(KDIR) M=$(PWD) modules

dto: spi0-rb5-overlay.dts
	dtc -@ -I dts -O dtb -o spi0-rb5.dtbo spi0-rb5-overlay.dts 

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean