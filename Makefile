
# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------
include config.mk

# default action: build all
all: build info

build: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
	$(SZ) $(BUILD_DIR)/$(TARGET).elf

upload: $(BUILD_DIR)/$(TARGET).bin
	# make sure STM32_Programmer_CLI is installed. and available in PATH
	# download: https://www.st.com/en/development-tools/stm32cubeclt.html
	STM32_Programmer_CLI -vb 1 -c port=SWD -w $< 0x8000000  -v  -rst

#######################################
# info
#######################################
info:
	@echo "PROJECT: "
	@printf "%b" "TARGET    = $(TARGET)\n";
	@printf "%b" "MCU       = $(MCU)\n";
	@printf "%b" "DEBUG     = $(DEBUG)\n";
	@echo "TOOLCHAIN: "
	@printf "%b" "  - CC    = $(CC)\n";
	@printf "%b" "  - AS    = $(AS)\n";
	@printf "%b" "  - CP    = $(CP)\n";
	@printf "%b" "  - SZ    = $(SZ)\n";

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/objects/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/objects/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/objects/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/objects/%.o: %.c Makefile | $(BUILD_DIR)/objects 
	@printf "%b" "$(COM_STRING)...";
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/objects/$(notdir $(<:.c=.lst)) $< -o $@ 2> $@.log; RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(@F)" "$(ERROR_STRING)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(@F)" "$(WARN_STRING)\n"; \
	else  \
		printf "%-60b%b" "$(@F)" "$(OK_STRING)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT


$(BUILD_DIR)/objects/%.o: %.s Makefile | $(BUILD_DIR)/objects
	@printf "%b" "$(COM_STRING)...";
	@$(AS) -c $(CFLAGS) $< -o $@ 2> $@.log; RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(@F)" "$(ERROR_STRING)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(@F)" "$(WARN_STRING)\n"; \
	else  \
		printf "%-60b%b" "$(@F)" "$(OK_STRING)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT


$(BUILD_DIR)/objects/%.o: %.S Makefile | $(BUILD_DIR)/objects
	@printf "%b" "$(COM_STRING)...";
	@$(AS) -c $(CFLAGS) $< -o $@ 2> $@.log; RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(@F)" "$(ERROR_STRING)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(@F)" "$(WARN_STRING)\n"; \
	else  \
		printf "%-60b%b" "$(@F)" "$(OK_STRING)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@printf "%b" "Linking...";
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 2> $@.log; RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(@F)" "$(ERROR_STRING)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(@F)" "$(WARN_STRING)\n"; \
	else  \
		printf "%-60b%b" "$(@F)" "$(OK_STRING)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT


$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@printf "%b" "Generating $(@F)...\n";
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@printf "%b" "Generating $(@F)...\n";
	@$(BIN) $< $@

$(BUILD_DIR)/objects:
	@mkdir -p $@

$(BUILD_DIR):
	@mkdir -p $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR) ./compile_commands.json

# Phony targets
# .PHONY: all build clean

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/objects/*.d)

# *** EOF ***
