# OS notes

## Hardware
Raspberry Pi 2 Model B
Uses Broadcom 2836 chip (architecture identical to BCM2835, except ARM1176JZF-S processor replaced with ARM Cortex-A7 processor)

## Development environment
Development done on x86_64 Linux 4.16.13-2-ARCH
Set up GCC cross-compiler for arm-none-eabi (provides toolchain to target System V ABI)

## First steps: minimal kernel 
### boot.s
```assembly
.section ".text.boot"
.global _start
```
Tells where the code is located in the compiled binary and makes _start global i.e. visible outside of the file

```assembly
_start:
    mrc p15, #0, r1, c0, c0, #5     - move to register from coprocessor
    and r1, r1, #3                  - and Rd, Rn, #3 == Rd := Rn & 3
    cmp r1, #0                      - r1 == 0? 
    bne halt
```
`mrc` - move to register from coprocessor (LOOK UP)
`and r1, r1, #3` - and Rn, Rm, <val> <=> Rn := Rm & <val>
Then compare if r1 == 0.
This sends three of four processors to halt. Multi-core OS is hard.

#### Note about the `mrc` command
Coprocessor in ARM is a misleading notion. It's shorthand for an optional piece of functionality that is not exposed via the core instruction set. ARM CPUs are modular. There are bits and pieces of CPU hardware that implementers of the architecture may or may not place on the chip. The memory management unit (MMU) is one example; there are others, such is the hardware debugging facility. Those are, indeed, identified by coprocessor number (pXX), so that more than one coprocessor can be present at the same time. The coprocessor number for MMU is traditionally p15. Coprocessors p0..p14 have nothing to do with memory management and may not be present. The debugging subsystem, for example, is p14.

The MRC and MCR commands are used to send commands to coprocessors. The mnemonic is, again, somewhat misleading - the effect of a command can be more than just a register move. It's more like MRC stands for "send a command to a coprocessor and get some data back" and MCR is "send a command to a coprocessor and pass some data along". Think of it that way. That's what the opcodes are for - that's the command to the coprocessor. Sometimes, a MCR/MRC command with a particular coproc # and opcode would even get separate mnemonic in the assembler (e. g. FPU commands).

The exact specifics of coprocessor opcodes and register numbers vary from one copropcessor to another. Since it's the MMU that you're interested in, read up on that particular one; it'll explain how do specific operations map to opcodes and coproc register numbers.

```assembly
mov sp, #0x8000
```
Setup stack pointer (r13). Program Counter (r15) for kernel starts at 0x8000 and grows upwards. Therefore our stack can safely start at 0x8000 without interfering with the kernel. Note that the stack grows downwards.

```assembly
    ldr r4, =__bss_start
    ldr r9, =__bss_end
```
Loads start and end of BSS (Basic Service Set) into registers. This is where statically allocated global variables that are not initialized explicitly to a particular valuea are stored. C runtime requires uninitialized global variables to be zeroed, which is done below:
```assembly
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
```
