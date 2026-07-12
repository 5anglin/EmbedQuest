/*
 * 参考链接器脚本（arm-none-eabi-gcc），对应文章"内存布局与链接器脚本"一节。
 * 仅作参考，请按目标 MCU 的 Flash/RAM 大小调整。
 */
ENTRY(Reset_Handler)

_estack = 0x20020000;   /* 栈顶：RAM 结束地址（示例：128KB RAM 的 STM32F4） */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))    /* 向量表必须放最前面 */
    } >FLASH

    .text :
    {
        *(.text*)               /* 代码段 */
        *(.rodata*)             /* 只读数据 */
        . = ALIGN(4);
    } >FLASH

    _sidata = LOADADDR(.data);  /* .data 在 Flash 中的加载地址 */

    .data :
    {
        _sdata = .;             /* .data 在 RAM 中的起始 */
        *(.data*)
        . = ALIGN(4);
        _edata = .;             /* .data 结束 */
    } >RAM AT> FLASH

    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM
}
