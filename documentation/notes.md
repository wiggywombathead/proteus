# OS notes

## Hardware
**Raspberry Pi 2 Model B** - uses Broadcom 2836 chip; its architecture is
identical to BCM2835 chip, except the ARM1176JZF-S processor is replaced with
the ARM Cortex-A7 processor

## Development environment
Developed on x86\_64 Linux 4.16.13-2-ARCH onwards. As the target environment is
a Raspberry Pi, which uses an ARM processor, there is the need to set up
cross-compiler to target this architecture. In particular, GCC provides
cross-compiler for arm-none-eabi, which provides a toolchain to target System V
ABI.

#### Note about System V ABI
System V ABI (Application Binary Interface) is a set of specifications that
detail calling conventions, object file formats, executable file formats,
dynamic linking semantics, and more for systems complying with the System V
Interface Definition. For example, it defines the Executable and Linkable Format
(ELF), a format for storing programs or fragments of programs on disk, created
as a result of compiling and linking. It is divided into sections, in particular
a `.text`, `.data`, `.rodata`, and `.bss` section (all discussed later), along
with `.comment`, `.note`, `.stab`, and `.stabstr` sections for compiler/linker
toolchain comments and debugging information. The calling convention details for
the Raspberry Pi 2's 32-bit Cortex A7 processor are as follows:

* 15 general purpose data storage (r0-r14)
* r15 - Program Counter
* r14 - Link Register
* r13 - Stack Pointer
* r12 - Intra-Procedure-Call Scratch Register
* r4 to r11 - used to hold local variables
* r0 to r3 - used to hold arguments passed to functions, and the returned result
* CPSR - Current Program Status Register
* SPSR - Saved Program Status Register

Subroutines must preserve the contents of r4 to r11 and the stack pointer. In
particular, subroutines that call other subroutines *must* save the return
address in the link register r14 to the stack before calling those other
subroutines. However, such subroutines do not need to return that value to
r14—they merely need to load that value into r15, the program counter, to
return.

It asserts a full descending stack:
* full: SP points to topmost item in stack, i.e. the location of the last item
  pushed (as opposed to empty: pointing to the next free location, the address
  where the next item will be stored)
* descending: stack grows downwards, i.e. the SP starts from a high memory
  address and progresses to lower memory addresses as items are pushed.

## First steps: minimal kernel 
### boot.s
```assembly
.section ".text.boot"
.global _start
```
This tells where the code is located in the compiled binary and makes `_start`
global i.e. visible outside of the file

```assembly
_start:
    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #0
    bne halt

    ...

halt:
    wfe
    b halt
```
`and Rn, Rm, <val>` - translates to Rn := Rm AND \<val\> \
Then compare if r1 == 0. \
This sends three of the four processors on board the chip to `halt`, where the
processor is put into a low-power state while it waits for an event. Multi-core
OS programming is presumably harder.

#### Note about the `mrc` instruction
Purpose is to transfer coprocessor-generated condition code flag values to the
ARM processor. In particular, `mrc p15, #0, <Rd>, c0, c0, #5` reads the MPIDR
(Multiprocessor Affinity Register), and stores it in register `Rd`.
The general syntax is `mrc pXX, op1, Rd, CRn, CRm {, op2}`, which executes some
7-bit opcode `op1` on register `pXX`, using coprocessor registers `CRn` and
`CRm` to pass operands, with an optional 7-bit opcode `op2`, and stores the
result in `Rd`. The exact code is coprocessor specific, and in this case was
found in the Cortex-A7 MPCore Technical Reference Manual.

Coprocessor in ARM is a misleading notion. It's shorthand for an optional piece
of functionality that is not exposed via the core instruction set. ARM CPUs are
modular. There are bits and pieces of CPU hardware that implementers of the
architecture may or may not place on the chip. The memory management unit (MMU)
is one example; there are others, such as the hardware debugging facility. Those
are, indeed, identified by coprocessor number (pXX), so that more than one
coprocessor can be present at the same time. The coprocessor number for MMU is
traditionally p15. Coprocessors p0..p14 have nothing to do with memory
management and may not be present. The debugging subsystem, for example, is p14.

```assembly
mov sp, #0x8000
```
Setup stack pointer (r13). Program Counter (r15) for kernel starts at 0x8000 and
grows upwards. Therefore our stack can safely start at 0x8000 without
interfering with the kernel. Note that the stack grows downwards.

```assembly
ldr r4, =__bss_start
ldr r9, =__bss_end
```
Loads start and end of BSS (Basic Service Set) into registers. This is where
statically allocated global variables that are not initialized explicitly to a
particular values are stored. The C runtime environment requires uninitialized
global variables to be zeroed, done by moving the value 0 to registers 5 to 8.

```assembly
b 2f

1:
    stmia r4!, {r5-r8}

2:
    cmp r4, r9 blo 1b
```
Check that address stored at r4 is less than r9. If so, the `N` flag is set and
so the line `blo 1b` is executed. This uses Local Labels to search **b**ackwards
for label 1 (previously we branched to label 2 by looking **f**orwards). The
`stm` instruction (**ST**ore **M**ultiple) stores the list of registers (the
four zeroed registers 5 to 8 inclusive) into the address of register 4. The `ia`
suffix, for **I**ncrement **A**fter, means to increment the address in register
4 to the address after the last address written by the instruction. The `!`
means we store the result of the instruction into register 4, rather than
discarding it. Overall, this block stores 16 bytes of zeroes into the address of
register 4, then increments that address. This loops until the address of
register 4 is greater than or equal to that of register 9, by which time the
entire BSS is zeroed. 

```assembly
ldr r3, =kernel_main
blx r3
```
Finally, we load the address of the function `kernel_main` into register 3, and
transfer execution to this (the address of the next instruction is stored in the
Link Register (r14), address of `kernel_main` is stored in PC).

Overall, this code initialises a minimum C environment, meaning we initialise
the stack and zero out the BSS segment before calling the `kernel_main`
function. We avoid using registers 0, 1, and 2 as they are used in the
`kernel_main` call to pass parameters and hardware information at runtime.

#### Note about freestanding vs. hosted environments
Typical programs are written to run in a hosted environment, meaning it has
access to the C standard library and other useful runtime features. By contrast,
in OS dev, we use a freestanding environment (as we must create our own standard
library to run on our operating system), meaning there is no C standard library,
and may only use what we supply ourselves. Some header files are not part of the
C standard library, but rather the compiler, meaning they are available in a
freestanding environment such as this. In particular, headers available (minus a
few special purpose ones from GCC) are: `<stddef.h>` `<stdint.h>`, `<float.h>`,
`<limits.h>`, `<stdarg.h>`, and`<iso646.h>`. Note that these only supply us with
some macros, and we still have to define all function we wish to use ourselves.

### uart.c
This sets up the hardware for basic I/O. For now, this is done using UART
(Universal Asynchronous Receiver Transmitter), meaning text data is sent and
received through serial ports. To do this in the final version requires a
USB-to-TTL cable, but we can emulate this using the virtual machine for now. In
this we use Memory Mapped I/O - the process of performing I/O by reading from
and writing to predefined memory addresses. 

A peripheral is a device with a specific address from and to which it may read
and write data. All peripherals may be described by an offset from the
Peripheral Base Address, which begins at address 0x3f000000 on the Raspberry Pi
2.

A register is a 32-bit chunk of memory that a peripheral may read from or write
to: these are at predefined offsets from the base address. A full list of all
UART registers on the BCM2835/6 chip can be found on page 177 of the BCM2835 ARM
Peripherals Manual. This also gives the UART base address of 0x7e201000, however
on the Raspberry Pi 2, this is at 0x3f201000 instead. **N.B.** From the manual:
"Physical addresses range from 0x20000000 to 0x20ffffff for peripherals. The
bus addresses for peripherals are set up to map onto the peripheral bus address
range starting at 0x7e000000. Thus a peripheral listed at 0x7ennnnnn will be
available at physical address 0x20nnnnnn."

`mmio_write()` takes a 32-bit register and stores the 32-bit data into the
register specified, while `mmio_read()` returns the data stored in that
register. The `delay()` function is used as a means of giving the hardware time
to respond to any writes we make.

`uart_init()` sets up the UART hardware for use, by setting some configuration
flags.

* `mmio_write(UART0_CR, 0x00000000)` disables all aspects of the UART hardware
(the Control Register)
* `mmio_write(GPPUD, 0x00000000)` disables all GPIO pins
* `mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15))` marks pins 14 and 15 for
disabling, and `mmio_write(GPPUDCLK0, 0x00000000)` makes our changes take
effect.
* `mmio_write(UART0_ICR, 0x7ff)` sets all flags in the Interrupt Clear Register,
clearing all pending interrupts.
* `mmio_write(UART0_IBRD, 1)` and `mmio_write(UART0_FBRD, 40)` set up the baud
rate of the connection, i.e. the bits that may be transmitted across the serial
port per second. To set this, we must calculate the baud rate divisor, using
IBRD = UART clock speed / (16 * desired baud rate) (page 183). UART clock
speed = 3 MHz, desired baud rate = 115200. Since this formula does likely not
yield a whole number, we then use FBRD = (fractional part * 64) + 0.5
* `mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6))` sets bits 4, 5, and 6
of the Line Control Register. This enables FIFOs, and sets the word length (the
number of data bits to be transmitted and received in a frame) to 8 bits, from
page 184 of the Peripherals Manual.  `mmio_write(UART0_IMSC, (1 << 1) | (1 << 4)
| (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10))` - disables
all interrupts by writing to the relevant bits in the Interrupt Mask Set Clear
register.
* `mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9))` enables UART hardware,
as well as the ability to receive (bit 8) and transmit (bit 9) data.

`uart_putc()` waits until the FIFO is no longer full (which we can tell by
looking at bit 5 of the Flags register, FR), and then writes `c` to the Data
Register, DR. `uart_getc()` waits until the FIFO is no longer empty, then
returns whatever is in the Data Register. `uart_puts()` just incorporates a loop
so that we may output whole strings at a time.

### kernel.c
Control is transferred to here from boot.s by the line `ldr r3, =kernel_main`.
All this does is initialises the UART, prints a welcome string, and waits to
receive and print some text from the keyboard. Notice the parameters for
`kernel_main`: the convention in ARM is to pass the first three parameters of a
function through registers 0, 1, and 2. The bootloader passes arguments to the
kernel via registers 0, 1, and 2, and boot.s makes sure to preserve these
registers (no line of code touches them). Register 0 contains a code for the
device from which the Raspberry Pi was booted (usually 0, but dependent on the
board firmware). Register 1 contains the 'ARM Linux Machine Type', which for the
Raspberry Pi 2 is 0x12dc, identifying the BCM2835 CPU (found [here]
(http://www.arm.linux.org.uk/developer/machines/)). When the bootloader loads
the kernel, it also passes some information about the hardware in order to run
it, called 'atags'. A pointer to this information is passed in register 2 just
before boot.s runs. Since register 2 contains the atags pointer, the third
argument to `kernel_main` is the atags pointer.

### linker.ld
To create the full kernel we must link all compiled object files into one final
program. For user-space programs, there are default scripts which do this, but
we have to create one ourselves for our kernel.

C program sections:
* `.text` - executable code
* `.rodata` - read-only data (global constants)
* `.data` - global variables initialised at compile-time
* `.bss` - uninitialised global variables

**N.B:** In linker scripts, `.` stands for current address.

```assembly
ENTRY(_start)
```
This declares that the symbol `_start` from boot.s as the entry point of our
program.

Next we set symbols `__start` and `__text_start` to be 0x8000, which is where
the bootloader will put our kernel image. We then declare the `.text` section to
start directly after this. The code from boot.s is located in the first part of
this section, `.text.boot`. `KEEP` tells the linker to not try to optimise the
code in `.text.boot`. The second part of the `.text` section is all `.text`
sections from other objects, in any order.

Next, `ALIGN` is used to set the current address to the next available page (the
next largest address divisible by our page size, 4096). Then in much the same
way we declare the `.rodata`, `.data`, and `.bss` sections.

### Makefile
Only a few things to note. `CC = arm-none-eabi-gcc` uses the ARM cross-compiler
(I installed it from Arch Linux's package manager, pacman). Since we are
cross-compiling, we specify our target CPU, namely `cortex-a7` for the Raspberry
Pi 2.

* `-fpic` - generate position independent code, for use in a shared library
* `-ffreestanding` - compile for freestanding, as opposed to hosted, environment

For now, we use the target `run` to emulate the Raspberry Pi 2 on QEMU.

## Memory
Throughout the running of the operating system, processes and computations will
need to use memory. Since we are the kernel, we can use whatever memory we like.
To stop this all blowing up in our face, however, we organise memory into
equal-sized blocks called pages - these allow us to allocate blocks of memory
that are not insignificantly small, but not so large that they take up a
significant portion of memory.

### atags
In order to organise our memory, we need to know how much is available to us.
This can be done using atags
([reference](http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html#appendix_tag_reference)).

We match the layout of the atags by defining some types (none, core, mem, ...,
cmdline). Then we iterate over the atag list until we reach the `MEM` tag, in
`get_mem_size()`.

## Paging
Number of pages is total memory (from atags) divided by page size, which is 4kB.
Pages need metadata: the virtual address they map to, flags for if it is
allocated, if it is a kernel page, and reserved for when we enable virtual
memory.

To hold all of the metadata, we use a large portion of memory just after the
kernel image for an array of pages/metadata. This address (just after the kernel
img) is found using the `__end` symbol, as declared in the linker script. We
also create a linked list of pages to keep track of which are free.

### Allocating pages
To allocate a page, just find one which hasn't been allocated yet and return a
pointer to its memory. The memory of the page itself can be found by multiplying
the index in the page array by 4096.

To free a page, we get the index to free by dividing its address by 4096.

## Allocating memory on the heap
We can take memory directly after the page metadata and reserve it for the heap.
This amount to reserve is arbitrary: 1MB is large enough to be sufficient for
all dynamic memory needs, and small enough to not use a significant portion of
memory that user code may want.

### Implementing `kmalloc()`
Associate each allocation with a header, which will all form a linked list. It
includes the allocation size, and whether it is currently in use. To allocate,
we must find an allocation that is at least the number of requested bytes and not
currently in use. If the allocation being inspected is large relative to the
request, we can split it up (i.e. if allocation is >= 2 times the header -
avoids having too many allocations that are half header, half data).

### Implementing `kfree()`
Need to mark free'd segments as unallocated, and merge adjacent free segments
into one big one (avoid internal fragmentation). We do this with a while loop,
incrementally increasing the `segment_size` of the free'd segment, effectively
merging free segments to the right.

### Initialising the heap
We must reserve the pages we are going to use, put a header stating there is a
1MB unused allocation here, and assign the `heap_segment_list_head` to this
header. Finally, this must actually be initialised by calling `mem_init()` in
`kernel_main()`.

## Printing to a real screen
### Framebuffers
Framebuffer - a piece of memory that is shared between CPU and GPU. CPU writes
RGB pixels to the framebuffer, and GPU renders it to the output device
(whichever one is connected).

Depth of a framebuffer - number of bits in every pixel e.g. 1 byte for each
red, green, and blue value (0-255) means depth of 24.

Pitch of a frambuffer - number of bytes in each row on the screen.

We can calculate pixels per row = pitch / (depth / 8).
Pixel located at (x,y) = pitch * y + (depth / 8) * x.

To draw anything to the screen, we need a framebuffer, which we request from the
GPU. We do this using the mailbox peripheral. 

### The mailbox peripheral
The mailbox is a peripheral that facilitates communication between the CPU and
the GPU. It starts at offset 0xB880.

The read register is at offset 0x00 from the mailbox base, and allows for
reading messages from the GPU. The lower 4 bits specify the channel the message
is from, and the upper 28 bits are data.

The status register is located at offset 0x18. Bit 30 signifies whether the read
register is empty, while bit 31 signifies whether the write register is full.

The write register is at offset 0x20.

A channel is a number specifying to you and the GPU what the information being
sent through the GPU means. For these purposes we will only need channel 1, the
Frambuffer channel, and channel 8, the property channel.

#### Reading from the mailbox
To read from the mailbox:
- read the status register to ensure the read register is not empty
- read the contents of the read register
- check the channel of the read message: either do something with the
  information, or discard
- if the channel is correct, read the data

#### Writing to the mailbox
- in a loop, check status register to ensure write register is not full
- write the data to the write register

### The property channel
The property channel, mailbox channel 8, is how to ask the GPU for a
framebuffer. It provides a means to get and set data about various hardware
devices, one of which is the framebuffer.

#### Property channel messages
A message must be 16-byte aligned buffer of 4-byte words. The response
overwrites the original message.

A message begins with a 4 byte size of the message, plus 4 bytes for the size
itself. This is followed by a 4 byte request/response code. When sending a
message, this must be 0. When receiving, it will be either 0x80000000
(indicating success) or 0x80000001 (indicating error).

Following the request/response code comes a list of tags - commands and buffers
for their (the commands') responses. The very last tag must be an end tag, which
is 4 bytes of 0. Finally, the entire message must be padded to ensure 16 byte
alignment.

#### Message tags
A tag is a command to get or set data about the hardware. Each starts with a 4
byte tag id, which identifies the command to be run. In general, tags are of the
form 0x000XYZZZ, where X specifies the hardware device to access, Y the type of
command (0 - get, 4 - test, 8 - set), and ZZZ is the specific command. A tag
also stores the size of its value buffer in bytes, i.e. the maximum of the size
of the parameters the command takes, and the size of the results. The tag also
has its own request/response code. Sending => 0, while receiving => value is
0x80000000 + the length of the result. After this is the value buffer, where the
parameters and results are stored.

#### Getting a framebuffer
First we must set the screen size, virtual screen size, and depth. The tag ids
for these commands are 0x00048003, 0x00048004, and 0x00048005 respectively. 

To set the screen size requires a 4-byte width and a 4-byte height, with no
result returned. Thus, the value buffer has size 8 bytes for the screen size
commands. To set the depth requires a 4-byte value, and no result is returned,
meaning the value buffer has size 4 bytes. 

We then send this buffer through the mailbox - we must ensure the buffer is at a
16-byte aligned address, so only the high 28 bits contain the address. We then
bitwise OR with this address to set the channel number, and send this through
the mailbox. To verify this has worked, we check the request/response code of
the buffer; if this is 0, then it has failed as the GPU could not overwrite this
part with a response code. If 0x80000001, then error. If 0x80000000, then
success.

Now the screen parameters are set, we can request a framebuffer - the tag ID
for this command is 0x00040001. It takes a single 4-byte parameter, the
requested alignment of the framebuffer, and returns two 4-byte values, a pointer
to the buffer and the buffer size. 

More information on framebuffers can be found
[here](https://github.com/raspberrypi/firmware/wiki/Mailboxes).

[Font](https://github.com/dhepper/font8x8/blob/master/font8x8_basic.h)

## Loading on real hardware
Using `arm-none-eabi-objcopy`, we copy the kernel ELF file to a raw binary file.
Then, we must copy this image to the SD card from which the operating system
will boot. An easy way to do this is to install an operating system with an already
functioning boot partition, and replace the `.img` file with that built by the
makefile one. In this
case I just used the official Raspbian download from the Raspberry Pi website.
Then you can mount the SD card with Raspbian, rename `kernel7.img` to something
partition between the GPU and CPU. Here the kernel image is loaded, the CPU is
released from reset, and control is transferred to it to execute the kernel.

After the operating system is loaded, the code on the GPU is not unloaded;
instead, it runs its own simple operating system, called Video Core Operating
System (VCOS). The kernel can then use this to communicate with the services it
provides (e.g. providing a framebuffer, as above) using the Mailbox Peripheral
and interrupts (the GPU is able to produce ARM interrupts). The GPU is not only
in charge of graphical functions - it also controls clocks and audio, for
example. In this way the GPU firmware is similarly to a normal PC's BIOS (Basic
Input/Output System).

Sources:
[1](https://raspberrypi.stackexchange.com/questions/8475/what-bios-does-raspberry-pi-use)
[2](https://raspberrypi.stackexchange.com/questions/7122/level-of-hackability-of-raspberry-pi/7126#7126)

## Debugging real hardware
On the first attempt, printing out on real hardware did not work, despite
functioning well in the emulated environment provided by QEMU. In particular,
the screen displayed the "rainbow screen". As there is no real platform to debug
software at such a low level, the approach I took was to flash the ACT LED in
two locations in the boot code - firstly before `kernel_main` is called, to
check the setup prior to this is functioning correctly, then after `kernel_main`
is called, to ensure the kernel is being called and executed correctly.

[here](https://raspberrypi.stackexchange.com/questions/67431/cant-turn-on-act-led-on-baremetal-pi3)

The ACT LED is located on GPIO pin 47. This pin is pull-high, meaning we have to
set the pin to turn the LED on (on the Pi 1, the ACT LED was on pin 16, which
was pull-low, meaning it had to be cleared to be turned on). So we need to use
GPSET and GPCLR to turn the pin on and off respectively. But there are two
versions of each, namely GPSET0, GPSET1, GPCLR0, and GPCLR1. GPSET0 and GPCLR0
deal with register 0 to 31, so we need to use GPSET1 and GPCLR1.

Now the ACT blinks on and off at around a half second delay thanks to the
following assembly: TODO

[Massively helpful
reference](https://www.raspberrypi.org/forums/viewtopic.php?p=852703)

## Switched to Raspi 1
Could not resolve issue of displaying to HDMI on Pi 2, so switched to Pi 1 where
the process is much simpler. Had to include the `-lgcc` linking flag for some
reason, other compilation threw the error `undefined reference to
__aeabi_uidivmod`. Searching around, this is because 
    

> The ARM family of CPUs does not have a native integer division instruction. So,
> division needs to be implemented by a library function. GCC knows this, and
> creates a reference to (in your case) `__aeabi_uidiv` (for unsigned int division).
> 
> You will need to link with an appropriate runtime support library that contains this function.

[Source](https://stackoverflow.com/questions/6576517/what-is-the-cause-of-not-being-able-to-divide-numbers-in-gcc).
To resolve this, I linked with the `-lgcc` flag.

Okay now I had set everything up, including configuring which model to compile
for in the `Makefile`, but the ACT was not blinking to show the kernel
successfully booted as had been programmed on the Pi 2. Both the Pi 2 and the Pi
1 I purchased were B+ boards, meaning their GPIO layout was identical, so it
could not have been a problem with the code. I eventually found that assembly
files ending in `.s` do not have the preprocessor invoked before assembly,
while those ending in `.S` (capital S) do. This meant that the kernel wasn't
booting as it was trying to execute instructions which did not exist - these
were enclosed in a preprocessor `ifdef` which was being ignored as the file was
named `boot.s`. Changing this to `boot.S` and the appropriate references to it
in the `Makefile` fixed this and the ACT was now blinking successfully, showing
the kernel was booting.
[Source](https://stackoverflow.com/questions/6359293/is-it-possible-to-use-ifdef-like-checks-in-assembler).

## Interrupts and Exceptions
### Exceptions
Exception - event triggered when something exceptional occurs during normal
execution, e.g. hardware giving new data to CPU, user code asking to perform and
privileged action, or bad instruction

On Pi, when exception occurs, a specific address is loaded into Program Counter
and execution branches to this point. Branch instructions need to be written at
these locations to branch to correct exception handling routines. This set of
Address is the **Vector Table**, and starts at address `0x0`. The table is as
follows:

| Address | Exception | Source | Action |
|---------|-----------|--------|--------|
|`0x00` | Reset | Hardware reset | Restart kernel |
|`0x04` | Undefined instruction | Executing garbage instruction | Kill offender |
|`0x08` | Software Interrupt (SWI) | Software wants to execute privileged operation | Perform op and return to caller
|`0x0c` | Prefetch Abort | Bad memory access of instruction | Kill offender |
|`0x10` | Data Abort | Bad memory access of data | Kill offender |
|`0x14` | Reserved | Reserved | Reserved |
|`0x18` | Interrupt Request (IRQ) | Hardware telling CPU something | Determine hardware that triggered and respond appropriately |
|`0x1c` | Fast Interrupt Request (FIQ) | Piece of hardware can do this faster than all others | Determine device that triggered and respond appropriately |

### Interrupt Requests
IRQ is a notification to the CPU that something happened in the hardware that
the CPU should know about e.g. keypress, accessing privileged memory, receiving
network packet. To determine which device can trigger interrupts and which has
triggered one, we use the IRG peripheral, located at offset `0xb000` from the
peripheral base address. The peripheral has three types of registers - pending,
enable, and disable.
Pending - indicate whether a given interrupt has been triggered. Used to
determine which hardware device has triggered the IRQ exception.
Enable - enable certain interrupts to be triggered by setting the appropriate
bit.
Disable - disable certain interrupts by setting appropriate bit

Pi has 72 possible IRQs.
- 0 to 63 are shared by CPU and GPU
- 64 to 71 are specific to the CPU

Timer is IRQ 1, and USB controller is IRQ 9

Notes: when you see an interrupt is pending, do not clear the bit. Each
peripheral has its own mechanism for clearing the bit that should be done in
handler for that peripheral's IRQ. Clearing a bit in the enable register does
not disable the IRQ. An IRQ should only be disabled by setting the bit in the
disable register.

### Setting up the Exception Vector Table
Exception handlers are functions, but not normal ones. They need more advanced
prologue and epilogue code than usual. 

#### `vector_table.S`
Approach is to write each branch instruction and copy the instructions from the
`.text` section to address `0x0` at runtime. Notice we have

```assembly
    ldr pc, irq_handler_abs_addr
    ...
    irq_handler_abs_addr:
        .word irq_handler
```

as opposed to

```assembly
    ldr pc, irq_handler_abs_addr
```

This is because we use the `-fpic` flag during compilation, which creates
Position Independent Code. This means writing `ldr pc, irq_handler` would get
compiled to `ldr pc, [pc, #irq_handler_offset]`, loading the address relative to
its current position. Then when we move the instruction to address `0x0` the
handler would be at the same relative offset. So instead, we put the absolute
address of the handler in memory, and load this address relative to the current
position, address `0x0`.

`move_exception_vector` is a function called from `interrupts_init`. `push
{r4-r9}` saves registers four to nine, the ones which the function will be
using. C function expect these registers to be saved for them, so we save them
to restore them later. `ldr r0, =exception_vector` loads the address of the
exception vector in register 0. We `mov r1, #0x0000` as register 1 is the
destination address.

```assembly
    ldmia r0!, {r2-r9}
    stmia r1!, {r2-r9}
```
First, we store the data in register 0 in registers 2 to 9. `ia` means that
after storing the data in each register, we increment the address so that the
next register gets the next of memory. `!` means to store the address after the
last word of memory copied into register 0, so we may start from there again
later. So these instructions load the 8 exception words from their starting
location into registers. The next instructions do the same thing but in reverse,
writing the register values to address `0x0`. It copies the absolute addresses
to sit above the exception instructions in memory:

```assembly
    ldmia r0!, {r2-r8}
    stmia r1!, {r2-r8}
```

Then
```assembly
    pop {r4-r9}
    blx lr
```
restores the saved registers and returns control to the caller.

### IRQ handler wrapper
Exceptions cannot simply jump to normal functions - certain things must be dealt
with before and after the normal function is executed, which we can't assume
when using interrupt routines. By using `__attribute__((interrupt("FIQ")))`, we
embed a default version of the special prologue and epilogue directly into the
function. This is very minimal, and for our IRQ handler we need some
customisation.

Due to a quirk with how ARM handles exceptions, we adjust the return address to
be one instruction back with `sub lr, lr, #4`. The instruction
`srsdb sp!, #0x13` stores the return address `lr` and the `spsr` (the general
`cspr` register that is shadowed by IRQ mode's own version of it) registers to
the stack of mode `0x13` (supervisor mode) and then uses that mode's stack
pointer.

N.B: When an exception occurs, the CPU switches from whatever mode it was in to
IRQ mode - this has its own stack and its own versions of a few registters, such
as `sp` and `cpsr`, which are separate from normal registers.

`cpsid if, #0x13` switches to supervisor mode with interrupts disabled.
`push {r0-r3, r12, lr}` saves all of the caller's registers. Any function we
call saves registers 4-11 and restores them for us. We have to save r0-r3, r12,
and `lr`. Normal functions usually accept that these registers are garbage and
may not save them, but since interrupts mean we are interrupting some other code
that did not consent to calling a function, we must preserve all of the
registers so it is as if nothing happened when we return control to it.

```assembly
    and r1, sp, #4
    sub sp, sp, r1
```
ARM documentation states that the stack must be 8-byte aligned when calling
functions, but the exception handler may leave us with a stack that is not. The
code above fixes this.

`bl irq_handler` calls our C code handler.
`add sp, sp, r1` restores the stack alignment.
`pop {r0-r3, r12, lr}` restores the caller's saved registers.
`rfeia sp!` restores the saved `cpsr` and returns to the address stored in `lr`

### Handling IRQs
Now the IRQ exception handler is set up, we need the ability to determine which
IRQ was triggered, and to handle. We do this using the IRQ Peripheral.

For each IRQ, we need a specialised handler function. We do this by defining the
type of an interrupt handler, then declaring a static array of handlers. There
are three 4-byte words to represent possible interrupts, and the last 3 bytes of
the basic interrupts are repeats of others, so there are 72 different interrupts
that could be handled. We therefore decelare an array of 72 handlers.

Pi documentation says that the interrupt pending flag may not be cleared using
the interrupt peripheral, but instead using the hardware peripheral that
triggered the interrupt. Therefore, we define the fnuction type
`interrupt_clearer` to deal with this.

We then register an IRQ handler by inspecting its IRQ number, and setting the
appropriate data.

To check which IRQs have been triggered and execute the handler, we chack the
enabled bits of the IRQ peripheral and execute the correct handler accordingly.
We iterate over all pending interrupts, and if there exists a corresponding
handler, the clearer is called. Interrupts are then enabled to allow for nesting
of interrupts. Then the handler is called and interrupts are disabled to finish
the interrupt.

### Initialising interrupts
We zero out the handlers and callers arrays and disable all interrupts by
writing `0xffffffff` to each `disable` entry in the array.
`move_exception_vector()` is then called to copy the exception vector table to
address 0. Then interrupts are enabled.

`INTERRUPTS_ENABLED()` loads the `cpsr` then checks bit 7. If it is clear, then
interrupts are enabled.
`ENABLE_INTERRUPTS()` executes `cps` (**C**hange **P**rocessor **S**tate) with
the `ie` (**I**nterrupts **E**nable) suffix. The argument `i` enables IRQs
(while `f` would enable FIQs)
`DISABLE_INTERRUPTS()` works like `ENABLE_INTERRUPTS()`, except the suffix is
`id` (**I**nterrupts **D**isable)

### System Timer
The system timer is a hardware clock that can keep time and generate interrupts
after a certain time. It is located at offset `0x3000` from the peripheral base.
It is a Free Running Timer (TODO: what does this mean??) that increments a
64-bit counter every microsecond. It starts as soon as the Pi boots, and is
constantly running in the background while the Pi is on.

There are four compare registers which the timer compares the low 32 bits with
each counter tick. If any compare register matches the counter, an IRQ is
triggered. Each compare register has its own interrupt, numbers 0-3. Registers 0
and 2 are used by the GPU, but 1 and 3 are available to use.
The control register contains flags in its least-significant 4 bits to indicate
whether or not an interrupt has been triggered. Clearing this bit clears the
interrupt pending flag for that timer.

We can set a timer by setting the `compare1` register to the current value plus
some number of microseconds. This effectively counts down this number of
microseconds, and when it is done it calls the timer IRQ handler. 
TODO: probably more on this

## Processes
To initialise processes, we must initialise the run queue, allocate a PCB for
the main process, mark this as the current process, and call the scheduler.

We create a new process for the main process, `init`, and set it as the current
one. We initialise it to start directly after the end of the stack (`__end`),
and give it a process ID and name. We then create the lists `all_procs` and
`run_queue` for use in scheduling.

### Scheduling
Normal processes have no consideration for other processes - they would hog the
CPU until they are finished with their job. We have to implement a way to
systematically boot them off the CPU. The first way we do this is by
implementing the Round Robin scheduler, which gives each process a set quantum
of time for which they may run, and after this use of the CPU is then passed to
the next waiting process, regardless of whether the current process has
terminated.

### Context Switching
We must save the process' registers onto its stack, save the stack pointer to
the PCB, and load the saved stack pointer of the other process and popping the
registers. In `switch_context`, we save the link register and stack pointer onto
the stack, then get the current program state register and save it into the
register 12, a caller-save register, meaning its value need not be preserved.
The general purpose registers are then pushed.

The context switch occurs in the instructions:
```assembly
    str sp, [r0]
    ldr sp, [r1]
```
We can access the saved stack pointers of each process by simply reading and
storing from the memory addresses in register 0 and register 1, as in the
`proc` structure, the first field is the saved stack pointer. `r0` is the
stack pointer of the old process, and `r1` is that of the new process.

Before completely loading the new process, we reset the timer so that it goes
off in another quantum. Then we pop off all general purpose registers and load
the current program state register.

The last thing to do is to resume execution of the new process, by loading the
saved link register into the program counter. `lr` is caller-save, meaning an
existing process will overwrite whater we put there when it is time t return
from the exception - a new process jumps straight into execution without
returning from an exception (as it technically never experienced one). The new
process does not have anywhere to return to, so when it exits it will use `lr`
to return. We take advantage of this to have a process jump automatically to
cleanup code when it dies.

### Creating a new process

