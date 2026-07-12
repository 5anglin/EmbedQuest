include ../common.mk
OBJS := main.c i2c_bitbang.c i2c_slave_sim.c
all: i2c_demo
i2c_demo: $(OBJS) i2c_bitbang.h i2c_slave_sim.h
	$(CC) $(CFLAGS) -o i2c_demo $(OBJS)
run: all
	./i2c_demo
clean:
	rm -f i2c_demo
.PHONY: all run clean
