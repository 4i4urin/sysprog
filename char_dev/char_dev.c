#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>


#define MY_MAJOR        30
#define DEVICE_NAME     "my_chdev"

#define BUF_LEN         1024

ssize_t my_read(
    struct file* ptr_file, char* user_buff, 
    size_t length, loff_t* offp
);

ssize_t my_write(
    struct file* ptr_file, const char* user_buff, 
    size_t length, loff_t* offp
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

char echo[BUF_LEN];


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
    char* echo = ((char*)ptr_file->private_data);
    int bytes_read = 0;

    printk(KERN_INFO "Try read\n");

    for (int i = 0; i < strlen(echo) && i < BUF_LEN; i++)
    {
        put_user(echo[i], user_buff++); 
        bytes_read++;
    }

    echo[strlen(echo) - bytes_read] = '\0';

    printk(KERN_INFO "DEV read %d bytes\n", bytes_read);
    
    return bytes_read;
}


ssize_t my_write(
    struct file* ptr_file, const char* user_buff, 
    size_t length, loff_t* offp) 
{
    char* echo = ((char*)ptr_file->private_data);
    int left = 0;
    int prev_len = strlen(echo);

    printk(KERN_INFO "Try write\n");
    if ( !strlen(echo) )
    {
        if (length >= BUF_LEN - 1)
            length = BUF_LEN - 1;
        left = copy_from_user(echo, user_buff, length);

        if ( left )
            printk(KERN_WARNING "WARNING: copy ony part of str\n");
    }
    else if (strlen(echo) < BUF_LEN - 1)
    {
        if (length >= (BUF_LEN - strlen(echo)))
            length = BUF_LEN - strlen(echo);
        left = copy_from_user(echo + strlen(echo), user_buff, length);

        if ( left )
            printk(KERN_WARNING "WARNING: copy ony part of str\n");
    } else
    {
        printk(KERN_WARNING "WARNING: Devise buff is full\n");
        return length;
    }

    echo[prev_len + length - left - 1] = '\0';
    printk(KERN_INFO "DEV write %ld bytes\n", strlen(echo));

    return length;
}


int my_open(struct inode* incode, struct file* ptr_file)
{
    ptr_file->private_data = echo;

    printk(KERN_INFO "Open device\n");
    return 0;
}


int my_close(struct inode* incode, struct file* ptr_file)
{
    char* echo = ((char*)ptr_file->private_data);

    printk(KERN_INFO "Close device STORED = %s\n", echo);
    return 0;
}


MODULE_LICENSE("GPL");
