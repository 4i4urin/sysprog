#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>


#define PROCFS_NAME "first_proc"
#define RULES_OF_USE 0646
#define BUF_LEN 1024


static ssize_t myproc_read(
    struct file* ptr_file, char* user_buff, 
    size_t length, loff_t* offp );

static ssize_t myproc_write(
    struct file* ptr_file, const char* user_buff, 
    size_t length, loff_t* offp );

static int myproc_open(struct inode* incode, struct file* ptr_file);
static int myproc_close(struct inode* incode, struct file* ptr_file);


struct proc_ops props = 
{
    .proc_read = myproc_read,
    .proc_write = myproc_write,
    .proc_open = myproc_open,
    .proc_release = myproc_close
};


static struct proc_dir_entry* proc_ptr;
char echo[BUF_LEN];

int init_module(void)
{
    printk(KERN_INFO "Start work\n");
    proc_ptr = proc_create(PROCFS_NAME, RULES_OF_USE, NULL, &props);
    if (proc_ptr == NULL)
    {
        printk(KERN_INFO "Can't create proc\n");
        return 0;
    }
    return 0;
}


void cleanup_module(void)
{
    proc_remove(proc_ptr);
    printk(KERN_INFO "Finish work\n");
}


static ssize_t myproc_read(
    struct file* ptr_file, char* user_buff, 
    size_t length, loff_t* offp )
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


static ssize_t myproc_write(
    struct file* ptr_file, const char* user_buff, 
    size_t length, loff_t* offp )
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


static int myproc_open(struct inode* incode, struct file* ptr_file)
{
    ptr_file->private_data = echo;

    printk(KERN_INFO "Open proc\n");
    return 0;
}


static int myproc_close(struct inode* incode, struct file* ptr_file)
{
    char* echo = ((char*)ptr_file->private_data);

    printk(KERN_INFO "Close device STORED = %s\n", echo);
    return 0;
}


MODULE_LICENSE("GPL");
