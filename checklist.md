# proteus checklist
## Todo
### Write-up
- [ ] Abstract
- [x] Motivation
  - [x] Introduction
- [x] Background
  - [x] Relevant material
  - [x] Why worthwhile
  - [x] Useful concepts
- [ ] Design
  - [x] Hardware/architecture
  - [x] Development Environment
  - [x] System Overview
  - [x] Project Management
    - [ ] Make contribution graph the most recent from Github
- [ ] Implementation
    - [x] Booting
    - [x] Organising Memory
    - [ ] Interacting with the GPU
    - [ ] Interrupts and Exceptions
    - [ ] System Timer
    - [ ] Processes
    - [ ] Scheduling
    - [ ] Interacting with the Memory Management Unit
    - [ ] Synchronisation
    - [ ] Inter-process Communication
    - [ ] User interaction
- [ ] Testing
- [ ] Evaluation
  - [ ] Achievements/Results
  - [ ] Limitations
  - [ ] Further work

Sections to add:
- [x] sec:LinkerSection
- [x] sec:evaluation
- [ ] sec:GPIO
- [ ] sec:IRQs
- [ ] sec:directives
- [ ] app:timetable (revised)
### Functional
- [x] Enable MMU
  - [x] Not working for `0x00100000`???
- [ ] Get `try_lock()` (i.e. mutexes) working with `ldrex/strex`
- [ ] Processes
  - [x] Round Robin
  - [x] Make `schedule()` take algorithm as parameter
    - `schedulerfn` is a function pointer which may change
  - [x] Write up notes on creating new process
  - [ ] Option to create thread with arguments
- [ ] Filesystem
- [ ] IPC
  - [x] Shared memory done to an extent
  - [ ] Message Passing
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
