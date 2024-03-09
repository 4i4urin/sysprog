#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>


#define MY_MAJOR        31
#define DEVICE_NAME     "my_chdev2"


char* phrase = \
    "The machines rose from the ashes of the nuclear fire.\n" \
    "Their war to exterminate mankind has raged for decades, but the final battle would not be fought in the future.\n" \
    "It would be fought here, in our present.\n" \
    "Tonight.\n";

ssize_t my_read(
    struct file* ptr_file, char* user_buff, 
    size_t length, loff_t* offp
);

ssize_t my_write(
    struct file* ptr_file, const char* user_buff, 
    size_t count, loff_t* offp
);

int my_open(struct inode* incode, struct file* ptr_file);
int my_close(struct inode* incode, struct file* ptr_file);


struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_close
};

struct cdev my_dev;


int init_module(void)
{

    if (register_chrdev_region(MKDEV(MY_MAJOR, 0), 1, DEVICE_NAME) < 0)
    {
        printk(KERN_WARNING "Can't registrate device\n");
        return 0;
    }

    cdev_init(&my_dev, &my_fops);
    if (cdev_add(&my_dev, MKDEV(MY_MAJOR, 0), 1) < 0)
        printk(KERN_WARNING "Can't add device\n");


    printk(KERN_INFO "Start work\n");
    return 0;
}


void cleanup_module(void)
{
    cdev_del(&my_dev);
    unregister_chrdev_region(MKDEV(MY_MAJOR, 0), 1);

    printk(KERN_INFO "Finish work\n");
}


ssize_t my_read(
    struct file* ptr_file, char* user_buff, 
    size_t length, loff_t* offp) 
{
    static uint8_t stop_read = 0;
    printk(KERN_INFO "Try read\n");

    if (stop_read == 1)
    {
        stop_read = 0;
        return 0;
    }

    int* some_data = ptr_file->private_data;
    ssize_t bytes_read = 0;
    for (size_t i = 0; i < length && (phrase[i] != 0); i++)
    {
        put_user(phrase[i], user_buff++);    
        bytes_read++;
    }
    stop_read += 1;
    *some_data = bytes_read;
    
    return bytes_read;
}


ssize_t my_write(
    struct file* ptr_file, const char* user_buff, 
    size_t count, loff_t* offp) 
{
    printk(KERN_INFO "Try write\n");
    return 0;
}


static int some_data = 0;
int my_open(struct inode* incode, struct file* ptr_file)
{
    ptr_file->private_data = &some_data;
    printk(KERN_INFO "Open device\n");
    return 0;
}


int my_close(struct inode* incode, struct file* ptr_file)
{
    printk(KERN_INFO "Close device read = %d\n", 
        *((int*)ptr_file->private_data));
    return 0;
}












MODULE_LICENSE("GPL");
