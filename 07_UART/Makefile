obj-m += ldd_uart.o
KDIR = /lib/modules/$(shell uname -r)/build

.PHONY: all uart0_driver 

all: uart0_driver 

uart0_driver:
	make -C $(KDIR) M=$(PWD) modules

dto: uart0-rb5-overlay.dts
	dtc -@ -I dts -O dtb -o uart0-rb5.dtbo uart0-rb5-overlay.dts 

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean