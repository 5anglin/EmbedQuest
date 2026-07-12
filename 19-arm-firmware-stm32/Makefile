# STM32F103C8 裸机固件构建（arm-none-eabi-gcc）
# 用法：
#   make            生成 blinky.elf / blinky.bin / blinky.hex
#   make size       显示各段大小
#   make flash      用 st-flash 烧录 .bin（需 stlink 工具）
#   make flash-ocd  用 openocd 烧录 .elf
#   make clean

TARGET  := blinky
TOOL    := arm-none-eabi
CC      := $(TOOL)-gcc
OBJCOPY := $(TOOL)-objcopy
SIZE    := $(TOOL)-size

MCUFLAGS := -mcpu=cortex-m3 -mthumb
CFLAGS   := $(MCUFLAGS) -std=c11 -Wall -Wextra -O2 -g3 \
            -ffunction-sections -fdata-sections -ffreestanding
LDSCRIPT := STM32F103C8.ld
LDFLAGS  := $(MCUFLAGS) -T$(LDSCRIPT) -nostartfiles \
            -Wl,--gc-sections -Wl,-Map=$(TARGET).map -specs=nano.specs

SRCS := startup_stm32f103.c system_stm32f103.c main.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET).elf $(TARGET).bin $(TARGET).hex size

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

size: $(TARGET).elf
	$(SIZE) $(TARGET).elf

flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

flash-ocd: $(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
	        -c "program $(TARGET).elf verify reset exit"

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).map

.PHONY: all size flash flash-ocd clean
