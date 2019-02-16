# proteus checklist
## Todo
### Functional
- [ ] Processes
  - [x] Round Robin
  - [ ] Make `schedule()` take algorithm as parameter
  - [ ] Write up notes on creating new process
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
- [ ] Font color must be backwards(???) - fix
- [ ] Change `act_blink()` to use system timer

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
