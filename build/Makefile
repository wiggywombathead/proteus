TOOLCHAIN ?= arm-none-eabi

# ARM cross compiler
CC := $(TOOLCHAIN)-gcc

# get raw binary (not elf)
OBJCOPY := $(TOOLCHAIN)-objcopy

# get listing file
OBJDUMP := $(TOOLCHAIN)-objdump

LD := $(TOOLCHAIN)-ld
GDB := $(TOOLCHAIN)-gdb

# set model
ifeq ($(model),1)
	CPU = arm1176jzf-s
	ARCHDIR = model1
	DIRECTIVES = -DRPIBPLUS
else ifeq ($(model),2)
	CPU = cortex-a7
	ARCHDIR = model2
	DIRECTIVES = -DRPI2
endif

CPU ?= arm1176jzf-s
ARCHDIR ?= model1
DIRECTIVES ?= -DRPIBPLUS

# set scheduler
ifeq ($(sched),fcfs)
	DIRECTIVES += -DSCHED_FCFS
else ifeq ($(sched),robin)
	DIRECTIVES += -DSCHED_ROUNDROBIN
endif

DOC = ../doc
KERN_SRC = ../src/kernel
COMMON_SRC = ../src/common

# include directories
INC = ../include
INC += ../reference/uspi/include
INC += ../reference/uspi/env/include
#INC += ../reference/csud/include
INC := $(patsubst %,-I%,$(INC))

LIB = ../lib
OBJ_DIR = objects

LIBRARIES := $(wildcard $(LIB)/*.a)

CFLAGS = -mcpu=$(CPU) -mfloat-abi=hard -mfpu=vfp -ffreestanding -g $(DIRECTIVES)
CSRCFLAGS = -O2 -Wall -Wextra

#LLIBS = -L$(LIB) $(patsubst $(LIB)/lib%.a,-l%,$(LIBRARIES))
#LFLAGS = $(LLIBS) -ffreestanding -nostdlib -O2 

LDFLAGS = -nostdlib -L$(LIB)
#LDLIBS = $(patsubst $(LIB)/lib%.a,-l%,$(LIBRARIES))
LDLIBS := -luspi

KSOURCES = $(wildcard $(KERN_SRC)/*.c)
KSOURCES += $(wildcard $(KERN_SRC)/$(ARCHDIR)/*.c)

CSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASOURCES = $(wildcard $(KERN_SRC)/*.S)

OBJECTS  = $(patsubst $(KERN_SRC)/%.c, $(OBJ_DIR)/%.o, $(KSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(CSOURCES))
OBJECTS += $(patsubst $(KERN_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASOURCES))

HEADERS = $(wildcard $(INC)/*.h)

LINKER = linker.ld

NAME = bloom
ELF = $(NAME).elf
IMG = $(NAME).img
MAP = $(NAME).map
LIST = $(NAME).list

.PHONY: build clean gdbinit

############
# ORIGINAL #
############
build: $(OBJECTS) $(HEADERS)
	@echo "Building $(IMG)"
	$(LD) -T $(LINKER) -o $(ELF) -Map $(MAP) $(OBJECTS) $(LDFLAGS) $(LDLIBS)
	$(OBJCOPY) $(ELF) -O binary $(IMG)
	$(OBJDUMP) -D $(ELF) > $(LIST)

#$(CC) -T $(LINKER) -o $(ELF) $(OBJECTS) $(LFLAGS)

$(OBJ_DIR)/%.o: $(KERN_SRC)/%.c
	@echo "Compiling $<"
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(CSRCFLAGS)

#$(CC) $(CFLAGS) -I$(KERN_SRC) -I$(INC) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KERN_SRC)/$(ARCHDIR)/%.c
	@echo "Compiling $<"
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(CSRCFLAGS)

#$(CC) $(CFLAGS) -I$(KERN_SRC) -I$(INC) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KERN_SRC)/%.S
	@echo "Compiling $<"
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	@echo "Compiling $<"
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(CSRCFLAGS)

#$(CC) $(CFLAGS) -I$(KERN_SRC) -I$(INC) -c $< -o $@ $(CSRCFLAGS)

documentation:
	cd $(DOC) && doxygen doxygen.config

tidy:
	@echo "Tidying up..."
	rm -rf $(OBJ_DIR)
	rm -f $(ELF)

clean:
	@echo "Removing $(OBJ_DIR), $(IMG), $(ELF)"
	rm -rf $(OBJ_DIR)
	rm -f $(ELF) $(IMG) $(MAP) $(LIST)

$(vb).SILENT:

run: build
	qemu-system-arm -m 512 -M raspi2 -serial stdio -kernel $(ELF)

#######################
# PERHAPS GET WORKING #
#######################
#all: $(IMG) $(LIST)
#
## make listing file
#$(LIST): $(ELF)
#	$(OBJDUMP) -d $(ELF) > $(LIST)
#
## make img file
#$(IMG): $(ELF)
#	$(OBJCOPY) $(ELF) -O binary $(IMG)
#
#$(ELF): $(OBJECTS) $(LINKER)
#	$(LD) $(OBJECTS) -L$(LIB) $(LFLAGS) -Map $(MAP) -o $(ELF) -T $(LINKER)

dbginit:
	echo "target remote localhost:1234" > .gdbinit
	echo "break kernel_main" > .gdbinit

dbg:
	$(GDB) $(IMG).elf

dbgrun: build gdbinit
	qemu-system-arm -m 256 -no-reboot -M raspi2 -serial stdio -kernel $(IMG).elf -S -s
