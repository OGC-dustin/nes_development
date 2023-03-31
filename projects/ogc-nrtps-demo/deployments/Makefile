# Target platform
TARGET_PLATFORM := nes

# Compilation needs
CC65_DIR := c:\\cc65
CC := $(CC65_DIR)\\bin\\cc65.exe
CA := $(CC65_DIR)\\bin\\ca65.exe
LD := $(CC65_DIR)\\bin\\ld65.exe

# output file name
TARGET := ogc-nrtps-demo.nes

EXAMPLES := $(TARGET:..nes=)
OBJECTS := $(TARGET:.nes=.o)
ASSEMBLY_SOURCES := $(TARGET:.nes=.s)
C_SOURCES := $(TARGET:.nes=.c)

.PHONY: all

# Disable builtin rules (for .c, .o) by providing an empty .SUFFIXES rule
# Yes. GNU make is a rat's nest
.SUFFIXES:

# Make sure intermediate files are *NOT* deleted
# Yes. GNU make is a rat's nest
.PRECIOUS: %.s %.o

all: $(TARGET)

# submodule build
clean:
	@rm -fv $(TARGET)
	@rm -fv $(OBJECTS)
	@rm -fv $(ASSEMBLY_SOURCES)
	@rm -fv crt0.o

%.s: %.c
	$(CC) -Oi $< --target $(TARGET_PLATFORM) -I$(CC65_DIR)/include/ --add-source

%.o: %.s
	$(CA) $<

%.nes: %.o crt0.o
	$(LD) -C nrom_128_horz.cfg -o $@ $^ $(TARGET_PLATFORM).lib