/*
 * SystemInit：本例保持复位默认时钟（HSI 8MHz），仅提供占位实现。
 * 真实项目通常在此配置 HSE + PLL 到 72MHz、Flash 等待周期等。
 */
void SystemInit(void)
{
    /* 复位默认使用内部 HSI，无需额外配置即可运行点灯示例。 */
}
