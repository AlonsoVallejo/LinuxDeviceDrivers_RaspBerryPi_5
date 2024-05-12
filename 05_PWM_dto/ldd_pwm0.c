#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/pwm.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/of.h>

/* Mod Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vallejo");
MODULE_DESCRIPTION("PWM driver to control PWM0 in RbPi5");

#define DRV_PWM_DEVICE_NAME "ldd_pwm0_device"
#define DRV_PWM_CLASS_NAME "ldd_pwm0_class"

#define PWM_INIT_DUTTY_CYCLE_NS (50000) 
#define PWM_INIT_PERIOD_NS (50000)

/* Variables for device and device class */
static dev_t pwm_device_num;
static struct class *pwm_class;
static struct cdev char_device;
struct pwm_device *pwm0 = NULL;

/* Driver functions */
static ssize_t driver_pwm0_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs);

/* Platform driver functions */
static int platform_pwm0_probe(struct platform_device *pdev);
static int platform_pwm0_remove(struct platform_device *pdev);

/* file operations structure for char device */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = driver_pwm0_write
};

/* setup device tree/open firmware table for compatible matching 
 * to load this module when device tree overlay is initailized */
static struct of_device_id my_match_table[] = {
     {
             .compatible = "pwm0_pin12_led",
     },
     {},
};

/* expose the above as a module alais so module is found and loaded */
MODULE_DEVICE_TABLE(of, my_match_table);

/* setup platform functions (probe and remove) and match table */
static struct platform_driver pwm0_device_driver = {
    .driver = {
        .name = "pwm0_led_drv",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(my_match_table),
    },
    .probe = platform_pwm0_probe,
    .remove = platform_pwm0_remove,
};

/*
** This function will be called when we write the Device file
*/ 
static ssize_t driver_pwm0_write(struct file *File, const char *user_buffer, size_t len, loff_t *offs) 
{
  	char rec_dutty_cycle[3] = {0}; /* i.e, 7% = "7", 56% = "56", 100% = "100" */
	uint32_t duty_cycle = 0;
	int ret_val = 0;

	ret_val = copy_from_user(rec_dutty_cycle, user_buffer, sizeof(rec_dutty_cycle) - 1 );
	/* Copy data from user space to kernel space */
	if( ret_val > 0) {
		pr_err("ldd_pwm0: not all the bytes have been copied from user\n");
		return ret_val;
	}

	/* Convert string to integer */
	ret_val = kstrtouint(rec_dutty_cycle, 10, &duty_cycle);
	if(ret_val) {
		pr_err("ldd_pwm0: str to into dutty cycle convertion failed\n");
		return ret_val;
	}

	/* Apply dutty cycle with conversion factor */ 
	if (pwm_config(pwm0, duty_cycle * 500, PWM_INIT_PERIOD_NS)){
		pr_err("ldd_pwm0: couldn't set pwm config dutty cycle\n");
	} 

	return ret_val;  
} 

/* install platform driver fuction */
static int platform_pwm0_probe(struct platform_device *pdev) 
{
	/* get PWM child if platform device of device tree overlay */
    pwm0 = devm_pwm_get(&pdev->dev, NULL); 
	if (IS_ERR_OR_NULL(pwm0)) {
		pr_err("ldd_pwm0: Could not get PWM0! %ld\n", PTR_ERR(pwm0));
		return -1;
	} 
	
    /* PWM initial state*/
   	if (pwm_config(pwm0, PWM_INIT_DUTTY_CYCLE_NS, PWM_INIT_PERIOD_NS)){
        pr_err("ldd_pwm0: config failed (init)\n");
    } 
	
    if (pwm_enable(pwm0)){
        pr_err("ldd_pwm0: enable failed\n");
    } 
	
	/* create character device for userspace interface */
	if( alloc_chrdev_region(&pwm_device_num, 0, 1, DRV_PWM_DEVICE_NAME) < 0) {
		pr_err("ldd_pwm0: Device number not allocated!\n");
		return -1;
	}

	if((pwm_class = class_create(DRV_PWM_CLASS_NAME)) == NULL) {
		pr_err("ldd_pwm0: Class not created!\n");
		unregister_chrdev_region(pwm_device_num, 1);
	    return -1;
	}

    /* Using this call the GPIO  will be visible in /dev/DRV_PWM_DEVICE_NAME */
	if (device_create(pwm_class, NULL, pwm_device_num, NULL, DRV_PWM_DEVICE_NAME) == NULL) {
		pr_err("ldd_pwm0: Can not create device file!\n");
		class_destroy(pwm_class);
        return -1;
	}

	cdev_init(&char_device, &fops);

	if(cdev_add(&char_device, pwm_device_num, 1) == -1) {
		pr_err("ldd_pwm0: Registering device to kernel failed!\n");
		device_destroy(pwm_class, pwm_device_num);
        return -1;
	}

	pr_info("ldd_pwm0: Installed\n");
	return 0;
}

/* remove platform driver fuction */ 
static int platform_pwm0_remove(struct platform_device *pdev) 
{
	pwm_put(pwm0);
	pwm_disable(pwm0);
	cdev_del(&char_device);
	device_destroy(pwm_class, pwm_device_num);
	class_destroy(pwm_class);
	unregister_chrdev_region(pwm_device_num, 1);
	pr_info("ldd_pwm0: Removed\n");
	return 0;
}

module_platform_driver(pwm0_device_driver);
