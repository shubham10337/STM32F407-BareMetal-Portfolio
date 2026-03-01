# --- UNIVERSAL BARE METAL BUILD SYSTEM (STM32F407) ---

# 1. Toolchain Paths
TOOLCHAIN_ROOT = C:/ST/STM32CubeCLT_1.20.0/GNU-tools-for-STM32/bin
CC      = $(TOOLCHAIN_ROOT)/arm-none-eabi-gcc
OBJCOPY = $(TOOLCHAIN_ROOT)/arm-none-eabi-objcopy
SIZE    = $(TOOLCHAIN_ROOT)/arm-none-eabi-size

# 2. Project Definitions
# PROFESSOR'S NOTE: We calculate the relative path to drivers.
# Assuming this is included from 'Projects/MyProject', drivers is '../../drivers'
BSP_DIR = ../../drivers

# 3. Architecture Flags
# NOTE: F407 has FPU. To enable hard float, change to: -mfloat-abi=hard -mfpu=fpv4-sp-d16
MACH    = cortex-m4
# CRITICAL: Added '-I$(BSP_DIR)/inc' so #include "stm32f4xx.h" works.
# CRITICAL: Added '-DSTM32F407xx' required by CMSIS headers to select the correct device.
CFLAGS  = -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0 \
          -I$(BSP_DIR)/inc \
          -DSTM32F407xx

# 4. Linker Flags
# Points to the linker script in 'drivers/linker'
LDFLAGS = -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs \
          -T $(BSP_DIR)/linker/stm32f407.ld \
          -Wl,-Map=final.map -Wl,--gc-sections -nostartfiles

# 5. Source Management
# Step A: Keep the sources you defined in your local Makefile (main.c, uart.c)
PROJECT_SRCS := $(SRCS)

# Step B: Append the Drivers/System sources
# We need startup.c (Reset_Handler) and system_stm32f4xx.c (SystemInit)
DRIVER_SRCS  = $(BSP_DIR)/src/startup.c \
               $(BSP_DIR)/src/system_stm32f4xx.c

# Combine all sources
ALL_SRCS = $(PROJECT_SRCS) $(DRIVER_SRCS)

# Step C: Generate Object names (.c -> .o)
OBJS = $(ALL_SRCS:.c=.o)

# 6. Build Targets

all: $(PROJECT_NAME).elf

# GENERIC RULE: Compiles ANY .c file into a .o file
# This replaces the hardcoded 'main.o: main.c' and works for uart.c too.
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -o $@ $<

# Linker Rule
$(PROJECT_NAME).elf: $(OBJS)
	@echo "Linking Target..."
	$(CC) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@ $(PROJECT_NAME).bin
	$(SIZE) $@
	@echo "-------------------------------------------------------"
	@echo "Build Complete: $(PROJECT_NAME).bin"
	@echo "-------------------------------------------------------"

clean:
	rm -f *.o *.elf *.bin *.map
	rm -f $(BSP_DIR)/src/*.o