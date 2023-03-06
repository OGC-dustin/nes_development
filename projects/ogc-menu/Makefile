BUILD = ./build
SRC = ./src
OBJECTS = ogc-menu.o

all: ogc-menu.nes

clean:
	mkdir -p $(BUILD)
	rm -rf build/*.o ogc-menu.nes

nes-pi.nes: $(OBJECTS)
	cl65 -t nes -o ogc-menu.nes $(BUILD)/*.o

%.o: $(SRC)/%.s
	ca65 $< -o $(BUILD)/$@  -t nes
