#ifndef _MAIN_H
#define _MAIN_H

#include "config.h"
#include <Arduino.h>


#define BOOL(x) (x = (x + 1) % 2)
#define BOARD_BTN PC13

extern uint8_t loraMessage[MESSAGE_SIZE];
extern uint8_t receivedMessage[MESSAGE_SIZE];

extern bool nextMessage;
extern uint8_t interruptState;
extern uint32_t currentTime;

#endif /* _MAIN_H */
