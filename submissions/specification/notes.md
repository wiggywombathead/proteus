# Specification notes

## General info
Due date: Thursday 11th October 2018 @ 12 noon  
Worth 5% of module grade  
Around 2000 words

Must include a timetable, showing dependencies between stages (table/Gantt
chart), and may include other appropriate figures

## Assessment criteria
Specification and Progress Report marked together at the end of Term 1.  
Specification fulfil the following:
* consistent with supervisor's understanding of project they have agreed to
  supervise
* objectives given in specification are worthy of a third year project
* methods and timetable are well-planned and feasible to implement in the time
* proposed resources are available as stated
* possible risks have been considered and suitable mitigation covered
* legal, social, ethical, and professional issues identified where appropriate

## Contents of specification
* Title
* Problem statement/gap in existing provision
* Objectives
* Methods
* Timetable
* Resources to be used
* Ethical considerations (if any)

### Title
No more than ten words - able to be changed during year as project develops

### Problem statement
Justify choosing the project by idnetifying the problems it solves or the
knowledge it contributes, i.e. software development project that provides a
system with useful functionality that does not currently exist (at least, not in
the form proposed).  
The problem is not necessarily one requiring original research, rather, a
significant exercise applying the skills of a Computer Science degree to a
topical area of interest.  
Must be able to demonstrate that it is suitably challenging for a third year
project, and this sense of challenge must be conveyed in the specification.

### Objectives
Set out the goals that will combine to produce the final solution. May be
appropriate to allow flexibility by identifying "must have" objectives plus
additional areas if there is more time to explore.  
Breaking it down into objectives helps to assess feasibility and where the time
is going to be spent - helpful with the timetable.

### Methodology
How the project will be approached - how software development will be managed,
for example. Is it appropriate to apply an accepted development methodology, and
if so, which one? If not, how is development being approached?  
What methods are being used to ensure that the project stays on track and
achieves its goals?

### Timetable
Identify and allocate time for all things that require effort - think critical
path analysis and devise timetable to take this into account. Identify main
components of work and the dependencies between them.  
Produce as detailed a timetable as possible for the project, and set out main
tasks that need completing and week-by-week objectives.  
Continue updating timetable throughout, but keep a copy of the original to
refer back to in the Progress Report.

### Resources
The resources that the project relies on, for example, the hardware to be used,
any non-standard software, data a company has promised to provide, or subjects
taking part in interviews. What if these resources became unavailable - back up
plan?  
This part of the Specification should show that there are appropriate back up
plans in place where necessary

### Legal, social, ethical, and professional issues
Identify issues and how they will be managed.

# My specification
## Title
A modular kernel for the Raspberry Pi

## Problem statement
There is a vast selection of operating systems 

## Objectives
Implement:
* kernel capable of booting from GRUB on real hardware (hard disk)
* take input from keyboard using USB port on Raspi
* print output to real screen using HDMI port on Raspi
* create, read, update, delete files
* run multiple processes
* load different modules

Loadable modules:
* different methods for CPU scheduling, including:
    * First Come First Served
    * Round Robin
    * Shortest Job First
    * Shortest Remaining Time First
    * Preemptive and non-preemptive Priority Scheduling
    * Lottery Scheduling

and possibly:
    * Completely Fair Scheduler (CFS)
    * Brain Fuck Scheduler (BFS)
    * O(1) Scheduler

* different methods for disk scheduling, including:
    * First Come First Served
    * Shortest Seek Time First
    * SCAN (elevator algorithm) and C-SCAN
    * LOOK and C-LOOK
* interprocess communication
    * shared memory
    * message passing
* filesystem (loadable on request)

## Methodology

## Timetable
## Resources
* Git - version control
* Github - easily work from multiple computers
* C - used to implement most of the kernel
* ARM Assembly - bootloading
* GCC (arm-none-eabi-gcc) - cross-compilation environment
* QEMU - emulate OS before running on real hardware

* Cortex-A7 MPCore Technical Reference Manual
* ARM Cortex-A Series Programmer's Guide
* Broadcom BCM2835/6 ARM Peripherals Manual

## Risk assessment
This project relies mainly on the GCC cross-compilation environment, the C
programming language, and the ARM assembly language by way of software. While
updates to these would be the only likely cause of risk to the project, these
are unlikely as they are all fairly well-established and unlikely to change
drastically. Furthermore, as the hardware for which the operating system will be
developed is known, the Raspberry Pi 2 Model B, which I have access to, there
will be no risk of having hardware specifications change.

## Legal, ethical, social, and professional considerations

