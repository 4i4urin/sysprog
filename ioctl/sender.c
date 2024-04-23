#include <stdio.h>
#include <fcntl.h>

#define USER

#include "ioctl.h"


int main(void)
{
    int char_dev = open("/dev/ioctl_test", O_RDWR);
    int send[10] = {10, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    puts("Print smth\n");
    while (getchar() != '\n')
        ;
    if (ioctl(char_dev, REVERSE) < 0)
        printf("Some error\n");

    send[0] = 100;
    puts("Next\n");
    while (getchar() != '\n')
        ;
    ioctl(char_dev, SELECT_DELETE, send);

    send[0] = 1000;
    puts("Again\n");
    while (getchar() != '\n')
        ;
    ioctl(char_dev, CHANGE_LETTER, send);

    send[0] = 10000;
    puts("Last\n");
    while (getchar() != '\n')
        ;
    ioctl(char_dev, ADD_STR, send);

    send[0] = 100000;
    puts("Last\n");
    while (getchar() != '\n')
        ;
    ioctl(char_dev, ADD_SIZE, send);
    return 0;
}