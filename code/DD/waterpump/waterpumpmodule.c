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



#define DEVICE_NAME "waterpump0"
#define CLASS_NAME "waterpumpClass"




MODULE_LICENSE("GPL");

/* Device variables */
static struct class* waterpumpDevice_class = NULL;
static struct device* waterpumpDevice_device = NULL;
static dev_t waterpumpDevice_majorminor;
static struct cdev c_dev;  // Character device structure


static struct class *s_pDeviceClass;
static struct device *s_pDeviceObject;
struct GpioRegisters *s_pGpioRegisters;

static const int waterpumpGpioPin = 12;

ssize_t waterpump_device_write(struct file *pfile, const char __user *pbuff, size_t len, loff_t *off) { //tirar o static
	struct GpioRegisters *pdev; 
	
	pr_alert("%s: called (%u)\n",__FUNCTION__,len);

	
	if(unlikely(pfile->private_data == NULL))
		return -EFAULT;

	pdev = (struct GpioRegisters *)pfile->private_data;
	if (pbuff[0]=='0')
		SetGPIOOutputValue(pdev, waterpumpGpioPin, 0);
	else
		SetGPIOOutputValue(pdev, waterpumpGpioPin, 1);
	return len;
}

ssize_t waterpump_device_read(struct file *pfile, char __user *p_buff,size_t len, loff_t *poffset){
	pr_alert("%s: called (%u)\n",__FUNCTION__,len);
	return 0;
}

int waterpump_device_close(struct inode *p_inode, struct file * pfile){
	
	pr_alert("%s: called\n",__FUNCTION__);
	pfile->private_data = NULL;
	return 0;
}


int waterpump_device_open(struct inode* p_indode, struct file *p_file){

	pr_alert("%s: called\n",__FUNCTION__);
	p_file->private_data = (struct GpioRegisters *) s_pGpioRegisters;
	return 0;
	
}


static struct file_operations waterpumpDevice_fops = {
	.owner = THIS_MODULE,
	.write = waterpump_device_write,
	.read = waterpump_device_read,
	.release = waterpump_device_close,
	.open = waterpump_device_open,
};

static int __init waterpumpModule_init(void) {
	int ret;
	struct device *dev_ret;

	pr_alert("%s: called\n",__FUNCTION__);

	if ((ret = alloc_chrdev_region(&waterpumpDevice_majorminor, 0, 1, DEVICE_NAME)) < 0) {
		return ret;
	}

	if (IS_ERR(waterpumpDevice_class = class_create(THIS_MODULE, CLASS_NAME))) {
		unregister_chrdev_region(waterpumpDevice_majorminor, 1);
		return PTR_ERR(waterpumpDevice_class);
	}
	if (IS_ERR(dev_ret = device_create(waterpumpDevice_class, NULL, waterpumpDevice_majorminor, NULL, DEVICE_NAME))) {
		class_destroy(waterpumpDevice_class);
		unregister_chrdev_region(waterpumpDevice_majorminor, 1);
		return PTR_ERR(dev_ret);
	}

	cdev_init(&c_dev, &waterpumpDevice_fops);
	c_dev.owner = THIS_MODULE;
	if ((ret = cdev_add(&c_dev, waterpumpDevice_majorminor, 1)) < 0) {
		printk(KERN_NOTICE "Error %d adding device", ret);
		device_destroy(waterpumpDevice_class, waterpumpDevice_majorminor);
		class_destroy(waterpumpDevice_class);
		unregister_chrdev_region(waterpumpDevice_majorminor, 1);
		return ret;
	}


	s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));
	//s_pGpioRegisters = (struct GpioRegisters *)ioremap_cache(GPIO_BASE, sizeof(struct GpioRegisters));
	
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pGpioRegisters);
	
	SetGPIOFunction(s_pGpioRegisters, waterpumpGpioPin, 0b001); //Output

	return 0;
}

static void __exit waterpumpModule_exit(void) {
	
	pr_alert("%s: called\n",__FUNCTION__);
	
	SetGPIOFunction(s_pGpioRegisters, waterpumpGpioPin, 0); //Configure the pin as input
	iounmap(s_pGpioRegisters);
	cdev_del(&c_dev);
	device_destroy(waterpumpDevice_class, waterpumpDevice_majorminor);
	class_destroy(waterpumpDevice_class);
	unregister_chrdev_region(waterpumpDevice_majorminor, 1);
}

module_init(waterpumpModule_init);
module_exit(waterpumpModule_exit);
