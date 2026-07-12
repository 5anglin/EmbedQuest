# 自动构建：每个 .c 都是独立可执行程序
include ../common.mk

SRCS := hardfault_coredump.c
BINS := $(SRCS:.c=)

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

run: all
	@for b in $(BINS); do echo "==== ./$$b ===="; ./$$b; echo; done

clean:
	rm -f $(BINS)

.PHONY: all run clean
