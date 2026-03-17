#include "event.h"

#include <stm32f1xx_hal.h>

#define EVENT_QUEUE_SIZE 16U

static volatile EventType s_queue[EVENT_QUEUE_SIZE];
static volatile uint8_t s_head = 0U;
static volatile uint8_t s_tail = 0U;
static volatile uint8_t s_count = 0U;

void EventQueue_Init(void){
    __disable_irq();
    s_head = 0U;
    s_tail = 0U;
    s_count = 0U;
    __enable_irq();
}

uint8_t EventQueue_Push(EventType event){
    uint8_t ok = 0U;

    __disable_irq();
    if (s_count < EVENT_QUEUE_SIZE){
        s_queue[s_head] = event;
        s_head = (uint8_t)((s_head + 1U) % EVENT_QUEUE_SIZE);
        s_count++;
        ok = 1U;
    }
    __enable_irq();

    return ok;
}

uint8_t EventQueue_Pop(EventType *event){
    uint8_t ok = 0U;

    if (event == NULL){
        return 0U;
    }

    __disable_irq();
    if (s_count > 0U){
        *event = s_queue[s_tail];
        s_tail = (uint8_t)((s_tail + 1U) % EVENT_QUEUE_SIZE);
        s_count--;
        ok = 1U;
    }
    __enable_irq();

    return ok;
}
