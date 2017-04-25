/*
 * AE12AE-0340115.c
 *
 *  Created on: 20 avr. 2017
 *      Author: MA
 */

#include "AE12AE.h"
#include "string.h"
#include "RS485_DE.h"
#include "RS485_RE.h"
#include "RS485_UART.h"
#include "Events.h"
#include "RS485_Driver.h"
#include "SEGGER_RTT.h"
#include "stdio.h"

void crrntMtr_GetCheckSum(uint8_t * array) {
  uint8_t* ptr = array;
  uint8_t i = 0;
  uint16_t sum = 0;
  for (i = 0; i < 4; i++) {
    sum += *(ptr++);
  }
  *ptr = sum & 0xff;
}

uint8_t crrntMtr_Read(uint8_t id, uint8_t * Value) {
  uint8_t i;
  TxData[0] = 0x2A;
  TxData[1] = id;
  TxData[2] = 0xD0;
  TxData[3] = 0x04; //Acquisition of measurement
  crrntMtr_GetCheckSum(TxData);
  RS485_Write(TxData, 5U, RS485_CurrentMeter);

  if(RS485_WaitForResponse(100) == RS485_OK) {
    for(i = 0; i < 5; i++) {
      *(Value + i) = crrntMtr_Response[i];
    }
    return 1;
  } else {
    return 0;
  }
}

uint8_t crrntMtr_Write(uint8_t id, uint8_t StartAddr, uint8_t Value) {
  uint8_t i = 0;
  TxData[0] = 0x2A;
  TxData[1] = id;
  TxData[2] = StartAddr;
  TxData[3] = Value;
  crrntMtr_GetCheckSum(TxData);
  RS485_Write(TxData, 5U, RS485_CurrentMeter);

  return 1;
}

boolean crrntMtrR_OnCharReceived(uint8_t crrntMtrRcvChar) {
  static uint8_t i = 0;
  static uint16_t checksum = 0;
  static uint8_t id = 0;
  static uint8_t mesure_MSB = 0;
  static uint8_t mesure_LSB = 0;
  static CrrntMtr_HeaderState HeaderState = CRRNTMTR_COMM_HEADER_NOT_DETECTED;
  switch (HeaderState) {
  case CRRNTMTR_COMM_HEADER_NOT_DETECTED:
	if (crrntMtrRcvChar == 0x2A) {
	  HeaderState = CRRNTMTR_COMM_HEADER_DETECTED;
	  checksum += crrntMtrRcvChar;           //calculate checksum
	  crrntMtr_Response[i] = crrntMtrRcvChar;
	  i++;                        //number of valid byte received
	}
	break;
  case CRRNTMTR_COMM_HEADER_DETECTED:
	crrntMtr_Response[i] = crrntMtrRcvChar;
	i++;                        //number of valid byte received
	switch (i) {
	case 2:     //ID
	  id = crrntMtrRcvChar;
	  break;
	case 3:     // MSB mesure
	  mesure_MSB = crrntMtrRcvChar;              //get mesure MSB
	  break;
	case 4:     // LSB mesure
	  mesure_LSB = crrntMtrRcvChar;
	  break;
	case 5:
		checksum = (0x2A + id + mesure_MSB + mesure_LSB) & 0xFF; //calculate checksum
		/* Clear the value prepare to next receive*/
		if(checksum == crrntMtrRcvChar) {
			i = 0;
			checksum = 0;
			HeaderState = CRRNTMTR_COMM_HEADER_NOT_DETECTED;
			return TRUE;
		} else {
			i = 0;
			checksum = 0;
			HeaderState = CRRNTMTR_COMM_HEADER_NOT_DETECTED;
			return FALSE;
		}
		break;
	default:
	  break;
	}
	break;
  }
  return FALSE;
}

void CurrentMeter_task() {
  int counter = 0;
  uint16_t i = 0;
  uint16_t value = 0;
  static uint8_t RX_Buffer[200];
  static uint16_t RX_Cursor = 0;
  char str[15];

  for(i = 0; i<200; i++) {
	  RX_Buffer[i] = 0;
  }

  crrntMtr_Write(0x04, 0xD0, 0x01);
  _time_delay_ticks(10);
  crrntMtr_Write(0x04, 0xD0, 0x08);
  _time_delay_ticks(10);
  crrntMtr_Write(0x04, 0xD0, 0x02);
  _time_delay_ticks(10);

  while(1) {
	/* Write your code here ... */
	crrntMtr_Read(0x04, &RX_Buffer[RX_Cursor]);
	value = (RX_Buffer[2]<<8 | RX_Buffer[3]);
	sprintf(str, "%d\n", value);

	SEGGER_RTT_WriteString(0, str);
	_time_delay_ticks(200);
  }
}
