PW_TRIPLET	= h8300-hitachi-elf

PW_AS		= $(PW_TRIPLET)-as
PW_LD		= $(PW_TRIPLET)-ld
PW_OBJCOPY	= $(PW_TRIPLET)-objcopy
PW_AS_FLAGS	= -I'src/pw/' -mach=h8300h
PW_LD_FLAGS	= -nostdlib -m h8300helf

PW_SRC_DIR	= src/pw/
PW_OBJ_DIR	= build/target/pw/

PW_LDS		= $(PW_SRC_DIR)/payload.ld
PW_SRC		= $(PW_SRC_DIR)/payload.S
PW_OBJ		= $(patsubst $(PW_SRC_DIR)/%.S, $(PW_OBJ_DIR)/%_S.o, $(PW_SRC))
PW_ELF		= $(PW_OBJ_DIR)/pw.elf
PW_BIN		= ./pw.bin
PW_BIN_INC	= ./src/arduino-send/payload.h

$(PW_OBJ_DIR)/%_S.o: $(PW_SRC_DIR)/%.S
	$(PW_AS) $(PW_AS_FLAGS) $< -o $@

pw: $(PW_OBJ)
	$(PW_LD) $(PW_LD_FLAGS) -T $(PW_LDS) $< -o $(PW_ELF)
	$(PW_OBJCOPY) $(PW_ELF) -O binary $(PW_BIN)

pw-header: pw
	@echo "#ifndef PAYLOAD_H\n#define PAYLOAD_H\n" > $(PW_BIN_INC)
	xxd -i $(PW_BIN) >> $(PW_BIN_INC)
	@echo "\n#endif" >> $(PW_BIN_INC)



AD_CLI		= arduino-cli
AC_FLAGS	= 
AU_FLAGS	= -v
AD_BOARD	= arduino:avr:mega
AD_SRC		= src/arduino-send/
AD_PORT		= /dev/ttyACM0

arduino:
	$(AD_CLI) compile $(AC_FLAGS) -b $(AD_BOARD) $(AD_SRC)

upload: arduino
	$(AD_CLI) upload $(AU_FLAGS) -p $(AD_PORT) -b $(AD_BOARD) $(AD_SRC)

clean:
	rm -f $(PW_OBJ_DIR)/* $(PW_ELF) $(PW_BIN)

