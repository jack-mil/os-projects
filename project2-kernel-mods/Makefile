CFLAGS= -std=gnu99 -Wno-declaration-after-statement -O2 -save-temps

obj-m += simple.o

all:
	make "KCFLAGS=$(CFLAGS)" -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean
