TOOLCHAIN = arm-none-eabi

CC = $(TOOLCHAIN)-gcc			# ARM cross compiler
OBJCOPY = $(TOOLCHAIN)-objcopy	# get raw binary (not elf)
GDB = $(TOOLCHAIN)-gdb

CPU = cortex-a7

CFLAGS = -mcpu=$(CPU) -fpic -ffreestanding
CSRCFLAGS = -O2 -Wall -Wextra
LFLAGS = -ffreestanding -O2 -nostdlib

KERN_HEAD = ../include
KERN_SRC = ../src/kernel
COMMON_SRC = ../src/common
OBJ_DIR = objects

KSOURCES = $(wildcard $(KERN_SRC)/*.c)
CSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASOURCES = $(wildcard $(KERN_SRC)/*.s)

OBJECTS  = $(patsubst $(KERN_SRC)/%.c, $(OBJ_DIR)/%.o, $(KSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(CSOURCES))
OBJECTS += $(patsubst $(KERN_SRC)/%.s, $(OBJ_DIR)/%.o, $(ASOURCES))

HEADERS = $(wildcard $(KERN_HEAD)/*.h)

IMG = proteus

.PHONY: build clean gdbinit

build: $(OBJECTS) $(HEADERS)
	$(CC) -T linker.ld -o $(IMG).elf $(LFLAGS) $(OBJECTS)
	$(OBJCOPY) $(IMG).elf -O binary $(IMG).img

$(OBJ_DIR)/%.o: $(KERN_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KERN_SRC) -I$(KERN_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KERN_SRC)/%.s
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KERN_SRC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KERN_SRC) -I$(KERN_HEAD) -c $< -o $@ $(CSRCFLAGS)

clean:
	rm -rf $(OBJ_DIR)
	rm $(IMG).elf
	rm $(IMG).img

run: build
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel $(IMG).elf

dbginit:
	echo "target remote loalhost:1234" > .gdbinit
	echo "break kernel_main" > .gdbinit

dbg:
	$(GDB) $(IMG).elf

dbgrun: build gdbinit
	qemu-system-arm -m 256 -no-reboot -M raspi2 -serial stdio -kernel $(IMG).elf -S -s
