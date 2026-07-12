# 顶层 Makefile：递归构建 / 运行 / 清理所有示例
# 用法：
#   make        构建全部示例
#   make run    构建并运行全部示例
#   make clean  清理全部产物

SUBDIRS := \
  01-c-undefined-behavior \
  02-lexical-traps \
  03-semantic-traps-1 \
  04-semantic-traps-2 \
  05-syntax-traps \
  06-preprocessor-horrors \
  07-preprocessor-toolkit \
  08-library-traps \
  09-linker-traps \
  10-portability-traps \
  11-fsm \
  12-cortex-m-startup \
  13-cortex-m-interrupts \
  14-hardfault-debug \
  15-embedded-architectures \
  16-i2c \
  17-debug-nine-rules \
  18-code-quality

all:
	@for d in $(SUBDIRS); do $(MAKE) -C $$d all || exit 1; done

run:
	@for d in $(SUBDIRS); do \
	  echo "########## $$d ##########"; \
	  $(MAKE) -s -C $$d run || exit 1; \
	done

clean:
	@for d in $(SUBDIRS); do $(MAKE) -C $$d clean; done

.PHONY: all run clean
