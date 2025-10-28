CROSS   ?= aarch64-none-elf
CC      := $(CROSS)-gcc
LD      := $(CROSS)-ld
OBJCOPY := $(CROSS)-objcopy
CFLAGS  := -nostdlib
LDFLAGS := -T linker.ld

all: kernel.elf kernel.bin

asm.o: arch/arm64/kernel/head.S
	$(CC) $(CFLAGS) -c $< -o $@

main.o: init/main.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: asm.o main.o linker.ld
	$(LD) $(LDFLAGS) asm.o main.o -o $@

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

clean:
	rm -f *.o *.elf *.bin

run: kernel.bin
	qemu-system-aarch64 -M virt -cpu cortex-a57 -m 2G -nographic -device loader,file=kernel.bin,addr=0x80000000,cpu-num=0

format:
	@git ls-files '*.c' '*.h' | xargs -r clang-format -i

format-check:
	@git ls-files '*.c' '*.h' | xargs -r clang-format --dry-run -Werror