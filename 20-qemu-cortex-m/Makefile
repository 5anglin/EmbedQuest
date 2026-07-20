# QEMU 可运行的 Cortex-M3 半主机示例
# 用法：
#   make          交叉编译生成 firmware.elf
#   make run      在 QEMU 里运行（半主机输出到当前终端），程序结束自动退出
#   make clean
#
# 依赖： gcc-arm-none-eabi 、 qemu-system-arm

TARGET  := firmware
TOOL    := arm-none-eabi
CC      := $(TOOL)-gcc

MCUFLAGS := -mcpu=cortex-m3 -mthumb
CFLAGS   := $(MCUFLAGS) -std=c11 -Wall -Wextra -O2 -g3 \
            -ffunction-sections -fdata-sections
# --specs=rdimon.specs 链接 newlib 半主机(rdimon)，让 printf 走到 QEMU 控制台
LDFLAGS  := $(MCUFLAGS) -T lm3s6965.ld -nostartfiles \
            -Wl,--gc-sections -Wl,-Map=$(TARGET).map \
            --specs=rdimon.specs

SRCS := startup.c main.c
OBJS := $(SRCS:.c=.o)

QEMU      := qemu-system-arm
QEMUFLAGS := -M lm3s6965evb -cpu cortex-m3 -nographic -semihosting

all: $(TARGET).elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS) lm3s6965.ld
	$(CC) $(LDFLAGS) $(OBJS) -o $@

run: $(TARGET).elf
	$(QEMU) $(QEMUFLAGS) -kernel $(TARGET).elf

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).map

.PHONY: all run clean
