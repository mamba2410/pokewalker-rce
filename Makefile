PW_TRIPLET	= h8300-hitachi-elf

PW_AS		= $(PW_TRIPLET)-as
PW_LD		= $(PW_TRIPLET)-gcc
PW_OBJCOPY	= $(PW_TRIPLET)-objcopy
PW_AS_FLAGS	= 
PW_LD_FLAGS	= 

PW_SRC_DIR	= src/pw/
PW_OBJ_DIR	= build/target/pw/

PW_SRC		= $(PW_SRC_DIR)/main.S
PW_OBJ		= $(patsubst $(PW_SRC_DIR)/%.S, $(PW_OBJ_DIR)/%_S.o, $(PW_SRC))
PW_ELF		= $(PW_OBJ_DIR)/pw.elf
PW_BIN		= ./pw.bin

$(PW_OBJ_DIR)/%_S.o: $(PW_SRC_DIR)/%.S
	$(PW_AS) $(PW_AS_FLAGS) $< -o $@

pw: $(PW_OBJ)
	$(PW_LD) $(PW_LD_FLAGS) $< -o $(PW_ELF)
	$(PW_OBJCOPY) $(PW_ELF) -f bin -o $(PW_BIN)


clean:
	rm -f $(PW_OBJ_DIR) $(PW_ELF) $(PW_BIN)
