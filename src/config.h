#ifndef _CONFIG_H
#define _CONFIG_H

// MAIN SOFTWARE CONFIGURATIONS
#define MODE_PIN PA0

// the commented #DEINES were used for SOFTWARE configuration of the MASTER and SLAVE.
// #define SLAVE 1u
// #define MASTER 0u

// #if (MASTER == 1u)
//   #define SW_TYPE MASTER
// #elif (SLAVE == 1u)
//   #define SW_TYPE SLAVE
// #endif

// COM CONFIGURATIONS
#define SERIAL_SPEED 115200u

#define SEND_PERIOD_MS 5000u

#define MESSAGE_SIZE 8u

// BME CONFIG

#define SENSOR_ADDR 0x77
#define SEA_LEVEL_PRESSURE 1013.25


#endif
