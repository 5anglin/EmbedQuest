# 推荐的编译器警告集合（第一招"编译器 Warning 一个都不能放过"）
# 在你的 Makefile 里： include warnings.mk  然后 $(CFLAGS)
# 对应文章：《还在996？教你五招……》

CFLAGS += -std=c11
CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += -Wshadow -Wconversion -Wsign-conversion
CFLAGS += -Wcast-align -Wcast-qual -Wpointer-arith
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes
CFLAGS += -Wswitch-default -Wswitch-enum
CFLAGS += -Wfloat-equal -Wundef -Wwrite-strings
CFLAGS += -Wredundant-decls -Wnested-externs
CFLAGS += -Wempty-body -Wignored-qualifiers
# 追求"零警告"时可开启：
# CFLAGS += -Werror
