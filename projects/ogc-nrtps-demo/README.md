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
