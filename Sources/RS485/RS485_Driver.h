/*
 * I2C_Driver.h
 *
 *  Created on: 11 avr. 2017
 *      Author: hanch_000
 */

#ifndef RS485_DRIVER_H_
#define RS485_DRIVER_H_

#include "stdint.h"
/******** Public macros ********/

#define         BIT0                    0x01
#define         BIT1                    0x02
#define         BIT2                    0x04
#define         BIT3                    0x08
#define         BIT4                    0x10
#define         BIT5                    0x20
#define         BIT6                    0x40
#define         BIT7                    0x80

#define         RTOS_DELAY_MINIUM()     _time_delay_ticks(1)

#define			DEBUG
/******** Public types ********/
typedef enum {
	RS485_User_NONE = 0, RS485_User_SERVO = BIT0, RS485_User_CM = BIT1
} RS485UserType;

typedef enum {
	RS485_OK, RS485_ERR, RS485_TIMEOUT
} RS485ResponseType;

typedef enum {
	RS485_Frame_WORKING = 0, RS485_Frame_DONE = BIT0, RS485_Frame_ERR = BIT1
} RS485FrameStateType;

/******** Public variables ********/

/******** Public function declaration ********/

void RS485_Init();

void RS485_Read(uint8_t* result, uint8_t len, RS485UserType user);

void RS485_Write(uint8_t* data, uint8_t len, RS485UserType user);

RS485ResponseType RS485_WaitForResponse(uint32_t timeoutTicks);

RS485UserType RS485_GetUser();

void RS485_OnBlockRx();

void RS485_OnTxCompleted();

#endif /* RS485_DRIVER_H_ */
