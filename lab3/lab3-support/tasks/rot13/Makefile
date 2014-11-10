CC      = gcc
OBJCOPY = objcopy
CFLAGS  = -O2 -ffreestanding -Wall -Werror -nostdinc -I ../../part1/libc/include
LDFLAGS = -nostdlib -Wl,-Ttext,$(LOAD_ADDR) 

LOAD_ADDR = 0xa2000000

CRT0    = ../../part1/libc/crt0.o
LDLIBS  = ../../part1/libc/libc.a

objects = rot13.o

default: rot13.bin

.PHONY: default clean clobber

rot13.bin: rot13
	$(OBJCOPY) -O binary $< $@

rot13: $(objects)
	$(CC) $(LDFLAGS) -o $@ $(CRT0) $^ $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f rot13 $(objects)

clobber: clean
	rm -f rot13.bin
