#ifndef _MAIN_H
#define _MAIN_H

#include "pinout.h"
#include "config.h"


#define BOOL(x) (x = (x + 1) % 2)

extern bool BME_ACTIVE;

//extern uint8_t loraMessage[MESSAGE_SIZE];
//extern uint8_t receivedMessage[MESSAGE_SIZE];
//extern bool nextMessage;
//extern uint8_t interruptState;
//extern uint32_t currentTime;

void ButtonClickInterrupt(void);
uint8_t initRegs(void);


#endif /* _MAIN_H */
