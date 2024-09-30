######################################
# target
######################################
TARGET = urcjoy

ENABLE_PROTO_PPM=1
ENABLE_PROTO_DSM=0
ENABLE_PROTO_FPORT=0
ENABLE_PROTO_IBUS=0
ENABLE_PROTO_SBUS=0
ENABLE_LOG_SERIAL=0

######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
ifeq ($(DEBUG), 1)
OPT = -Og
else
OPT = -Os
endif

#######################################
# paths
#######################################
# Build path
PROJECT_DIR = $(shell pwd)
BUILD_DIR = ${PROJECT_DIR}/build
TOOLCHAIN_DIR = ${HOME}/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.6
######################################
# source
######################################
# C sources
C_SOURCES =  \
$(shell find ${PROJECT_DIR}/Core/Src/ -maxdepth 1 -type f -name "*.c" | sort) \
$(shell find ${PROJECT_DIR}/USB_DEVICE/ -type f -name "*.c" | sort) \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
${TOOLCHAIN_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
${TOOLCHAIN_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
${TOOLCHAIN_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
${TOOLCHAIN_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Src/usbd_customhid.c \
${PROJECT_DIR}/Core/Src/protocol/protocol.c

ifeq ($(ENABLE_PROTO_PPM), 1) 
C_SOURCES += ${PROJECT_DIR}./Core/Src/protocol/protocol_ppm.c
endif
ifeq ($(ENABLE_PROTO_DSM), 1) 
C_SOURCES += ${PROJECT_DIR}./Core/Src/protocol/protocol_dsm.c
endif
ifeq ($(ENABLE_PROTO_FPORT), 1) 
C_SOURCES += ${PROJECT_DIR}./Core/Src/protocol/protocol_fport.c
endif
ifeq ($(ENABLE_PROTO_IBUS), 1) 
C_SOURCES += ${PROJECT_DIR}./Core/Src/protocol/protocol_ibus_ia6.c ${PROJECT_DIR}./Core/Src/protocol/protocol_ibus.c
endif
ifeq ($(ENABLE_PROTO_SBUS), 1) 
C_SOURCES += ${PROJECT_DIR}./Core/Src/protocol/protocol_sbus.c
endif


# ASM sources
ASM_SOURCES = startup_stm32f103x6.s
# ASM sources
ASMM_SOURCES = 

#######################################
# binaries
#######################################
GCC_PATH = /opt/stm32cubeclt/GNU-tools-for-STM32/bin
STM32_PROGRAMMER = /opt/stm32cubeclt/STM32CubeProgrammer/bin/STM32_Programmer_CLI
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS = \
-DUSE_HAL_DRIVER \
-DSTM32F103x6 \
-DPROTO_PPM=$(ENABLE_PROTO_PPM) \
-DPROTO_DSM=$(ENABLE_PROTO_DSM) \
-DPROTO_FPORT=$(ENABLE_PROTO_FPORT) \
-DPROTO_IBUS=$(ENABLE_PROTO_IBUS) \
-DPROTO_SBUS=$(ENABLE_PROTO_SBUS) \

ifneq ($(ENABLE_LOG_SERIAL), 1)
C_DEFS += -DNO_LOG_SERIAL
endif

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IUSB_DEVICE/App \
-IUSB_DEVICE/Target \
-I${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc \
-I${TOOLCHAIN_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-I${TOOLCHAIN_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc \
-I${TOOLCHAIN_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc \
-I${TOOLCHAIN_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include \
-I${TOOLCHAIN_DIR}/Drivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103C6Tx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections


OK_STRING    = "[OK]"
ERROR_STRING = "[ERROR]"
WARN_STRING  = "[WARNING]"
COM_STRING   = "Compiling"
