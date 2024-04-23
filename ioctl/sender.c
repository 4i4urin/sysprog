#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define USER

#include "ioctl.h"

#define EXIT_CODE 0xFFFFFFFF
#define ERROR_CODE 0xFFFFFFF0

unsigned int get_user_commad(void);
void clear_stdin(void);
void get_replase_vals(t_replet* replet);


int main(void)
{
    int char_dev = open("/dev/ioctl_test", O_RDWR);
    unsigned int command;

    t_seldel seldel_val;
    t_replet replet_val;
    t_addstr addstr_val;

    int iocttl_res = 0;

    while (1)
    {
        printf("print command 'v', 'd', 'r', 'a', 's', 'q'> ");
        command = get_user_commad();
        switch (command)
        {
        case REVERSE:
            iocttl_res = ioctl(char_dev, REVERSE);
            puts("Send reverse buffer command");
            break;

        case SELECT_DELETE:
            printf("Print string to delete > ");
            fgets(seldel_val, sizeof(seldel_val), stdin);
            clear_stdin();
            iocttl_res = ioctl(char_dev, SELECT_DELETE, &seldel_val);
            puts("Send delete string command");
            break;

        case REPLACE:
            get_replase_vals(&replet_val);
            iocttl_res = ioctl(char_dev, REPLACE, &replet_val);
            puts("Send replase letter command");
            break;

        case ADD_STR:
            printf("Print string to add from begining > ");
            fgets(addstr_val, sizeof(addstr_val), stdin);
            iocttl_res = ioctl(char_dev, ADD_STR, &addstr_val);
            puts("Send add string command");
            break;

        case ADD_SIZE:
            iocttl_res = ioctl(char_dev, ADD_SIZE);
            puts("Send insert strings size command");
            break;

        case EXIT_CODE:
            close(char_dev);
            return 0;

        default:
            puts("Unexpected command");
            continue;
        }
        if (iocttl_res < 0)
            puts("ERROR: Can't send to device");
        else
            puts("Successful");
    }
    close(char_dev);
    return 0;
}


void get_replase_vals(t_replet* replet)
{
    printf("print letter > ");
    replet->find = getchar(); 
    clear_stdin();
    printf("print replacement > ");
    replet->insert = getchar(); 
    clear_stdin();
}
    

unsigned int get_user_commad(void)
{
    char ch = getchar();
    unsigned int ret = 0;
    switch (ch)
    {
        case 'v': ret = REVERSE; break;
        case 'd': ret = SELECT_DELETE; break;
        case 'r': ret = REPLACE; break;
        case 'a': ret = ADD_STR; break;
        case 's': ret = ADD_SIZE; break;
        case 'q': ret = EXIT_CODE; break;
        default: 
            ret = ERROR_CODE;
    }
    clear_stdin();
    return ret;
}


void clear_stdin(void)
{
    char ch = getchar();
    if (ch == '\n') 
        return;

    while ( (ch = getchar()) != '\n' ) // read everything till \n
    {
        if (ch == EOF)
        {
            clearerr(stdin);
            break;
        }
    }
}

