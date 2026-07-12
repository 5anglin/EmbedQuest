/*
 * 状态机实现方式一：经典 switch-case
 * 对应文章：《万物皆可状态机：解密状态机 (FSM)，轻松驾驭复杂系统》
 *
 * 编译： gcc -Wall -Wextra -o fsm_switch fsm_switch.c
 */
#include <stdio.h>

typedef enum { STATE_IDLE, STATE_RUNNING, STATE_ERROR } SystemState_t;
typedef enum { EV_NONE, EV_START_PRESSED, EV_STOP_PRESSED, EV_FAULT_DETECTED } SystemEvent_t;

static SystemState_t g_currentState = STATE_IDLE;

static void state_machine_handler(SystemEvent_t event)
{
    printf("当前状态: %d, 收到事件: %d\n", g_currentState, event);
    switch (g_currentState) {
    case STATE_IDLE:
        if (event == EV_START_PRESSED) {
            puts("动作: 从IDLE切换到RUNNING");
            g_currentState = STATE_RUNNING;
        }
        break;
    case STATE_RUNNING:
        if (event == EV_STOP_PRESSED) {
            puts("动作: 从RUNNING切换到IDLE");
            g_currentState = STATE_IDLE;
        } else if (event == EV_FAULT_DETECTED) {
            puts("动作: 发生故障！切换到ERROR状态");
            g_currentState = STATE_ERROR;
        }
        break;
    case STATE_ERROR:
        if (event == EV_STOP_PRESSED) {
            puts("动作: 从ERROR状态恢复到IDLE");
            g_currentState = STATE_IDLE;
        }
        break;
    default:
        g_currentState = STATE_ERROR;
        break;
    }
}

int main(void)
{
    puts("=== switch-case 状态机演示 ===");
    state_machine_handler(EV_START_PRESSED);   /* IDLE -> RUNNING */
    state_machine_handler(EV_FAULT_DETECTED);  /* RUNNING -> ERROR */
    state_machine_handler(EV_STOP_PRESSED);    /* ERROR -> IDLE */
    printf("最终状态: %d (0=IDLE)\n", g_currentState);
    return 0;
}
