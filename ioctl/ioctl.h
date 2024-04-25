#ifndef IOCTL_H
#define IOCTL_H

#ifdef USER
    #include <sys/ioctl.h>
#else
    #include <linux/ioctl.h>
#endif /* USER */


#define MY_MAJOR        30
#define DEVICE_NAME     "ioctl_test"

#define BUF_LEN 1024


#define SEARCH_STR_SIZE     10
#define INSERT_STR_SIZE     20

typedef char t_seldel[SEARCH_STR_SIZE];
typedef struct _t_chlet
{
    char find;
    char insert;  
} t_replet;
typedef char t_addstr[INSERT_STR_SIZE];


#define REVERSE         _IO('i', 1)
#define SELECT_DELETE   _IOR('o', 2, t_seldel)
#define REPLACE         _IOW('c', 3, t_replet)
#define ADD_STR         _IOWR('t', 4, t_addstr)
#define ADD_SIZE        _IOC_SIZE(5)


#endif /* IOCTL_H */

