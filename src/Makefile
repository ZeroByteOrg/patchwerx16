TARGET		=	PATCHWERX16.PRG
PKG			=	patchwerx16.zip
SYMFILE		=	$(TARGET).sym

CFLAGS		=	-t cx16 -O
LDFLAGS		=	-t cx16 -g -Ln $(SYMFILE)
EMU			=	box16

CC			=	/usr/local/bin/cc65
LD			=	/usr/local/bin/cl65

SRC		= $(wildcard *.c)
OBJ		= $(SRC:.c=*.o)
INC		= $(wildcard *.h)
ASM		= wait.asm
#INC	+=	scoreboard.h sound.h

RESOURCE	=

.PHONY: all clean run pkg

all: $(TARGET) $(RESOURCE)

$(TARGET): $(SRC) $(INC)
	cl65 $(LDFLAGS) -o $@ $(SRC) $(ASM)

install:
	cp $(TARGET) $(RESOURCE) ~/x16/

clean:
	rm -f *.o *.s *.PRG *.sym *.zip

run: all
	@$(EMU) -prg $(TARGET) -run -sym $(SYMFILE)

test: test.c
	cl65 -t cx16 -O -o TEST test.c
	x16emu -prg TEST -debug

pkg: $(PKG)

$(PKG): $(TARGET) $(RESOURCE)
	zip $@ $(TARGET) $(RESOURCE)

%.s: %.c %.h
	$(CC)	-o $@ $<

%.o: %.s %.h
	ca65 -t cx16 -o $@ $<
