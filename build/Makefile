TOOLCHAIN ?= arm-none-eabi

CC := $(TOOLCHAIN)-gcc			# ARM cross compiler
OBJCOPY := $(TOOLCHAIN)-objcopy	# get raw binary (not elf)
GDB := $(TOOLCHAIN)-gdb

# set constants based on pi model
ifeq ($(model),1)
	CPU = arm1176jzf-s
	ARCHDIR = model1
	DIRECTIVES = -DRPIBPLUS
else ifeq ($(model),2)
	CPU = cortex-a7
	ARCHDIR = model2
	DIRECTIVES = -DRPI2
else
	CPU = arm1176jzf-s
	ARCHDIR = model1
	DIRECTIVES = -DRPIBPLUS
endif

KERN_HEAD = ../include
KERN_SRC = ../src/kernel
COMMON_SRC = ../src/common
LIBS = ../libs
OBJ_DIR = objects

LIBRARIES := $(wildcard $(LIBS)/*.a)

CFLAGS = -mcpu=$(CPU) -fpic -ffreestanding -g $(DIRECTIVES)
CSRCFLAGS = -O2 -Wall -Wextra
LFLAGS = -ffreestanding -O2 -nostdlib -lgcc $(patsubst $(LIBS)/lib%.a,-l%,$(LIBRARIES))

KSOURCES = $(wildcard $(KERN_SRC)/*.c)
KSOURCES += $(wildcard $(KERN_SRC)/$(ARCHDIR)/*.c)

CSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASOURCES = $(wildcard $(KERN_SRC)/*.S)

OBJECTS  = $(patsubst $(KERN_SRC)/%.c, $(OBJ_DIR)/%.o, $(KSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(CSOURCES))
OBJECTS += $(patsubst $(KERN_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASOURCES))

HEADERS = $(wildcard $(KERN_HEAD)/*.h)

IMG = proteus

.PHONY: build clean gdbinit

build: $(OBJECTS) $(HEADERS)
	@echo "Building $(IMG)"
	$(CC) -T linker.ld -o $(IMG).elf $(OBJECTS) -L$(LIBS) $(LFLAGS) 
	$(OBJCOPY) $(IMG).elf -O binary $(IMG).img

$(OBJ_DIR)/%.o: $(KERN_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KERN_SRC) -I$(KERN_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KERN_SRC)/$(ARCHDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KERN_SRC) -I$(KERN_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KERN_SRC)/%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KERN_SRC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KERN_SRC) -I$(KERN_HEAD) -c $< -o $@ $(CSRCFLAGS)

clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR)
	rm $(IMG).elf
	rm $(IMG).img

run: build
	qemu-system-arm -m 512 -M raspi2 -serial stdio -kernel $(IMG).elf

dbginit:
	echo "target remote localhost:1234" > .gdbinit
	echo "break kernel_main" > .gdbinit

dbg:
	$(GDB) $(IMG).elf

dbgrun: build gdbinit
	qemu-system-arm -m 256 -no-reboot -M raspi2 -serial stdio -kernel $(IMG).elf -S -s
