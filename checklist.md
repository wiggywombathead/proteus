# proteus checklist
## Todo
- [x] Set up virtual machine
- [x] Hello kernel world
- [x] Memory management
  - [x] implement generic linked list with macros
  - [x] implement dynamic allocator (malloc and free)
  - [ ] TODO: memcpy and framebuffer.h
- [x] Display to real screen through HDMI
- [ ] Processes
- [ ] Change return value of `get_total_mem()` to 0 in atag.c in real thing
- [ ] Boot from GRUB on real hardware

- [ ] printf() - handle more cases
- [ ] itoa()

## CPU scheduling
  * FCFS
  * SJF
  * SRTF
  * Priority (preemptive and non)
  * Round Robin
  * Lottery

## Disk scheduling (REDACTED)
  * FCFS
  * SSTF
  * SCAN
  * C-SCAN
  * LOOK
  * C-LOOK

## Filesystem
  * Persistent
  * Loadable on request

## Interprocess Communication
  * Shared memory
  * Message passing
