#include <stdio.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	printf("%d\n", getpid());

	while (1) { }

	return 0;
}