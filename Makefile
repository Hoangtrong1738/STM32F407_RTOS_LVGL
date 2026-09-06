
TARGET      := build/blink_led
BUILD_DIR   := build
MAP_FILE    := file.map
LINKER      := STM32F407VETX_FLASH.ld

CC          := arm-none-eabi-gcc
OBJCOPY     := arm-none-eabi-objcopy

CPU_FLAGS := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CSTD        := -std=gnu11
OPT         := -O0 -g

INCLUDE_DIRS := \
	driver/inc \
	driver/Mylib/Inc \
	Inc

SRC_DIRS := \
	Src \
	driver/scr \
	driver/Mylib/Src

DEFS :=

# File dang viet do hoac chua muon build thi de o day.
EXCLUDE_SRCS := 

# Bat LVGL khi can build UI:
#   mingw32-make USE_LVGL=1
USE_LVGL ?= 0
LVGL_DIR := lvgl/lvgl-master
ifeq ($(USE_LVGL),1)
INCLUDE_DIRS += $(LVGL_DIR) $(LVGL_DIR)/src
SRC_DIRS += $(LVGL_DIR)/src
DEFS += -DLV_CONF_INCLUDE_SIMPLE
endif

ASM_SOURCES := Startup/startup_stm32f407vetx.s

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
C_SOURCES := $(foreach dir,$(SRC_DIRS),$(call rwildcard,$(dir)/,*.c))
C_SOURCES := $(filter-out $(EXCLUDE_SRCS),$(C_SOURCES))

FREERTOS_DIR := FreeRTOS-LTS/FreeRTOS/FreeRTOS-Kernel
# Thêm đường dẫn chứa header của FreeRTOS, thư mục Port, và thư mục chứa FreeRTOSConfig.h
INCLUDE_DIRS += \
    $(FREERTOS_DIR)/include \
    $(FREERTOS_DIR)/portable/GCC/ARM_CM4F


# Thêm thủ công các file core của FreeRTOS thay vì dùng rwildcard để tránh xung đột
FREERTOS_SRCS := \
    $(FREERTOS_DIR)/croutine.c \
    $(FREERTOS_DIR)/event_groups.c \
    $(FREERTOS_DIR)/list.c \
    $(FREERTOS_DIR)/queue.c \
    $(FREERTOS_DIR)/stream_buffer.c \
    $(FREERTOS_DIR)/tasks.c \
    $(FREERTOS_DIR)/timers.c \
    $(FREERTOS_DIR)/portable/GCC/ARM_CM4F/port.c \
    $(FREERTOS_DIR)/portable/MemMang/heap_4.c

# Gộp source của FreeRTOS vào tổng source chung
C_SOURCES += $(FREERTOS_SRCS)

OBJECTS := \
	$(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o)) \
	$(addprefix $(BUILD_DIR)/,$(ASM_SOURCES:.s=.o))

DEPS := $(OBJECTS:.o=.d)

INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))
CFLAGS   := $(CPU_FLAGS) $(CSTD) $(OPT) -ffunction-sections -fdata-sections -MMD -MP $(INCLUDES) $(DEFS)
ASFLAGS  := $(CPU_FLAGS)
LDFLAGS  := $(CPU_FLAGS) -T"$(LINKER)" -Wl,-Map="$(MAP_FILE)" -Wl,--gc-sections -static --specs=nosys.specs

.PHONY: all clean Flash print

all: $(TARGET).elf $(TARGET).hex $(TARGET).bin

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@if not exist "$(subst /,\,$(@D))" mkdir "$(subst /,\,$(@D))"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	@if not exist "$(subst /,\,$(@D))" mkdir "$(subst /,\,$(@D))"
	$(CC) -x assembler-with-cpp -c $< $(ASFLAGS) -o $@

print:
	@echo C_SOURCES=$(C_SOURCES)
	@echo OBJECTS=$(OBJECTS)

clean:
	@if exist "$(BUILD_DIR)" rmdir /S /Q "$(BUILD_DIR)"
	@if exist "$(MAP_FILE)" del /Q "$(MAP_FILE)"

Flash:
	STM32_Programmer_CLI -c port=SWD mode=UR -w $(TARGET).hex -v -rst

-include $(DEPS)
