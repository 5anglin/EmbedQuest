/*
 * 状态机实战：用 FSM 控制 LED（灭 -> 亮 -> 闪烁 -> 灭 循环）
 * 对应文章：《万物皆可状态机……》"用状态机点亮一个LED"
 *
 * 编译： gcc -Wall -Wextra -o led_fsm led_fsm.c
 */
#include <stdio.h>

/* --- 硬件抽象层（PC 上用打印模拟）--- */
static void hw_turn_led_off(void)      { puts("硬件操作: LED灭了！"); }
static void hw_turn_led_on(void)       { puts("硬件操作: LED亮了！"); }
static void hw_start_led_blinking(void){ puts("硬件操作: LED开始闪啊闪！"); }
static void hw_stop_led_blinking(void) { puts("硬件操作: LED停止闪烁了。"); }

/* --- 状态机定义 --- */
typedef enum { FSM_LED_STATE_OFF, FSM_LED_STATE_ON, FSM_LED_STATE_BLINKING } LedFsmState_t;
typedef enum { FSM_LED_EV_NONE, FSM_LED_EV_BUTTON_PRESSED } LedFsmEvent_t;

static LedFsmState_t s_ledCurrentState = FSM_LED_STATE_OFF;

static void led_fsm_init(void)
{
    s_ledCurrentState = FSM_LED_STATE_OFF;
    hw_turn_led_off();
    puts("LED状态机: 初始化完成，当前状态 LED_OFF (灭)");
}

static void led_fsm_handle_event(LedFsmEvent_t event)
{
    if (event == FSM_LED_EV_NONE && s_ledCurrentState != FSM_LED_STATE_BLINKING)
        return;

    if (s_ledCurrentState == FSM_LED_STATE_BLINKING && event == FSM_LED_EV_NONE) {
        puts("LED状态机: 我在闪烁中...");
        return;
    }

    switch (s_ledCurrentState) {
    case FSM_LED_STATE_OFF:
        if (event == FSM_LED_EV_BUTTON_PRESSED) {
            hw_turn_led_on();
            s_ledCurrentState = FSM_LED_STATE_ON;
            puts("LED状态机: 状态切换 -> LED_ON (亮)");
        }
        break;
    case FSM_LED_STATE_ON:
        if (event == FSM_LED_EV_BUTTON_PRESSED) {
            hw_start_led_blinking();
            s_ledCurrentState = FSM_LED_STATE_BLINKING;
            puts("LED状态机: 状态切换 -> LED_BLINKING (闪烁)");
        }
        break;
    case FSM_LED_STATE_BLINKING:
        if (event == FSM_LED_EV_BUTTON_PRESSED) {
            hw_stop_led_blinking();   /* 退出动作 */
            hw_turn_led_off();        /* 转换动作 */
            s_ledCurrentState = FSM_LED_STATE_OFF;
            puts("LED状态机: 状态切换 -> LED_OFF (灭)");
        }
        break;
    }
}

int main(void)
{
    led_fsm_init();
    for (int i = 1; i <= 3; ++i) {
        printf("\n模拟第%d次按按钮:\n", i);
        led_fsm_handle_event(FSM_LED_EV_BUTTON_PRESSED);
        led_fsm_handle_event(FSM_LED_EV_NONE);
    }
    puts("\n完美！一个循环结束了！");
    return 0;
}
