#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

typedef enum
{
    EVENT_LEFT = 0,
    EVENT_RIGHT = 1
} EventType;

void EventQueue_Init(void);
uint8_t EventQueue_Push(EventType event);
uint8_t EventQueue_Pop(EventType *event);

#endif
