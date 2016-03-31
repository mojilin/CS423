<<<<<<< HEAD
EXTRA_CFLAGS +=
APP_EXTRA_FLAGS:= -O2 -ansi -pedantic
KERNEL_SRC:= /lib/modules/$(shell uname -r)/build
SUBDIR= $(PWD)
GCC:=gcc
RM:=rm

.PHONY : clean

all: clean modules app

obj-m:= mp3.o

modules:
	$(MAKE) -C $(KERNEL_SRC) M=$(SUBDIR) modules

app: userapp.c userapp.h
	$(GCC) -g -o userapp userapp.c
	sudo rmmod mp3
	sudo insmod mp3.ko

clean:
	$(RM) -f userapp *~ *.ko *.o *.mod.c Module.symvers modules.order
=======
EXTRA_CFLAGS +=
APP_EXTRA_FLAGS:= -O2 -ansi -pedantic
KERNEL_SRC:= /lib/modules/$(shell uname -r)/build
SUBDIR= $(PWD)
GCC:=gcc
RM:=rm

.PHONY : clean

all: clean modules app

obj-m:= mp3.o

modules:
	$(MAKE) -C $(KERNEL_SRC) M=$(SUBDIR) modules

app: userapp.c userapp.h
	$(GCC) -g -o userapp userapp.c
	sudo rmmod mp3
	sudo insmod mp3.ko

clean:
	$(RM) -f userapp *~ *.ko *.o *.mod.c Module.symvers modules.order
>>>>>>> 692f7e2acf6268f8ff1bb26db15eaa30ee331072
