# nes_development
Development for the 6502 microprocessor in a NES on Windows 10

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
## 6502 Assembly ( 8 bit processor )
---
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
- A - Accumulator for mathematics and bitwise manipulation
- X - Index Register ( Counter or Position ) [set immediate, copy memory, increment, decrement ]
- Y - Index Register ( COunter or Postiion ) [set immediate, copy memory, increment, decrement ]
- PC - program counter
- SP - stack pointer
- SR - 

System Memory ( 64 KB )
- RAM 0x000 - 0x07FF
- Mirror 1 - 0x0800 - 0x0FFF
- Mirror 2 - 0x1000 - 0x17FF
- Mirror 3 - 0x1800 - 0x1FFF
- I/O - 0x2000 - 0x401F ( graphics, Sounds, COntrollers, PRG-ROM bank swapping )
- Catridsge Space - 0x4020 ->

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
