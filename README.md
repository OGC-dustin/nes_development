# nes_development
Development for the 6502 microprocessor variant ( 65c02 ) in an NES on Windows 10
- https://www.nesdev.org/ includes references and NES Cart Database
- http://www.6502.org/tutorials/6502opcodes.html
- https://en.wikibooks.org/wiki/NES_Programming
- https://en.wikibooks.org/wiki/6502_Assembly -> https://en.wikibooks.org/wiki/65c02_Assembly
- https://en.wikibooks.org/wiki/NES_Programming/Introduction
- https://en.wikibooks.org/wiki/NES_Programming/Initializing_the_NES
- https://github.com/jmk/cc65-nes-examples ( written in C )

---
## Tools ( NesHacker Tutorial Video at https://www.youtube.com/watch?v=RtY5FV5TrIU )
---
Editor 

    Visual Studio Code - Plugins and extension to help with nes games
    https://code.visualstudio.com/ -> download
    extensions tab -> install "ca65 macro assembler"

Assembler

    cc65 - the 6502 C compiler includes the ca65 assembler we are looking for
    https://github.com/cc65/cc65 -> https://cc65.github.io/
    -> getting started page -> download windows snapshot

Emulator

    fceux - Suggested for development
        https://fceux.com/web/download.html -> download section -> win32 Binary
    bizhawk - suggested for playing games on Windows
        https://github.com/TASEmulators/BizHawk ( I have not tested this yet )

Audio Develpoment

    FamiTracker
    http://www.famitracker.com/ -> download

Graphic Development

    YY-CHR https://w.atwiki.jp/yychr/

---
## Development Sequence
---
1. https://github.com/NesHacker/DevEnvironmentDemo is a demo project referenced in the above tutorial.
2. the demo.s file is the assembly source file of interest ( the games code )
3. cl65config.json is like a makefile which must point to path of cc65 assembler files, define the demo.s file and also containes parameters that will be used during compilation
4. ( ONE TIME ONLY ) Visual Studio must have a "Build Task" defined for this project in order to build it...  the ca65 extension can help with this
- open demo.s
- Ctrl + Shift + P
- "configure default build task"
- "ca65: Build without config"
- new file tasks.json contains the generated build task
5. Build the demo
- open demo.s
- Ctrl + Shift + P
- "run build task"
6. Test output of the build in your emulator

---
## NES version of 6502 ( 8 bit processor )
---
CPU - Ricoh 2A03 CPU which is a MOS 6502 without a decimal mode.
- Clocked 1.789773Mhz for NTSC (System 21.47727Mhz / 12) and 1.773447Mhz for PAL (System 26.601171Mhz / 15)
- Internal Memory - 2 KB

Video - Ricoh 2C02 PPU (Picture Processing Unit)
- Resolution - 256 x 240 pixels (Only 256 x 224 pixels are visible on NTSC)
    - each nametable can hold 32x30 blocks of 8x8 characters from pattern tables
- Colors - Fixed 52 (44 colors, 5 grays and 3 duplicated)
- Palette
    - Background: 4 Palettes x 3 Colors + 1 Back Color
    - Sprites: 4 Palettes x 3 Colors
    - Tiles - 8 x 8 pixels tiled background
    - Sprites - Up to 64 8x8-pixel or 8x16 pixel sprites but only 8 can be displayed per scan-line
- Sound
    - 2 Pulse wave channels
    - 1 Triangle wave channel
    - 1 Noise channel
    - 1 DPCM channel

Number Representation
- \# is a predicate meaning Constant or Immediate Value that can be added to any of the following 3 type i.e. #$101, #%101, and #101
- $101 = Hex 0x101 = 257
- %101 = Binary 0b101 = 5
- 101 = Decimal 101 = 101

Addressing Modes
- Immediate Addressing - with the use of the immediate symbol \#, only work with single bytes
- ZeroPage Addressing - 8 bit address operand, actions on ram 0x000 to 0x00FF are typically faster 
- Absolute Addressing - use a full 16 bit address with your operand
- Implicit Addressing - no operands used with instruction

Registers ( 6 )
- A - Accumulator, all data movement, mathematical, and logical operations go through here, otherwise it is a general-purpose register
- X - Index Register, used for general counting and pre-index memoery offsetting, otherwise it is a general-purpose register
- Y - Index Register, used for general counting and post-index memoery offsetting, otherwise it is a general-purpose register
- PC - Program Counter - tracks program execution
- SP - Stack Pointer - tracks stack position
- SR - Processor Flags/Status - holds results of previous instructions
    - N - negative flag
    - V - Overflow
    - Unused
    - B - Break
    - D - Decimal ( I don't think this is supported on NES )
    - I - Interrupt Disable
    - Z - zero bit
    - C - carry bit

System Memory ( 64 KB )

Internal ??
- 0x0000 - 0x00FF ( 256 Bytes ) RAM Zero Page - Special Zero Page addressing modes give faster memory R/W access
- 0x0100 - 0x01FF ( 256 Bytes ) RAM Stack memory
- 0x0200 - 0x07FF ( 1536 Bytes ) RAM

External ??
- 0x0800 - 0x0FFF ( 2048 Bytes ) Mirror 1 of RAM
- 0x1000 - 0x17FF ( 2048 Bytes ) Mirror 2 of RAM
- 0x1800 - 0x1FFF ( 2048 Bytes ) Mirror 3 of RAM
- 0x2000 - 0x2007 ( 8 Bytes ) Input/Output registers ( graphics, Sounds, Controllers, PRG-ROM bank swapping )
    - PPU
        - 0x2000 W/O PPU Control Register 1
        - 0x2001 W/O PPU Control Register 2
        - 0x2002 R/O PPU Status Register
        - 0x2006 W/O PPU Memory Address
        - 0x2007 R/W PPU Memory Data
    - Sprites
        - 0x2003 W/O Sprite Memory Address
        - 0x2004 R/W Sprite Memory Data
        - 0x2005 W/O Background Scroll
- 0x2008 - 0x3FFF ( 8184 Bytes ) Multiple Mirrors of I/O registers ???
- 0x4000 - 0x401F ( 32 Bytes ) Input/Output registers ( graphics, Sounds, Controllers, PRG-ROM bank swapping )
    - Sound
        - Square Wave 1
            - 0x4000
            - 0x4001
            - 0x4002
            - 0x4003
        - Square Wave 2
            - 0x4004
            - 0x4005
            - 0x4006
            - 0x4007
        - Triangle Wave
            - 0x4008
            - 0x4009
            - 0x400A
            - 0x400B
        - Noise
            - 0x400C
            - 0x400D
            - 0x400E
            - 0x400F
    - DMC
        - 0x4010 sample rate ( bits 4-7 unused? )
        - 0x4011 volume
        - 0x4012 voice table start address ( N * 0x40 + 0xC000 )
        - 0x4013 Voice table size ( Bytes ) (N * 16 + 1)
    - Sprites ( continued )
        - 0x4014 W/O DMA access to sprite memory
    - Sound ( continued )
        - 0x4015 R/W enable/disable individual sound channels
    - Joystick
        - 0x4016 R/W Joystick 1
        - 0x4017 R/W Joystick 2
    
- 0x4020 - 0x5FFF ( 8160 Bytes ) Expansion ROM
- 0x6000 - 0x7FFF ( 8192 Bytes ) SRAM used to save game data between game plays
    - ( NOTE: some cartridges had batteries to save data while disconnected... check for leakage if you own any of these )
- 0x8000 - 0xFFFF ( 32768 Bytes ) PRG-ROM
    - 0xFFFA - 0xFFFB Non-Maskable Interrupt handler routine ( NMI )
    - 0xFFFC - 0xFFFD Power on reset handler routine ( PORN )
    - 0xFFFE - 0xFFFF Break handler routine ( BRK instruction )

50+ instructions ( what data does this instruction modify, how does it modify it )

---
## NES System Specifications and Development Notes
---
Program ROM of NES is at address 0x8000

PPU - Picture Processing Unit - Coprocessor
- 4 sections
    1. pattern tables
        - ( 2 sections, each 64KB of memory, 256 Sprite ( 8x8 ) tiles per table )
    2. name tables ( background graphics )
        - 32x30 tiles, each of which is 8x8, 1 byte per tile pointing to pattern tables
        - advanced processes used to update without slowing down a game by rerendering the full table
        - attribute table at the end of each nametable
    3. Palettes ( 50+ colors possible )
        - 8 active palletes at a time, each with 4 colors
            - 4 palletes for the background, 4 palletes for the foreground
        - 1st color of each pallete is interpreted as the transparency color, leaving 3 colors for the sprite remain
        
    4. Object Attribute Memory ( OAM ) - Foreground and Sprites, the things above and below the background
        - independent sprites that make up characters, enemies, effects, and more
        - 64 sprites at any given time, each defined by 4 bytes
            1. vertical ( y ) coordinate
            2. pattern table refrence
            3. attributes
                - 0b0 - fg pallete
                - 0b1 - fg pallete
                - 0b2 - unused, ignored by ppu
                - 0b3 - unused, ignored by ppu
                - 0b4 - unused, ignored by ppu
                - 0b5 - above ( 0b0 ) or below ( 0b1 ) background
                - 0b6 - flip control
                - 0b7 - flip control
            4. horizontal ( x ) coordinate
    - ( additional functions possible with advanced controls )

---
NTSC ( 60 FPS, 525 scan lines )
    - Full screen is represented by 256 x 240 pixel name table representation
