# OGC NRTPS Demo on NES

## 'C' based development on NES using NRTPS and tasks

Show basic screens, controls, audio and computation via separate tasks

Flow chart:
1. Assembly interrupt collection defines the three interrupts of the NES and support drivers provide the rest of needed functionality
- firmware/nes/drivers.s provides other related drivers specific to the NES
- firmware/nes/crt0.s initializes everything, sets up interrupts and stack before passing control to 'C' main function
- firmware/nes/nmi.s defines the non maskable interrupt handler
- firmware/nes/irq.s defines the maskable interrupt handler
2. The OGC NRTPS provides the 'C' main function that finishes intialization and starts a game application
- software/libraries/ogc-lib-os-nrtps provides task scheduler functionality
3. The game application 




Title Screen:
	- "NRTPS DEMO" with build __DATE__ and __TIME__
Menu Screen:
	- button capture and associated processing ( with sound effects ):
		- Start: resets screen removing all objects and resetting spawn point
		- Select: starts or stops background music ( other buttons have sound effects added )
		- A: drops a hard object from the spawn point
		- B: drops a soft object from the spawn point
		- D-Pad: moves a spawn point sprite around the screen

Requirements:
- NRTPS task handle needs a monotonic clock and the ability to handle clock rollover
	- ?? interrupt advances clock each time the screen vsync is generated
	- ?? animation timers are stored in ram, how does this work

---
referencing https://github.com/jmk/cc65-nes-examples ( written in C )

Makefile is added as a WIP

Current compilation steps simplified to running the compile.bat file
---