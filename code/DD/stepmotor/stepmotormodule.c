#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>

#include <linux/io.h>
//#include <mach/platform.h>

#include "utils.h"



#define DEVICE_NAME "stepmotor0"
#define CLASS_NAME "stepmotorClass"




MODULE_LICENSE("GPL");

/* Device variables */
static struct class* stepmotorDevice_class = NULL;
static struct device* stepmotorDevice_device = NULL;
static dev_t stepmotorDevice_majorminor;
static struct cdev c_dev;  // Character device structure


static struct class *s_pDeviceClass;
static struct device *s_pDeviceObject;
struct GpioRegisters *s_pGpioRegisters;



static const int StepmotorGpioPinB1 = 5;
static const int StepmotorGpioPinA2 = 6;
static const int StepmotorGpioPinA1 = 13;
static const int StepmotorGpioPinB2 = 26;

ssize_t stepmotor_device_write(struct file *pfile, const char __user *pbuff, size_t len, loff_t *off) { //tirar o static
	struct GpioRegisters *pdev; 
	
	pr_alert("%s: called (%u)\n",__FUNCTION__,len);

	
	if(unlikely(pfile->private_data == NULL))
		return -EFAULT;

	pdev = (struct GpioRegisters *)pfile->private_data;
	switch (pbuff[0]) {
	case '0':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 0);
		break;
	case '1':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 0);
		break;
	case '2':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 0);
		break;
	case '3':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 0);
		break;
	case '4':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 0);
		break;
	case '5':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 0);
		break;
	case '6':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 0);
		break;
	case '7':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 0);
		break;
	case '8':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 1);
		break;
	case '9':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 1);
		break;
	case 'A':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 1);
		break;
	case 'B':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 1);
		break;
	case 'C':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 1);
		break;
	case 'D':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 1);
		break;
	case 'E':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 0);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 1);
		break;
	case 'F':
		SetGPIOOutputValue(pdev, StepmotorGpioPinA1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB1, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinA2, 1);
		SetGPIOOutputValue(pdev, StepmotorGpioPinB2, 1);
		break;
	}
	return len;
}

ssize_t stepmotor_device_read(struct file *pfile, char __user *p_buff,size_t len, loff_t *poffset){
	pr_alert("%s: called (%u)\n",__FUNCTION__,len);
	return 0;
}

int stepmotor_device_close(struct inode *p_inode, struct file * pfile){
	
	pr_alert("%s: called\n",__FUNCTION__);
	pfile->private_data = NULL;
	return 0;
}


int stepmotor_device_open(struct inode* p_indode, struct file *p_file){

	pr_alert("%s: called\n",__FUNCTION__);
	p_file->private_data = (struct GpioRegisters *) s_pGpioRegisters;
	return 0;
	
}


static struct file_operations stepmotorDevice_fops = {
	.owner = THIS_MODULE,
	.write = stepmotor_device_write,
	.read = stepmotor_device_read,
	.release = stepmotor_device_close,
	.open = stepmotor_device_open,
};

static int __init stepmotorModule_init(void) {
	int ret;
	struct device *dev_ret;

	pr_alert("%s: called\n",__FUNCTION__);

	if ((ret = alloc_chrdev_region(&stepmotorDevice_majorminor, 0, 1, DEVICE_NAME)) < 0) {
		return ret;
	}

	if (IS_ERR(stepmotorDevice_class = class_create(THIS_MODULE, CLASS_NAME))) {
		unregister_chrdev_region(stepmotorDevice_majorminor, 1);
		return PTR_ERR(stepmotorDevice_class);
	}
	if (IS_ERR(dev_ret = device_create(stepmotorDevice_class, NULL, stepmotorDevice_majorminor, NULL, DEVICE_NAME))) {
		class_destroy(stepmotorDevice_class);
		unregister_chrdev_region(stepmotorDevice_majorminor, 1);
		return PTR_ERR(dev_ret);
	}

	cdev_init(&c_dev, &stepmotorDevice_fops);
	c_dev.owner = THIS_MODULE;
	if ((ret = cdev_add(&c_dev, stepmotorDevice_majorminor, 1)) < 0) {
		printk(KERN_NOTICE "Error %d adding device", ret);
		device_destroy(stepmotorDevice_class, stepmotorDevice_majorminor);
		class_destroy(stepmotorDevice_class);
		unregister_chrdev_region(stepmotorDevice_majorminor, 1);
		return ret;
	}


	s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));
	//s_pGpioRegisters = (struct GpioRegisters *)ioremap_nocache(GPIO_BASE, sizeof(struct GpioRegisters));
	
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pGpioRegisters);
	
	SetGPIOFunction(s_pGpioRegisters, StepmotorGpioPinA1, 0b001); //Output
	SetGPIOFunction(s_pGpioRegisters, StepmotorGpioPinA2, 0b001); //Output
	SetGPIOFunction(s_pGpioRegisters, StepmotorGpioPinB1, 0b001); //Output
	SetGPIOFunction(s_pGpioRegisters, StepmotorGpioPinB2, 0b001); //Output

	return 0;
}

static void __exit stepmotorModule_exit(void) {
	
	pr_alert("%s: called\n",__FUNCTION__);
	 
	//Configure the pins as input
	
	SetGPIOFunction(s_pGpioRegisters, StepmotorGpioPinA1, 0);
	SetGPIOFunction(s_pGpioRegisters, StepmotorGpioPinA2, 0);
	SetGPIOFunction(s_pGpioRegisters, StepmotorGpioPinB1, 0);
	SetGPIOFunction(s_pGpioRegisters, StepmotorGpioPinB2, 0);
	
	iounmap(s_pGpioRegisters);
	cdev_del(&c_dev);
	device_destroy(stepmotorDevice_class, stepmotorDevice_majorminor);
	class_destroy(stepmotorDevice_class);
	unregister_chrdev_region(stepmotorDevice_majorminor, 1);
}

module_init(stepmotorModule_init);
module_exit(stepmotorModule_exit);
