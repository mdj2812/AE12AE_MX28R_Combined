/*
 * I2C_Driver.c
 *
 *  Created on: 11 avr. 2017
 *      Author: hanch_000
 */

#include "RS485_Driver.h"
#include "RS485_DE.h"
#include "RS485_RE.h"
#include "RS485_UART.h"
#include "Events.h"
#include "MX28R.h"
#include "AE12AE.h"

/* Definition of I2C state structure */
typedef struct {
  LDD_TDeviceData* uartPtr;
  LDD_TDeviceData* dePtr;
  LDD_TDeviceData* rePtr;
  RS485UserType user;
  LWEVENT_STRUCT lwevent_RS485Rx;
} RS485_Type_State;

/******** Internal variables ********/

static RS485_Type_State RS485State = {
    NULL, NULL, NULL, RS485_None, NULL
};
static uint8_t RS485RcvChar;

/******** Internal function declaration ********/

static inline void RS485_Send(uint8_t* data, uint8_t len);

static inline void RS485_Receive(uint8_t* result, uint8_t len);

/******** Internal function implementation ********/

static inline void RS485_Send(uint8_t* data, uint8_t len) {
  RS485_DE_SetVal(RS485State.dePtr);
  RS485_RE_SetVal(RS485State.rePtr);
  RS485_UART_SendBlock(RS485State.uartPtr, data, len);
}

static inline void RS485_Receive(uint8_t* result, uint8_t len) {
  RS485_UART_ReceiveBlock(RS485State.uartPtr, result, len);
}

/******** Public function implementation ********/

void RS485_Init() {
  RS485State.uartPtr = RS485_UART_Init(NULL);
  RS485State.dePtr = RS485_DE_Init(NULL);
  RS485State.rePtr = RS485_RE_Init(NULL);
  _lwevent_create(&RS485State.lwevent_RS485Rx, 0);

  RS485_Read(&RS485RcvChar, 1U, RS485_None);
}

void RS485_Read(uint8_t* result, uint8_t len, RS485UserType user) {
  while((RS485State.user | user) != user) {
    RTOS_DELAY_MINIUM();
  }
  RS485State.user = user;
/* Receive InpData (1 byte) from the I2C bus and generates a stop condition to end transmission */
  RS485_Receive(result, len);
}

void RS485_Write(uint8_t* data, uint8_t len, RS485UserType user) {
  while((RS485State.user | user) != user) {
    RTOS_DELAY_MINIUM();
  }
  RS485State.user = user;
  /* Send */
  RS485_Send(data, len);
}

RS485ResponseType RS485_WaitForResponse(uint32_t timeoutTicks) {
  RS485ResponseType resType;

  _lwevent_clear(&RS485State.lwevent_RS485Rx, 0xFFFFFFFF);
  if (_lwevent_wait_ticks(&RS485State.lwevent_RS485Rx, 0xFFFFFFFF, FALSE, timeoutTicks) == LWEVENT_WAIT_TIMEOUT) {
    //time_out
    resType = RS485_TIMEOUT;
  } else {
    if (RS485State.lwevent_RS485Rx.VALUE == 0x01) {
      //success
      resType = RS485_OK;
    } else {
      //Checksum error
      resType = RS485_ERR;
    }
  }
  _lwevent_clear(&RS485State.lwevent_RS485Rx, 0xFFFFFFFF);
  RS485State.user = RS485_None;

  return resType;
}

RS485UserType RS485_GetUser() {
  return RS485State.user;
}

void RS485_OnBlockRx() {
  switch(RS485State.user) {
  	  case RS485_Servo:
  		if(MX28R_OnCharReceived(RS485RcvChar)) {
  	        _lwevent_set(&RS485State.lwevent_RS485Rx, 0x01);
  		}
  		break;
  	  case RS485_CurrentMeter:
  		if(crrntMtrR_OnCharReceived(RS485RcvChar)) {
  	        _lwevent_set(&RS485State.lwevent_RS485Rx, 0x01);
  		}
  		break;
  	  default:
  		break;
  }

  RS485_Read(&RS485RcvChar, 1U, RS485State.user);
}

void RS485_OnTxCompleted() {
  RS485_DE_ClrVal(RS485State.dePtr);
  RS485_RE_ClrVal(RS485State.rePtr);
}
