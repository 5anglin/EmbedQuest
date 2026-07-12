/*
 * 链接脚本：STM32F103C8（64KB Flash / 20KB RAM）
 * 面向 arm-none-eabi-gcc。对应文章《ARM Cortex-M启动揭秘》内存布局一节。
 */
ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* 栈顶 = RAM 末端 */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } >FLASH

    .text :
    {
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } >FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT> FLASH

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    /* 简单的栈/堆下限检查（可选） */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        . = . + 0x400;   /* 预留最小堆 1KB */
        . = . + 0x400;   /* 预留最小栈 1KB */
        . = ALIGN(8);
    } >RAM
}
