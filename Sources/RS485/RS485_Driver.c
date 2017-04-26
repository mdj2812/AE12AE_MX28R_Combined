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
    NULL, NULL, NULL, RS485_User_NONE, NULL
};
static uint8_t RS485RcvChar;

/******** Internal function declaration ********/

static inline RS485ErrorType RS485_Send(uint8_t* data, uint8_t len);

static inline RS485ErrorType RS485_Receive(uint8_t* result, uint8_t len);

/******** Internal function implementation ********/

static inline RS485ErrorType RS485_Send(uint8_t* data, uint8_t len) {
  RS485_DE_SetVal(RS485State.dePtr);
  RS485_RE_SetVal(RS485State.rePtr);
  if(RS485_UART_SendBlock(RS485State.uartPtr, data, len) == ERR_OK) {
	  return RS485_OK;
  } else {
	  return RS485_ERR;
  }
}

static inline RS485ErrorType RS485_Receive(uint8_t* result, uint8_t len) {
  if(RS485_UART_ReceiveBlock(RS485State.uartPtr, result, len) == ERR_OK) {
	  return RS485_OK;
  } else {
	  return RS485_ERR;
  }
}

/******** Public function implementation ********/

void RS485_Init() {
  RS485State.uartPtr = RS485_UART_Init(NULL);
  RS485State.dePtr = RS485_DE_Init(NULL);
  RS485State.rePtr = RS485_RE_Init(NULL);
  _lwevent_create(&RS485State.lwevent_RS485Rx, 0);

  RS485_Read(&RS485RcvChar, 1U, RS485_User_NONE);
}

RS485ErrorType RS485_Read(uint8_t* result, uint8_t len, RS485UserType user) {
  while((RS485State.user | user) != user) {
    RTOS_DELAY_MINIUM();
  }
  RS485State.user = user;
/* Receive InpData (1 byte) from the I2C bus and generates a stop condition to end transmission */
  return RS485_Receive(result, len);
}

RS485ErrorType RS485_Write(uint8_t* data, uint8_t len, RS485UserType user) {
  while((RS485State.user | user) != user) {
    RTOS_DELAY_MINIUM();
  }
  RS485State.user = user;
  /* Send */
  return RS485_Send(data, len);
}

RS485ErrorType RS485_WaitForResponse(uint32_t timeoutTicks) {
  RS485ErrorType resType;
  uint_32 ret;

  _lwevent_clear(&RS485State.lwevent_RS485Rx, 0xFFFFFFFF);
  if (_lwevent_wait_ticks(&RS485State.lwevent_RS485Rx, 0xFFFFFFFF, FALSE, timeoutTicks) == LWEVENT_WAIT_TIMEOUT) {
    //time_out
    resType = RS485_TIMEOUT;
  } else {
    if (RS485State.lwevent_RS485Rx.VALUE == RS485_Frame_DONE) {
      //success
      resType = RS485_OK;
    } else if (RS485State.lwevent_RS485Rx.VALUE == RS485_Frame_ERR_MODE){
      // need reset
      resType = RS485_ERR_MODE;
    } else {
      //Checksum error
      resType = RS485_ERR;
    }
  }
  _lwevent_clear(&RS485State.lwevent_RS485Rx, 0xFFFFFFFF);
  RS485State.user = RS485_User_NONE;

  return resType;
}

void RS485_OnBlockRx() {
  RS485FrameStateType ret;
  static uint8_t errorCounter = 0;

  switch(RS485State.user) {
	  case RS485_User_NONE:
		ret = RS485_Frame_WORKING;
		break;
  	  case RS485_User_SERVO:
  		ret = MX28R_FrameComposer(RS485RcvChar);
  		break;
  	  case RS485_User_CM:
		ret = AE12AE_FrameComposer(RS485RcvChar);
  		break;
  	  default:
  		ret = RS485_Frame_ERR;
  		break;
  }

  if(ret) {
	  _lwevent_set(&RS485State.lwevent_RS485Rx, ret);
  }

  RS485_Read(&RS485RcvChar, 1U, RS485State.user);
}

void RS485_OnTxCompleted() {
  RS485_DE_ClrVal(RS485State.dePtr);
  RS485_RE_ClrVal(RS485State.rePtr);
}
