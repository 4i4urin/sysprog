#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "ioctl.h"
#include "buf_operations.h"

#define BUF_LEN 1024

ssize_t my_read(
    struct file* ptr_file, char* user_buff, 
    size_t length, loff_t* offp
);

ssize_t my_write(
    struct file* ptr_file, const char* user_buff, 
    size_t length, loff_t* offp
);

long my_ioctl(
    struct file* ptr_file, unsigned int cmd, unsigned long arg
);

int my_open(struct inode* incode, struct file* ptr_file);
int my_close(struct inode* incode, struct file* ptr_file);



struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_close,
    .unlocked_ioctl = my_ioctl
};

struct cdev my_dev;

char buf[BUF_LEN];


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


long my_ioctl(
    struct file* ptr_file, unsigned int cmd, unsigned long arg)
{
    char* buf = ((char*)ptr_file->private_data);

    switch (cmd)
    {
    case REVERSE:
        printk(KERN_INFO "IOCTL: reverse buff str\n");
        reverse_buf(buf);
        break;
    case SELECT_DELETE:
        printk(KERN_INFO "IOCTL: delete received str from buff\n");
        select_delete(buf, arg);
        break;
    case CHANGE_LETTER:
        printk(KERN_INFO "IOCTL: change received letter to another in buff\n");
        change_letter(buf, arg);
        break;
    case ADD_STR:
        printk(KERN_INFO "IOCTL: add string from the beginning\n");
        add_str_begin(buf, arg);
        break;
    case ADD_SIZE:
        printk(KERN_INFO "IOCTL: add size of every str\n");
        add_str_size(buf);
        break;
    default:
        printk(KERN_WARNING "IOCTL: unexpected command\n");
    }

    return 0;
}


ssize_t my_read(
    struct file* ptr_file, char* user_buff, 
    size_t length, loff_t* offp) 
{
    char* buf = ((char*)ptr_file->private_data);
    int bytes_read = 0;

    printk(KERN_INFO "Try read\n");

    for (int i = 0; i < strlen(buf) && i < BUF_LEN; i++)
    {
        put_user(buf[i], user_buff++); 
        bytes_read++;
    }

    buf[strlen(buf) - bytes_read] = '\0';

    printk(KERN_INFO "DEV read %d bytes\n", bytes_read);
    
    return bytes_read;
}


ssize_t my_write(
    struct file* ptr_file, const char* user_buff, 
    size_t length, loff_t* offp) 
{
    char* buf = ((char*)ptr_file->private_data);
    int left = 0;
    int prev_len = strlen(buf);

    printk(KERN_INFO "Try write\n");
    if ( !strlen(buf) )
    {
        if (length >= BUF_LEN - 1)
            length = BUF_LEN - 1;
        left = copy_from_user(buf, user_buff, length);

        if ( left )
            printk(KERN_WARNING "WARNING: copy ony part of str\n");

    }
    else if (strlen(buf) < BUF_LEN - 1)
    {
        if (length >= (BUF_LEN - strlen(buf)))
            length = BUF_LEN - strlen(buf);
        left = copy_from_user(buf + strlen(buf), user_buff, length);

        if ( left )
            printk(KERN_WARNING "WARNING: copy ony part of str\n");
    } else
    {
        printk(KERN_WARNING "WARNING: Devise buff is full\n");
        return length;
    }

    buf[prev_len + length - left] = '\0';
    printk(KERN_INFO "DEV write %ld bytes\n", strlen(buf));

    return length;
}


int my_open(struct inode* incode, struct file* ptr_file)
{
    ptr_file->private_data = buf;

    printk(KERN_INFO "Open device\n");
    return 0;
}


int my_close(struct inode* incode, struct file* ptr_file)
{
    char* buf = ((char*)ptr_file->private_data);

    printk(KERN_INFO "Close device STORED = %s\n", buf);
    return 0;
}


MODULE_LICENSE("GPL");
