# OGC NRTPS Demo on NES

'C' based development on NES using NRTPS and tasks
demo is to show basic screen progression, control capture, audio and computation using separate NRTPS tasks
final target is to create a game called "The Daily Grind" or "Daily Grind"

Flow chart ( "screens" are handled in a GUI task using state machine with the screen being states ):
    Title Screen:
		"NRTPS DEMO" with build __DATE__ and __TIME__
	Menu Screen:
		button capture and associated processing ( with sound effects ):
		    Start: resets screen removing all objects and resetting spawn point
			Select: starts or stops background music ( other buttons have sound effects added )
			A: drops a hard object from the spawn point
			B: drops a soft object from the spawn point
			D-Pad: moves a spawn point sprite around the screen

Requirements:
    NRTPS task handle needs a monotonic clock and the ability to handle clock rollover
	    Clock options include interrupts each time the screen vsync is generated
