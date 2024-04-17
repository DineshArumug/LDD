#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define RD_VALUE _IOR('a','b',int32_t *)
#define WR_VALUE _IOW('a','b',int32_t *)

int32_t value = 0;
dev_t dev = 0;
static struct class *myclass;
static struct cdev my_cdev;

static int my_open(struct inode *inode,struct file *file)
{
	printk("Driver open Function is called.... !!!\n");
	return 0;
}

static int my_close(struct inode *inode,struct file *file)
{
	printk("Driver close function called ... !!!\n");
	return 0;
}

static ssize_t  my_write(struct file *fil, const char *buf, size_t len, loff_t * off)
{
	printk("Driver write function called ... !!!\n");
//	printk("Data write = %s\n",(char *)buf);
	return 0;
}

static ssize_t my_read(struct file *file,char __user *buf,size_t len,loff_t *off)
{
	printk("Driver read function called... !!!\n");
	return 0;
}

static long my_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
		case WR_VALUE:
			printk("WR_VALUE\n");
			if(copy_from_user(&value,(int32_t *)arg,sizeof(value)))
			{
				pr_err("Data Write : error ...\n");
			}
			printk("Value = %d \n",value);
			break;
		case RD_VALUE:
			printk("RD_VALUE\n");
			if(copy_to_user((int32_t *)arg,&value,sizeof(value)))
			{
				pr_err("Data Read : error ...\n");
			}
			break;
	}
	return 0;
}

struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.write = my_write,
	.read = my_read,
	.unlocked_ioctl = my_ioctl,
};
static int __init main_init(void)
{
	if((alloc_chrdev_region(&dev,2,10,"Dinesh_Minor_Major"))<0)
	{
		printk(KERN_INFO "Cannot be allocate the major and minor\n");
		return -1;
	}
	printk(KERN_INFO "Major = %d Minor = %d\n",MAJOR(dev),MINOR(dev));

	cdev_init(&my_cdev,&fops);
	if((cdev_add(&my_cdev,dev,1))<0)
	{
		printk("Cannot add the device to the system\n");
		unregister_chrdev_region(dev,10);
		return -1;
	}

	myclass = class_create(THIS_MODULE,"DINESH_CLASS");
	if(IS_ERR(myclass))
	{
		printk("Cannot be create class\n");
		unregister_chrdev_region(dev,10);
		return -1;
	}
	if(IS_ERR(device_create(myclass,NULL,dev,NULL,"Dinesh_Dev")))
	{
		printk("Cannot be create device file\n");
		class_destroy(myclass);
		unregister_chrdev_region(dev,10);
		return -1;
	}
	printk(KERN_INFO "Kernel Module is inserted successfully\n");
	return 0;
}

static void __exit main_exit(void)
{
	device_destroy(myclass,dev);
	class_destroy(myclass);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,10);
	printk(KERN_INFO "Kernel Module is removed successfully\n");
}

module_init(main_init);
module_exit(main_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DINESH");
MODULE_DESCRIPTION("SIMPLE DEVICE DRIVER PROGRAM");
MODULE_VERSION("1.0");
