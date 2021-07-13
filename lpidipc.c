#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <linux/pid.h>
#include <linux/delay.h>

#define DEVICE_NAME "comchar"
#define  CLASS_NAME "ipc"

static struct class*  class;
static struct device*  device;
static int major;
static int given_task_pid = -1;
static struct task_struct* given_task = NULL;
static struct cfs_rq* pid_cfs_rq = NULL;

atomic_t sync = ATOMIC_INIT(0);

/*  executed once the device is closed or releaseed by userspace
 *  @param inodep: pointer to struct inode
 *  @param filep: pointer to struct file 
 */
static int comchar_release(struct inode *inodep, struct file *filep)
{    
    pr_info("comchar: Device successfully closed\n");

    return 0;
}

/* executed once the device is opened.
 * It is not threads safe yet.
 *
 */
static int comchar_open(struct inode *inodep, struct file *filep)
{
    int ret = 0; 
 
    pr_info("comchar: Device opened\n");

    return ret;
}


static ssize_t comchar_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	size_t ret = 0;
	atomic_dec(&sync);
	while(atomic_read(&sync) < 0)
	{
		//ndelay(1);
		usleep_range(5, 10);
		//schedule_timeout_idle(0);
	}
	return ret;
}

static ssize_t comchar_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	size_t ret = 0;
	atomic_inc(&sync);
	return ret;
}

static const struct file_operations comchar_fops = {
    .open = comchar_open,
    .read = comchar_read,
    .write = comchar_write,
    .release = comchar_release,
    /*.unlocked_ioctl = comchar_ioctl,*/
    .owner = THIS_MODULE,
};

static int __init comchar_init(void)
{
    int ret = 0;    
    major = register_chrdev(0, DEVICE_NAME, &comchar_fops);

    if (major < 0) {
        pr_info("comchar: fail to register major number!");
        ret = major;
        goto out;
    }

    class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(class)){ 
        unregister_chrdev(major, DEVICE_NAME);
        pr_info("comchar: failed to register device class");
        ret = PTR_ERR(class);
        goto out;
    }

    device = device_create(class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(device)) {
        class_destroy(class);
        unregister_chrdev(major, DEVICE_NAME);
        ret = PTR_ERR(device);
        goto out;
    }
    
out: 
    return ret;
}

static void __exit comchar_exit(void)
{
    device_destroy(class, MKDEV(major, 0));  
    class_unregister(class);
    class_destroy(class); 
    unregister_chrdev(major, DEVICE_NAME);
    
    pr_info("comchar: unregistered!");
}

module_init(comchar_init);
module_exit(comchar_exit);
MODULE_LICENSE("GPL");
