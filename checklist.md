# proteus checklist
## Todo
### Write-up
- [ ] Introduction
- [ ] Background
  - [ ] Motivation
  - [ ] Relevant material
  - [ ] Why useful
- [ ] Design
  - [ ] Hardware/architecture overview
  - [ ] Project Management
- [ ] Implementation
    - [ ] Boot
    - [ ] Memory management
    - [ ] Framebuffer
    - [ ] Processes
    - [ ] Synchronisation
    - [ ] Keyboard
- [ ] Testing
- [ ] Results
- [ ] Conclusions
  - [ ] Evaluation
  - [ ] Further work

### Functional
- [x] Enable MMU
  - [ ] Not working for `0x00100000`???
- [ ] Get `try_lock()` (i.e. mutexes) working with `ldrex/strex`
- [ ] Processes
  - [x] Round Robin
  - [x] Make `schedule()` take algorithm as parameter
    - `schedulerfn` is a function pointer which may change
  - [x] Write up notes on creating new process
  - [ ] Option to create thread with arguments
- [ ] Filesystem
- [ ] IPC
- [x] Interrupts
  - [x] Finish `vector_table.S`
  - [x] Write up notes
- [x] Boot on real pi
- [x] Look through Baking Pi code and translate to C
  - unnecessary
- [x] Display to real HDMI
- [x] Debug rainbow screen at boot

### Polish
- [ ] doxygen documentation for structs
- [ ] Font color must be backwards(???) - fix
- [x] Write more generalised Makefile
- [x] Change `act_blink()` to use system timer

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
