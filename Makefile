
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
	STM32_Programmer_CLI -vb -c port=SWD -w $< 0x08000000 -v

#######################################
# info
#######################################
info:
	@echo "PROJECT: "
	@printf "%b" "$(COM_COLOR)TARGET    = $(OBJ_COLOR)$(TARGET)$(NO_COLOR)\n";
	@printf "%b" "$(COM_COLOR)MCU       = $(OBJ_COLOR)$(MCU)$(NO_COLOR)\n";
	@printf "%b" "$(COM_COLOR)DEBUG     = $(OBJ_COLOR)$(DEBUG)$(NO_COLOR)\n";
	@echo "TOOLCHAIN: "
	@printf "%b" "$(COM_COLOR)  - CC      = $(OBJ_COLOR)$(CC)$(NO_COLOR)\n";
	@printf "%b" "$(COM_COLOR)  - AS      = $(OBJ_COLOR)$(AS)$(NO_COLOR)\n";
	@printf "%b" "$(COM_COLOR)  - CP      = $(OBJ_COLOR)$(CP)$(NO_COLOR)\n";
	@printf "%b" "$(COM_COLOR)  - SZ      = $(OBJ_COLOR)$(SZ)$(NO_COLOR)\n";

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
	@printf "%b" "$(COM_COLOR)$(COM_STRING)...";
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/objects/$(notdir $(<:.c=.lst)) $< -o $@ 2> $@.log; RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(OBJ_COLOR) $(@F)" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(OBJ_COLOR) $(@F)" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"; \
	else  \
		printf "%-60b%b" "$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT


$(BUILD_DIR)/objects/%.o: %.s Makefile | $(BUILD_DIR)/objects
	@printf "%b" "$(COM_COLOR)$(COM_STRING)...";
	@$(AS) -c $(CFLAGS) $< -o $@ 2> $@.log; RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"; \
	else  \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT


$(BUILD_DIR)/objects/%.o: %.S Makefile | $(BUILD_DIR)/objects
	@printf "%b" "$(COM_COLOR)$(COM_STRING)...";
	@$(AS) -c $(CFLAGS) $< -o $@ 2> $@.log; RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"; \
	else  \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@printf "%b" "$(COM_COLOR)$(COM_STRING)...";
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 2> $@.log; RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"; \
	else  \
		printf "%-60b%b" "$(OBJ_COLOR) $(F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT


$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@printf "%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@$(NO_COLOR)\n";
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@printf "%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@$(NO_COLOR)\n";
	@$(BIN) $< $@

$(BUILD_DIR)/objects:
	@mkdir -p $@

$(BUILD_DIR):
	@mkdir -p $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

  # Phony targets
.PHONY: all build upload clean

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/objects/*.d)

# *** EOF ***
