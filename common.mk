# 共享编译配置，被各子目录 Makefile 包含
CC      ?= gcc
CSTD    ?= -std=c11
CFLAGS  ?= $(CSTD) -Wall -Wextra -O2 -g
