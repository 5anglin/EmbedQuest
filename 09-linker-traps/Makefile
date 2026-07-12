include ../common.mk
OBJS := main.c config.c motor.c valve.c
all: linker_demo
linker_demo: $(OBJS) config.h
	$(CC) $(CFLAGS) -o linker_demo $(OBJS)
run: all
	./linker_demo
clean:
	rm -f linker_demo
.PHONY: all run clean
