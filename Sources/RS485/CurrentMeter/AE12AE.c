/*
 * AE12AE-0340115.c
 *
 *  Created on: 20 avr. 2017
 *      Author: MA
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "AE12AE.h"
#include "RS485_Driver.h"
#include "SEGGER_RTT.h"
#include "stdio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RESPONSE_BUFFER_SEIZE	(uint8_t)(20)

#define AE12AE_HEADER			(uint8_t)(0x2A)
#define AE12AE_ID				(uint8_t)(0x04)

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t AE12AE_Response[RESPONSE_BUFFER_SEIZE];
static uint16_t AE12AE_Response_Cursor = 0;
static uint8_t AE12AE_TxData[20];

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/
static void AE12AE_GetCheckSum(uint8_t * array);
static RS485ErrorType AE12AE_Read(uint8_t * Value);
static RS485ErrorType AE12AE_Write(uint8_t StartAddr, uint8_t Value);

/*******************************************************************************
 * Public function
 ******************************************************************************/
boolean AE12AE_Init() {
  LDD_TDeviceData* ALIM_12V_RLY_Ptr;

  ALIM_12V_RLY_Ptr = ALIM_12V_RLY_Init(NULL);

  while(AE12AE_Read(&AE12AE_Response[AE12AE_Response_Cursor]) != RS485_OK) {
	  ALIM_12V_RLY_ClrVal(ALIM_12V_RLY_Ptr);
	  RTOS_DELAY_TICKS(100);
	  ALIM_12V_RLY_SetVal(ALIM_12V_RLY_Ptr);
	  RTOS_DELAY_TICKS(10);

	  // Pass to mode 1
	  AE12AE_Write(0xD0, 0x01);
	  RTOS_DELAY_TICKS(10);
	  AE12AE_Write(0xD0, 0x08);
	  RTOS_DELAY_TICKS(10);
	  // Auto-calibration
	  AE12AE_Write(0xD0, 0x02);
	  RTOS_DELAY_TICKS(10);
  }

  return TRUE;
}

void AE12AE_task() {
  int counter = 0;
  uint16_t i = 0;
  uint16_t value = 0;
  char str[15];

  while(1) {
	/* Write your code here ... */
	AE12AE_Read(&AE12AE_Response[AE12AE_Response_Cursor]);
#ifdef DEBUG
	value = (AE12AE_Response[2]<<8 | AE12AE_Response[3]);
	sprintf(str, "%d\n", value);
	SEGGER_RTT_WriteString(0, str);
#endif
	RTOS_DELAY_TICKS(200);
  }
}

RS485FrameStateType AE12AE_FrameComposer(uint8_t AE12AERcvChar) {
  RS485FrameStateType ret = RS485_Frame_WORKING;
  static uint8_t i = 0;
  static uint16_t checksum = 0;
  static uint8_t id = 0;
  static uint8_t mesure_MSB = 0;
  static uint8_t mesure_LSB = 0;
  static AE12AE_HeaderState HeaderState = AE12AE_COMM_HEADER_NOT_DETECTED;

  switch (HeaderState) {
  case AE12AE_COMM_HEADER_NOT_DETECTED:
	if (AE12AERcvChar == AE12AE_HEADER) {
	  HeaderState = AE12AE_COMM_HEADER_DETECTED;
	  checksum += AE12AERcvChar;           //calculate checksum
	  AE12AE_Response[i] = AE12AERcvChar;
	  i++;                        //number of valid byte received
	}
	break;
  case AE12AE_COMM_HEADER_DETECTED:
	AE12AE_Response[i] = AE12AERcvChar;
	i++;                        //number of valid byte received
	switch (i) {
	case 2:     //ID
	  id = AE12AERcvChar;
	  break;
	case 3:     // MSB mesure
	  mesure_MSB = AE12AERcvChar;              //get mesure MSB
	  break;
	case 4:     // LSB mesure
	  mesure_LSB = AE12AERcvChar;
	  break;
	default:
		if(i == 5) {
			checksum = (AE12AE_HEADER + id + mesure_MSB + mesure_LSB) & 0xFF; //calculate checksum
			/* Clear the value prepare to next receive*/
			if(checksum == AE12AERcvChar) {
				ret = RS485_Frame_DONE;
			} else if((AE12AERcvChar == 0x2A) && ((AE12AE_HEADER + id + mesure_MSB) & 0xFF) == mesure_LSB) {
				ret = RS485_Frame_ERR_MODE;
			} else {
				ret = RS485_Frame_ERR;
			}
		} else {
			ret = RS485_Frame_ERR_MODE;
		}
		i = 0;
		checksum = 0;
		HeaderState = AE12AE_COMM_HEADER_NOT_DETECTED;
	  break;
	}
	break;
  }
  return ret;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
static void AE12AE_GetCheckSum(uint8_t * array) {
  uint8_t* ptr = array;
  uint8_t i = 0;
  uint16_t sum = 0;
  for (i = 0; i < 4; i++) {
    sum += *(ptr++);
  }
  *ptr = sum & 0xff;
}

static RS485ErrorType AE12AE_Read(uint8_t * Value) {
  RS485ErrorType ret;
  uint8_t i;

  AE12AE_TxData[0] = AE12AE_HEADER;
  AE12AE_TxData[1] = AE12AE_ID;
  AE12AE_TxData[2] = 0xD0;
  AE12AE_TxData[3] = 0x04; //Acquisition of measurement
  AE12AE_GetCheckSum(AE12AE_TxData);
  RS485_Write(AE12AE_TxData, 5U, RS485_User_CM);

  ret = RS485_WaitForResponse(100);
  if(ret == RS485_OK) {
    for(i = 0; i < 5; i++) {
      *(Value + i) = AE12AE_Response[i];
    }
  }

  return ret;
}

static RS485ErrorType AE12AE_Write(uint8_t StartAddr, uint8_t Value) {
  uint8_t i = 0;
  AE12AE_TxData[0] = AE12AE_HEADER;
  AE12AE_TxData[1] = AE12AE_ID;
  AE12AE_TxData[2] = StartAddr;
  AE12AE_TxData[3] = Value;
  AE12AE_GetCheckSum(AE12AE_TxData);

  return RS485_Write(AE12AE_TxData, 5U, RS485_User_CM);
}
