#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>  
#include <linux/gpio.h>     
#include <linux/err.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vallejo");
MODULE_DESCRIPTION("device driver - GPIO Driver for raspberry pi 5");
MODULE_VERSION("1.0");

/* some function does not work in kernel 6.6.28 */
#define USING_RASPBERRY_PI_5

//LED is connected to this GPIO, check file /sys/kernel/debug/gpio */
#define GPIO_PIN_CHIP_LED (592) 
#define GPIO_PIN_BOARD_LABEL ("GPIO21") 

#define DRV_DEVICE_NAME ("ldd_gpio_device")
#define DRV_CLASS_NAME ("ldd_gpio_class")

/* Variables for device and device class */
dev_t my_device_nr = 0;
static struct class *dev_class;
static struct cdev cdevice;

 static int __init ldd_gpio_driver_init(void);
static void __exit ldd_gpio_driver_exit(void);
 
/*************** Driver functions **********************/
static int drv_open(struct inode *inode, struct file *file);
static int drv_release(struct inode *inode, struct file *file);
static ssize_t drv_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t drv_write(struct file *filp, const char *buf, size_t len, loff_t * off);
/******************************************************/
 
//File operation structure 
static struct file_operations fops =
{
  .owner          = THIS_MODULE,
  .read           = drv_read,
  .write          = drv_write,
  .open           = drv_open,
  .release        = drv_release,
};

/*
** This function will be called when we open the Device file
*/ 
static int drv_open(struct inode *inode, struct file *file)
{
  pr_info("ldd_gpio: Device File Opened\n");
  return 0;
}

/*
** This function will be called when we close the Device file
*/
static int drv_release(struct inode *inode, struct file *file)
{
  pr_info("ldd_gpio: Device File Closed\n");
  return 0;
}

/*
** This function will be called when we read the Device file
*/ 
static ssize_t drv_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state = 0;
  
  //reading GPIO value
  gpio_state = gpio_get_value(GPIO_PIN_CHIP_LED);
  
  //write to user
  len = 1;
  if( copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("ldd_gpio: ERROR: Not all the bytes have been copied to user\n");
  }
  
  pr_info("Read function : GPIO_LED_OUT_PIN = %d \n", gpio_state);
  
  return 0;
}

/*
** This function will be called when we write the Device file
*/ 
static ssize_t drv_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[10] = {0};
  
  if( copy_from_user( rec_buf, buf, len ) > 0) {
    pr_err("ldd_gpio: ERROR: not all the bytes have been copied from user\n");
  }
  
  pr_info("Write Function : GPIO_LED_OUT_PIN Set = %c\n", rec_buf[0]);
  
  if (rec_buf[0] == '1') {
    //set the GPIO value to HIGH
    gpio_set_value(GPIO_PIN_CHIP_LED, 1);
  } else if (rec_buf[0] == '0') {
    //set the GPIO value to LOW
    gpio_set_value(GPIO_PIN_CHIP_LED, 0);
  } else {
    pr_err("ldd_gpio: Unknown command : Please provide either 1 or 0 \n");
  }
  
  return len;
}

/*
** Module Init function
*/ 
static int __init ldd_gpio_driver_init(void)
{
  
  /*Allocating Major number*/
  if((alloc_chrdev_region(&my_device_nr, 0, 1, DRV_DEVICE_NAME)) < 0){
    pr_err("ldd_gpio: Cannot allocate major number\n");
    unregister_chrdev_region(my_device_nr,1);
    return -1;
  }
 
#ifndef USING_RASPBERRY_PI_5
  /*Creating struct class*/
  dev_class = class_create(THIS_MODULE, DRV_CLASS_NAME);
#else
  /*Creating struct class*/
  dev_class = class_create(DRV_CLASS_NAME);
#endif
 if(dev_class == NULL){
    pr_err("ldd_gpio: Cannot create the struct class\n");
    class_destroy(dev_class);
    return -1;
  }
  /* Using this call the GPIO  will be visible in /dev/ldd_gpio_device
  ** echo 1 > /dev/ldd_gpio_device  (turn ON the LED)
  ** echo 0 > /dev/ldd_gpio_device  (turn OFF the LED)
  ** cat /dev/ldd_gpio_device       (read the value LED)
  */
  if(device_create(dev_class, NULL, my_device_nr, NULL, DRV_DEVICE_NAME) == NULL){
    pr_err( "ldd_gpio: Cannot create the Device \n");
    device_destroy(dev_class, my_device_nr);
    return -1;
  }
 /*Creating cdev structure*/
  cdev_init(&cdevice, &fops);
 

  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_PIN_CHIP_LED) == false){
    pr_err("ldd_gpio: GPIO %d is not valid\n", GPIO_PIN_CHIP_LED);
    device_destroy(dev_class, my_device_nr);
    return -1;
  }

  /*Adding character device to the system*/
  if((cdev_add(&cdevice, my_device_nr, 1)) < 0){
    pr_err("ldd_gpio: Cannot add the device to the system\n");
    cdev_del(&cdevice);
    return -1;
  }
      
  //Requesting the GPIO
  if(gpio_request(GPIO_PIN_CHIP_LED, GPIO_PIN_BOARD_LABEL) < 0){
    pr_err("ldd_gpio: ERROR: GPIO %d couldn't be requested\n", GPIO_PIN_CHIP_LED);
    cdev_del(&cdevice);
    device_destroy(dev_class, my_device_nr);
    class_destroy(dev_class);
    unregister_chrdev_region(my_device_nr,1);
    gpio_free(GPIO_PIN_CHIP_LED);
    return -1;
  }
  
  //configure the GPIO as output
  gpio_direction_output(GPIO_PIN_CHIP_LED, 0);

  pr_info("ldd_gpio: Device Driver Insert...Done!\n");
  return 0;
}

/*
** Module exit function
*/ 
static void __exit ldd_gpio_driver_exit(void)
{
  gpio_set_value(GPIO_PIN_CHIP_LED, 0);
  gpio_free(GPIO_PIN_CHIP_LED);
  device_destroy(dev_class, my_device_nr);
  class_destroy(dev_class);
  cdev_del(&cdevice);
  unregister_chrdev_region(my_device_nr, 1);
  pr_info("ldd_gpio: Device Driver Remove...Done!\n");
}
 
module_init(ldd_gpio_driver_init);
module_exit(ldd_gpio_driver_exit);
