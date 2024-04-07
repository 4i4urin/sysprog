#include <linux/module.h>
#include <linux/printk.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define SYS_NAME "first_sys"
#define BUF_LEN 1024


static struct kobject* p_kobj;
char sysfs_buf[BUF_LEN];


static ssize_t sysfs_read(
    struct kobject *kobj, struct kobj_attribute *attr, char* user_buf);

static ssize_t sysfs_write(
    struct kobject *kobj, struct kobj_attribute *attr, 
    const char __user * user_buf, size_t length);

static struct kobj_attribute sysfs_attribute = 
    __ATTR( my_sysfs_file, 0664, sysfs_read, sysfs_write);



int init_module(void)
{
    printk(KERN_INFO "Start work\n");

    p_kobj = kobject_create_and_add(SYS_NAME, kernel_kobj);
    if (p_kobj == NULL)
    {
        printk(KERN_WARNING "Can't create kobj\n");
        return 0;
    }

    if (sysfs_create_file(p_kobj, &sysfs_attribute.attr))
    {
        printk(KERN_WARNING "Faild to create file\n");
        return 0;
    }
    return 0;
}


void cleanup_module(void)
{
    kobject_put(p_kobj);
    printk(KERN_INFO "Finish work\n");
}


static ssize_t sysfs_read(
    struct kobject *kobj, struct kobj_attribute *attr, char* user_buf)
{
    int bytes_read = 0;

    printk(KERN_INFO "Try read\n");

    bytes_read = sprintf(user_buf, "%s", sysfs_buf);
    sysfs_buf[0] = '\0';

    printk(KERN_INFO "DEV read %d bytes\n", bytes_read);
    
    return bytes_read;
}


static ssize_t sysfs_write(
    struct kobject *kobj, struct kobj_attribute *attr, 
    const char __user * user_buf, size_t length)
{
    int prev_len = strlen(sysfs_buf);

    printk(KERN_INFO "Try write\n");
    printk(KERN_INFO "STR: %s len = %ld\n", user_buf, length);
    if ( !prev_len )
    {
        if (length >= BUF_LEN - 1)
            length = BUF_LEN - 1;
    
        strcpy(sysfs_buf, user_buf);
    }
    else if (prev_len < BUF_LEN - 1)
    {
        if (length >= (BUF_LEN - prev_len))
            length = BUF_LEN - prev_len;

        strcpy(sysfs_buf + prev_len, user_buf);
    } else
    {
        printk(KERN_WARNING "WARNING: Devise sysfs_buf is full\n");
        return length;
    }

    sysfs_buf[ prev_len + length ] = '\0';
    printk(KERN_INFO "DEV write %ld bytes\n", strlen(sysfs_buf));

    return length;
}


MODULE_LICENSE("GPL");
