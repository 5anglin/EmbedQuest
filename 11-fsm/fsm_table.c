/*
 * 状态机实现方式二：函数指针表（状态表）
 * 对应文章：《万物皆可状态机……》"状态表/函数指针大法"
 *
 * 编译： gcc -Wall -Wextra -o fsm_table fsm_table.c
 */
#include <stdio.h>

typedef enum { STATE_IDLE, STATE_RUNNING, STATE_ERROR, STATE_COUNT } SystemState_t;
typedef enum { EV_NONE, EV_START_PRESSED, EV_STOP_PRESSED, EV_FAULT_DETECTED } SystemEvent_t;

static SystemState_t g_currentState = STATE_IDLE;

static void handle_state_idle(SystemEvent_t event);
static void handle_state_running(SystemEvent_t event);
static void handle_state_error(SystemEvent_t event);

static void (*const state_handlers[STATE_COUNT])(SystemEvent_t) = {
    handle_state_idle,
    handle_state_running,
    handle_state_error,
};

static void state_machine_handler(SystemEvent_t event)
{
    if (event != EV_NONE)
        state_handlers[g_currentState](event);
}

static void handle_state_idle(SystemEvent_t event)
{
    if (event == EV_START_PRESSED) {
        puts("动作: 从IDLE切换到RUNNING");
        g_currentState = STATE_RUNNING;
    }
}
static void handle_state_running(SystemEvent_t event)
{
    if (event == EV_STOP_PRESSED) {
        puts("动作: 从RUNNING切换到IDLE");
        g_currentState = STATE_IDLE;
    } else if (event == EV_FAULT_DETECTED) {
        puts("动作: 发生故障！切换到ERROR状态");
        g_currentState = STATE_ERROR;
    }
}
static void handle_state_error(SystemEvent_t event)
{
    if (event == EV_STOP_PRESSED) {
        puts("动作: 从ERROR状态恢复到IDLE");
        g_currentState = STATE_IDLE;
    }
}

int main(void)
{
    puts("=== 函数指针表状态机演示 ===");
    state_machine_handler(EV_START_PRESSED);
    state_machine_handler(EV_FAULT_DETECTED);
    state_machine_handler(EV_STOP_PRESSED);
    printf("最终状态: %d (0=IDLE)\n", g_currentState);
    return 0;
}
