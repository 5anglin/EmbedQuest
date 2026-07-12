# 注意：vector_table.c 与 gcc_linker_script.ld 面向 arm-none-eabi，仅作参考，
#      不在 PC(gcc) 上编译。此处只构建可运行的启动流程仿真。
include ../common.mk
all: startup_sim
startup_sim: startup_sim.c
	$(CC) $(CFLAGS) -o startup_sim startup_sim.c
run: all
	./startup_sim
clean:
	rm -f startup_sim
.PHONY: all run clean
