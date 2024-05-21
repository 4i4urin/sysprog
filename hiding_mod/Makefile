obj-m := hiding_mod.o

PWD := $(CURDIR)
CC = gcc
CFLAGS = -Wall -Wextra

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) $(CFLAGS) -o just_proc just_proc.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm just_proc
