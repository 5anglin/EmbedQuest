include ../common.mk
all: interrupt_demo
interrupt_demo: interrupt_demo.c platform_mock.h
	$(CC) $(CFLAGS) -o interrupt_demo interrupt_demo.c
run: all
	./interrupt_demo
clean:
	rm -f interrupt_demo
.PHONY: all run clean
