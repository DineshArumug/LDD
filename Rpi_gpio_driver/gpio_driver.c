#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/err.h>

#define DEV_MEM_SIZE 10

char device_buffer[DEV_MEM_SIZE];

/*To hold the file oprations performed on this driver */
ssize_t hello_read(struct file *pfile, char __user *buff, size_t length, loff_t *offset)
{
 char temp[3]= " \n";
 int len;
 temp[0] = gpio_get_value(17) + '0';

 length=2;
 /* copy from user */
 if(copy_to_user(buff,&temp,length))
 {
	 return -EFAULT;
 }
 printk(KERN_INFO"Inside the %s function gpio_status = %c",__FUNCTION__,temp[0]);
 return length;
}
ssize_t hello_write(struct file *pfile, const char __user *buff, size_t length, loff_t *offset)
{
 char value[10]={0};
 printk(KERN_INFO"Inside the %s function",__FUNCTION__);

 /* copy from user */
 if(copy_from_user(value,buff,length))
 {
	 return -EFAULT;
 }
  
   if(value[0]=='0')
   {
     gpio_set_value(21,0);
   }
   else if(value[0]=='1')
   {
     gpio_set_value(21,1);
   }
   else
   {
     printk("Invalid Input\n");
   }
 return length;
}

int hello_open(struct inode *pinode, struct file *pfile)
{
 printk(KERN_INFO"File Open successfully\n");
 return 0; 
}
int hello_release(struct inode *pinode, struct file *pfile)
{
 printk(KERN_INFO"File Close successfully\n");
 return 0;
}

struct file_operations gpio_fileopration =
{
 .owner   = THIS_MODULE,
 .open    = hello_open,
 .read    = hello_read,
 .write   = hello_write,
 .release = hello_release,
};

/*This hold the device number*/
dev_t device_number;

/* cdev variable*/
struct cdev chr_cdev;

struct class *dev_class;

struct device *device_gpio;

static int __init hello_init02(void)
{
 printk(KERN_INFO"Inside the %s function",__FUNCTION__);
 /*registration of char driver if you know Major number which is not in use */
 
 /*1. Dynamically allocate a device number */
 alloc_chrdev_region(&device_number,0,1,"gpio_driver");

 printk(KERN_INFO"Device Number <major>:<minior> = %d:%d\n",MAJOR(device_number),MINOR(device_number)); 
 /*2. initilize the cdev stracture with fops */
 cdev_init(&chr_cdev,&gpio_fileopration);

 /*3. Register a device (cdev stracture) with VFS */
 cdev_add(&chr_cdev,device_number,1);

 /*4. create device class under /sys/class/ */
 dev_class =  class_create(THIS_MODULE,"hello_class");

 /*5. populate the sysfs with device information */
 device_gpio = device_create(dev_class,NULL,device_number,NULL,"mygpio");

 /*GPIO 4 init */
 if(gpio_request(21,"LED"))
 {
	 printk("Can not allocate GPIO 4\n");
 }

 gpio_direction_output(21,0);

 gpio_request(17,"BUTTON");
 gpio_direction_input(17);
 return 0;
}

static void __exit hello_exit02(void)
{
 printk(KERN_INFO"Inside the %s function",__FUNCTION__);
 /*unregister the char driver*/
 gpio_set_value(21,0);
 gpio_free(21);
 gpio_free(17);
 class_destroy(dev_class);
 cdev_del(&chr_cdev);
 unregister_chrdev_region(device_number,1);
 printk(KERN_INFO"Device Unload");
}

module_init(hello_init02);
module_exit(hello_exit02);

MODULE_AUTHOR("SwapNiL");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO module");

