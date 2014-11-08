# Uncomment this for cross-compilation.
ARMLINUX 	= arm-linux-

CC		= $(ARMLINUX)gcc
OBJCOPY = $(ARMLINUX)objcopy
CFLAGS  = -O2 -ffixed-r8 -ffreestanding -Wall -Werror -nostdinc \
          -I ../uboot/include -I include
ASFLAGS = -nostdinc -I ../uboot/include -I include
LDFLAGS = -nostdlib -Wl,-N,-Ttext,$(LOAD_ADDR)

LOAD_ADDR = 0xa3000000

STUBS = ../uboot/stubs.o

# Add any new files here.
objects = globals.o swi_handler.o user_setup.o exit_handler.o kernel.o

default: kernel.bin

.PHONY: default clean clobber

kernel.bin: kernel
	$(OBJCOPY) -O binary $< $@

kernel: start.o $(objects)
	$(CC) $(LDFLAGS) -o $@ $^ $(STUBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f kernel start.o $(objects)

clobber: clean
	rm -f kernel.bin
