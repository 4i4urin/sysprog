obj-m := my_ioctl.o
my_ioctl-objs += buf_operations.o ioctl.o

PWD := $(CURDIR)

CC = gcc
CFLAGS = -Wall -Wextra

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) $(CFLAGS) -o sender sender.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm sender
