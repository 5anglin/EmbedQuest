include ../common.mk
all: pp_demo
pp_demo: main.c log.h pp_utils.h
	$(CC) $(CFLAGS) -o pp_demo main.c
run: all
	./pp_demo
clean:
	rm -f pp_demo
.PHONY: all run clean
