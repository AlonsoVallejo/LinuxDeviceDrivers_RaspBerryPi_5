#include <linux/module.h>
#include <linux/init.h>
#include <linux/serdev.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vallejo");
MODULE_DESCRIPTION("serial port (UART0) driver RaspBerry Pi 5");

/* Serial driver functions */
static int serial_uart0_probe(struct serdev_device *serdev);
static void serial_uart0_remove(struct serdev_device *serdev);
static int uart0_drv_recv(struct serdev_device *serdev, const unsigned char *buffer, size_t size);

/* setup device tree/open firmware table for compatible matching 
 * to load this module when device tree overlay is initailized */
static struct of_device_id uart0_drv_ids[] = {
	{
		.compatible = "serial_uart0_drv",
	}, 
	{ }
};
MODULE_DEVICE_TABLE(of, uart0_drv_ids);

static const struct serdev_device_ops uart0_drv_ops = {
	.receive_buf = uart0_drv_recv,
};

/* setup Serial platform (probe and remove) and match table */
static struct serdev_device_driver uart0_drv_driver = {
	.probe = serial_uart0_probe,
	.remove = serial_uart0_remove,
	.driver = {
		.name = "serial_uart0_drv",
		.of_match_table = uart0_drv_ids,
	},
};

/* This function is called when data is received from serial port */
static int uart0_drv_recv(struct serdev_device *serdev, const unsigned char *buffer, size_t size) {
	int ret = 0;
    ret = serdev_device_write_buf(serdev, buffer, size);
	pr_info("ldd_uart: Data recieved from Serial port (UART0) is: %c\n", buffer[0]);
	return ret;
}

/**
 * @brief This function is called on loading the driver 
 */
static int serial_uart0_probe(struct serdev_device *serdev) {
	int status;
	
	serdev_device_set_client_ops(serdev, &uart0_drv_ops);
	status = serdev_device_open(serdev);
	if(status) {
		pr_err("ldd_uart: Error opening serial port!\n");
		return -status;
	}

	/* Set UART0 parameters */
	serdev_device_set_baudrate(serdev, 9600);
	serdev_device_set_flow_control(serdev, false);
	serdev_device_set_parity(serdev, SERDEV_PARITY_NONE);

	status = serdev_device_write_buf(serdev, "Send data: ", sizeof("Send data: "));

	pr_info("ldd_uart: Installed \n");
	return 0;
}

/**
 * @brief This function is called on unloading the driver 
 */
static void serial_uart0_remove(struct serdev_device *serdev) {
	pr_info("ldd_uart: Removed \n");
	serdev_device_close(serdev);
}

static int __init ldd_uart0_driver_init(void) {
	if(serdev_device_driver_register(&uart0_drv_driver)) {
		pr_err("ldd_uart: - Error! Could not load driver\n");
		return -1;
	}
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ldd_uart0_driver_exit(void) {
	serdev_device_driver_unregister(&uart0_drv_driver);
}

module_init(ldd_uart0_driver_init);
module_exit(ldd_uart0_driver_exit);