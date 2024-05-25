#include <stdio.h>
#include <unistd.h>


int main(void)
{
    printf("%d\n", getpid());
    double a = 1;
    while (1) { 
        a *= 2;
    }

    return 0;
}
