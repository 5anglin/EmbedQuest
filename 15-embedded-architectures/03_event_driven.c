/*
 * 架构三：事件驱动 (事件队列 + 函数指针分发)
 * 对应文章：《嵌入式软件架构的"十八般武艺"》
 * 编译： gcc -Wall -Wextra -o event_driven 03_event_driven.c
 */
#include <stdio.h>
#include <stdint.h>

typedef enum {
    EVT_BUTTON_PRESSED, EVT_TEMPERATURE_CHANGED, EVT_TIMER_EXPIRED,
    EVT_DATA_RECEIVED, EVT_MAX
} EventType;

typedef struct {
    EventType type;
    union {
        struct { uint8_t button_id; } button_event;
        struct { float temperature; } temp_event;
        struct { uint8_t timer_id; }  timer_event;
        struct { uint8_t data[16]; uint8_t length; } data_event;
    } data;
} Event;

#define EVENT_QUEUE_SIZE 16
static Event   event_queue[EVENT_QUEUE_SIZE];
static uint8_t queue_head = 0, queue_tail = 0;

typedef void (*EventHandler)(const Event *e);

static void HandleButtonEvent(const Event *e)
{ printf("  按钮事件: id=%u\n", e->data.button_event.button_id); }
static void HandleTemperatureEvent(const Event *e)
{ printf("  温度事件: %.1f\n", e->data.temp_event.temperature); }
static void HandleTimerEvent(const Event *e)
{ printf("  定时器事件: id=%u\n", e->data.timer_event.timer_id); }
static void HandleDataEvent(const Event *e)
{ printf("  数据事件: len=%u\n", e->data.data_event.length); }

static EventHandler event_handlers[EVT_MAX] = {
    [EVT_BUTTON_PRESSED]      = HandleButtonEvent,
    [EVT_TEMPERATURE_CHANGED] = HandleTemperatureEvent,
    [EVT_TIMER_EXPIRED]       = HandleTimerEvent,
    [EVT_DATA_RECEIVED]       = HandleDataEvent,
};

static int post_event(const Event *e)
{
    uint8_t next_tail = (queue_tail + 1) % EVENT_QUEUE_SIZE;
    if (next_tail == queue_head) return -1;   /* 满 */
    event_queue[queue_tail] = *e;
    queue_tail = next_tail;
    return 0;
}

int main(void)
{
    puts("=== 事件驱动架构演示 ===");
    Event e1 = { .type = EVT_BUTTON_PRESSED };  e1.data.button_event.button_id = 2;
    Event e2 = { .type = EVT_TEMPERATURE_CHANGED }; e2.data.temp_event.temperature = 36.5f;
    Event e3 = { .type = EVT_TIMER_EXPIRED };    e3.data.timer_event.timer_id = 1;
    post_event(&e1); post_event(&e2); post_event(&e3);

    /* 事件循环 */
    while (queue_head != queue_tail) {
        Event cur = event_queue[queue_head];
        queue_head = (queue_head + 1) % EVENT_QUEUE_SIZE;
        if (cur.type < EVT_MAX && event_handlers[cur.type])
            event_handlers[cur.type](&cur);
    }
    puts("事件队列已处理完毕。");
    return 0;
}
