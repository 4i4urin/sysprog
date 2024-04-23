#include "buf_operations.h"

#include <linux/module.h>
#include <linux/uaccess.h>


#include "ioctl.h"


int reverse_buf(char* buf)
{
    return 0;
}


int select_delete(char* buf, unsigned long __user arg)
{
    t_seldel seldel_val;
    if ( copy_from_user(&seldel_val, (void*)arg, sizeof(seldel_val)) )
    {
        printk(KERN_WARNING "Can't copy from user select_delete");
        return 0;
    }
    printk(KERN_INFO "String to delete: %s\n", seldel_val);
    return 0;
}


int replace(char* buf, unsigned long __user arg)
{
    t_replet replet_val;
    if ( copy_from_user(&replet_val, (void*)arg, sizeof(replet_val)) )
    {
        printk(KERN_WARNING "Can't copy from user select_delete");
        return 0;
    }
    printk(KERN_INFO "letter: %c replace: %c\n", replet_val.find, replet_val.insert);
    return 0;
}


int add_str_begin(char* buf, unsigned long __user arg)
{
    t_addstr addstr_val;
    if ( copy_from_user(&addstr_val, (void*)arg, sizeof(addstr_val)) )
    {
        printk(KERN_WARNING "Can't copy from user select_delete");
        return 0;
    }
    printk(KERN_INFO "String to add: %s\n", addstr_val);
    return 0;
}


int add_str_size(char* buf)
{
    return 0;
}


MODULE_LICENSE("GPL");

