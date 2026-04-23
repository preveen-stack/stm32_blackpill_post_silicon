CC=arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -O0 -g -Wall -ffreestanding -Iinclude
LDFLAGS=-T linker.ld -nostdlib

SRCS = \
src/main.c \
src/uart.c \
tests/test_runner.c \
tests/test_sram.c

OBJS=$(SRCS:.c=.o)

all: firmware.elf

firmware.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

flash:
	st-flash write firmware.elf 0x8000000

clean:
	rm -f $(OBJS) firmware.elf
