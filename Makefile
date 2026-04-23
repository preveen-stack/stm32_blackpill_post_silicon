CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS  = -mcpu=cortex-m4 -mthumb -O0 -g -Wall -ffreestanding -Iinclude
ASFLAGS = -mcpu=cortex-m4 -mthumb
LDFLAGS = -T linker.ld -nostdlib -Wl,-Map=firmware.map

# Source files
C_SRCS = \
src/main.c \
src/uart.c \
src/pc13.c \
tests/test_runner.c \
tests/test_sram.c \
tests/test_hse.c \
tests/test_timer.c

ASM_SRCS = \
startup_stm32f411.s

# Object files
C_OBJS   = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.s=.o)

OBJS = $(C_OBJS) $(ASM_OBJS)

# Default target
all: firmware.bin

# Compile C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile ASM
%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

# Link
firmware.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# Convert to binary
firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

# Flash
flash: firmware.bin
	st-flash write firmware.bin 0x08000000

# Clean
clean:
	rm -f $(OBJS) firmware.elf firmware.bin firmware.map
