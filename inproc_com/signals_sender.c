#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define AUTOMATIC   1
#define SIGNAL      2
#define STOP        -1
#define IO_ERROR    -2


void send_signal(int pid, int sig_id);
void send_auto(int pid);
char get_user_commad(void);
int uint_from_str(const char* str);
void clear_stdin(void);
void delay(int millisec);


int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Write PID to command string arguments\n");
        return -1;
    }

    int pid = uint_from_str(argv[1]);
    if ( pid <= 0)
    {
        printf("ERROR: Unexpected PID\n");
        return -1;
    }

    char command = 0;
    int sig_id = 0;
    while (1)
    {
        printf("print command 'q', 'a', 's'> ");
        command = get_user_commad();
        switch (command)
        {
        case STOP: return 0; 
        case AUTOMATIC: send_auto(pid); break;
        case IO_ERROR: 
            printf("ERROR: Unknown input\n");
            break;
        case SIGNAL:
            scanf("%d", &sig_id);
            clear_stdin();
            send_signal(pid, sig_id);
            break;
        }
    }
    return 0;
}


void send_signal(int pid, int sig_id)
{
    if (sig_id > 31)
    {
        printf("ERROR: unknown signal id\n");
        return;
    }
    kill(pid, sig_id);
}


void send_auto(int pid)
{
    printf("send all signals except of SIGKILL and SIGSTOP\n");
    for (int i = 1; i < 32; i++)
    {
        if (i == 9 || i == 19)
            continue;
        kill(pid, i);
        delay(500);
    }
}


char get_user_commad(void)
{
    char ch = getchar();
    switch (ch)
    {
        case 'a': 
            clear_stdin(); 
            return AUTOMATIC;
        case 'q':
            clear_stdin(); 
            return STOP;
        case 's': 
            return SIGNAL;
        default: 
            clear_stdin(); 
            return IO_ERROR;
    }
}


int uint_from_str(const char* str)
{
    int ret_val = 0;
    long unsigned int i = 0;
    for ( ; i < strlen(str) && isdigit(str[i]); i++)
    {
        ret_val = ret_val * 10 + (str[i] - '0');
    }
    if (i < strlen(str))
        return -1;
    return ret_val;
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


void delay(int millisec)
{
    clock_t start_time = clock();
    while (clock() < start_time + millisec) ;
}
