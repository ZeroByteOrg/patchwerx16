TARGET		= PATCHWERX16.PRG

BIN		= build
SRC		= src
RES		= res

all:
	@$(MAKE) -C $(SRC)
	@$(MAKE) -C $(RES)
	@cp $(SRC)/$(TARGET) $(BIN)/
	@cp $(RES)/*.BIN $(BIN)/

clean:
	@rm -f $(BIN)/*
	@$(MAKE) -C $(SRC) clean
	@$(MAKE) -C $(RES) clean


# Build project, and run with box16 emulator
box16: all
	@(cd $(BIN) ; box16 -prg $(TARGET) -run)

# Build project and run with official emu, pre-release build of R39
run: all
	@(cd $(BIN) ; x16 -v r39 -prg $(TARGET) -run)

# Build project and run with official emu
run38: all
	@(cd $(BIN) ; x16-emu -prg $(TARGET) -run)

$(TARGET): $(BIN)/$(TARGET)

patchwerx16: $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(SRC)/$(TARGET)
	$(MAKE) -C $(SRC)
	@cp $(SRC)/$(TARGET) $(BIN)/
