# OGC NRTPS Demo on NES

## 'C' based development on NES using NRTPS and tasks

Show basic screens, controls, audio and computation via separate tasks

Flow chart:
1. Title Screen:
	- "NRTPS DEMO" with build __DATE__ and __TIME__
2. Menu Screen:
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

Current compilation steps to get a working control input with music and sound effects:
```
c:/cc65/bin/cc65.exe -Oi ogc-nrtps-demo.c --target nes -Ic:/cc65/include/ --add-source
c:/cc65/bin/ca65.exe ogc-nrtps-demo.s
c:/cc65/bin/ld65.exe -C .\nrom_128_horz.cfg crt0.o ogc-nrtps-demo.o -o ogc-nrtps-demo.nes nes.lib
```
---