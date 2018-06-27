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
Setup stack pointer (r13). Program Counter (r15) for kernel starts at 0x8000 an
grows upwards. Therefore our stack can safely start at 0x8000 without
interfering with the kernel. Note that the stack grows downwards.

```assembly
ldr r4, =__bss_start
ldr r9, =__bss_end
```
Loads start and end of BSS (Basic Service Set) into registers. This is where
statically allocated global variables that are not initialized explicitly to a
particular valuea are stored. The C runtime environment requires uninitialized
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
* `.bss` - uninitialised gloabal variables

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

* `-fpic` - generate position inedependent code, for use in a shared library
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
