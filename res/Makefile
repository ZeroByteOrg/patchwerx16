IMG2BIN		= ../tools/img2tileset.py

all: GFX.BIN PALETTE.BIN

clean:
	@rm GFX.BIN
	

GFX.BIN: widget_tiles.png
	$(IMG2BIN) -b 4 -s 16x16 $< $@

PALETTE.BIN: palette.png
	@echo "The palette conversion tool should run here..."
