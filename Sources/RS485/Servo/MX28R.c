/*
 * MX28R.c
 *
 *  Created on: 4 nov. 2015
 *      Author: hanch_000
 */

#include "MX28R.h"
#include "RS485_DE.h"
#include "RS485_RE.h"
#include "RS485_UART.h"
#include "Events.h"
#include "stdlib.h"
#include "RS485_Driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
// Torque limit of the motor
#define MX28R_TORQUE_MAX                (uint16_t)(0x0200)
#define MX28R_TORQUE_MIN                (uint16_t)(0x0100)
#define MX28R_TORQUE_PRECISION          (uint16_t)(0x0010)
// Moving speed of the motor
#define MX28R_MOVING_SPEED              (uint16_t)(0x0200)
// Minimal moving speed of the motor
#define MX28R_SPEED_MIN                 (uint16_t)(0x0070)

// Positions
#define MX28R_CW_ANGLE_LIMIT            (uint16_t)(0x0000)
#define MX28R_CCW_ANGLE_LIMIT           (uint16_t)(0x0FFF)
#define MX28R_INITIAL_POSITION          (uint16_t)(0x07E8)
#define MX28R_START_POSITION            (uint16_t)(0x0000)
#define MX28R_END_POSITION              (uint16_t)(0x0FFF)
#define MX28R_POS_PRECISION             (uint16_t)(0x0010)
#define RESPONSE_BUFFER_SEIZE           (uint8_t)(20)

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t TxData[20];
static uint8_t MX28R_Response[RESPONSE_BUFFER_SEIZE ];
static uint16_t thtl0Pos;
static uint16_t thtl100Pos;
static int16_t FinalThtl;

void thtlServoInit() {
  /* Minimum of the CW angle */
  uint16_t angleLimitCW = MX28R_CW_ANGLE_LIMIT;
  /* Maximum of the CCW angle */
  uint16_t angleLimitCCW = MX28R_CCW_ANGLE_LIMIT;
  /* Limit of the torque */
  uint16_t torqueLimit = MX28R_TORQUE_MAX;
  /* Moving speed */
  uint16_t movingSpeed = MX28R_MOVING_SPEED;
//  uint8_t baud = 16;
//  MX28R_Write(0x04, &baud, 1);
//  MX28R_Reset();
  /* Set the CW limit to the THTL_CW_ANGLE_LIMIT */
  MX28R_Write(0x06, (uint8_t*) &angleLimitCW, 2);
  /* Set the CCW limit to the THTL_CCW_ANGLE_LIMIT */
  MX28R_Write(0x08, (uint8_t*) &angleLimitCCW, 2);
  /* Set the moving speed to the movingSpeed */
  MX28R_Write(0x20, (uint8_t*) &movingSpeed, 2);
  /* Set the Max Torque to the torqueLimit */
  MX28R_Write(0x0E, (uint8_t*) &torqueLimit, 2);

  MX28R_Calibration(&thtl0Pos, &thtl100Pos);
}

void MX28R_GetCheckSum(uint8_t * array) {
  uint8_t* ptr = array;
  uint8_t i = 0;
  uint16_t sum = 0;
  ptr += 2; //ID
  for (i = 0; i < (*(array + 3) + 1); i++) {
    sum += *(ptr++);
  }
  *ptr = ~(sum & 0xff);
}

uint8_t MX28R_SetGoalPosition(uint16_t pos) {
  TxData[0] = 0xFF;
  TxData[1] = 0xFF;
  TxData[2] = 0x01;
  TxData[3] = 0x05;
  TxData[4] = 0x03;
  TxData[5] = 0x1E;
  TxData[6] = pos & 0xff;
  TxData[7] = (pos & 0xff00) >> 8;
  MX28R_GetCheckSum(TxData); // data8
  RS485_Write(TxData, 9U, RS485_User_SERVO);

  return RS485_OK == RS485_WaitForResponse(200);
}

uint8_t MX28R_SetThrottlePercent(uint8_t percent) {
  return MX28R_SetGoalPosition(thtl0Pos + percent * (thtl100Pos - thtl0Pos) * (thtl100Pos > thtl0Pos) / 100);
}

uint8_t MX28R_Read(uint8_t StartAddr, uint8_t * Value, uint8_t Length) {
  uint8_t i;
  TxData[0] = 0xFF;
  TxData[1] = 0xFF;
  TxData[2] = 0x01;
  TxData[3] = 0x04; //Frame Length
  TxData[4] = 0x02; //Instruction: Read
  TxData[5] = StartAddr;
  TxData[6] = Length;
  MX28R_GetCheckSum(TxData);
  RS485_Write(TxData, 8U, RS485_User_SERVO);

  if (RS485_WaitForResponse(200) == RS485_OK) {
    for (i = 0; i < Length; i++) {
      *(Value + i) = MX28R_Response[4 + i];
    }
    return 1;
  } else {
    return 0;
  }
}

uint8_t MX28R_Write(uint8_t StartAddr, uint8_t * Value, uint8_t Length) {
  uint8_t i = 0;
  TxData[0] = 0xFF;
  TxData[1] = 0xFF;
  TxData[2] = 0x01;
  TxData[3] = Length + 3;
  TxData[4] = 0x03;
  TxData[5] = StartAddr;
  for (i = 0; i < Length; i++)
    TxData[6 + i] = Value[i];
  MX28R_GetCheckSum(TxData);
  RS485_Write(TxData, Length + 7, RS485_User_SERVO);

  return RS485_OK == RS485_WaitForResponse(200);
}

uint8_t MX28R_Reset() {
  uint8_t i = 0;
  TxData[0] = 0xFF;
  TxData[1] = 0xFF;
  TxData[2] = 0x01;
  TxData[3] = 0x02;
  TxData[4] = 0x06;
  MX28R_GetCheckSum(TxData);
  RS485_Write(TxData, 6, RS485_User_SERVO);

  return RS485_OK == RS485_WaitForResponse(200);
}

void MX28R_GetActualSpeed(uint16_t* speed) {
  MX28R_Read(0x26, (uint8_t*)speed, 2);
}

void MX28R_GetActualPosition(uint16_t* pos) {
  MX28R_Read(0x24, (uint8_t*)pos, 2);
}

void MX28R_Calibration(uint16_t * low, uint16_t * high) {
  /* Limit of the torque */
  static uint16_t torqueLimit = MX28R_TORQUE_MIN;
  static uint16_t speed;
  /***************************Get the max torque limit *****************************/
  /* Try to go back to the THTL_INITIAL_POSITION */
  MX28R_SetGoalPosition(MX28R_INITIAL_POSITION);
  /* Wait till it stopped */
  do {
    /* Wait 10 ticks = 50 Ms */
    _time_delay_ticks(10);
    MX28R_GetActualSpeed(&speed);
  }
  while (speed);
  /* Set the position to END_POSITION */
  MX28R_SetGoalPosition(MX28R_END_POSITION);
  do {
    /* Calibrate torque */
    if((speed & ~0xFC00) < MX28R_SPEED_MIN  && torqueLimit < MX28R_TORQUE_MAX ) {
      torqueLimit += MX28R_TORQUE_PRECISION;
    }
    /* Set the Torque Limit to the torqueLimit */
    MX28R_Write(0x22, (uint8_t*) &torqueLimit, 2);
    /* Wait 10 ticks = 50 Ms */
    _time_delay_ticks(10);
    MX28R_GetActualSpeed(&speed);
  }
  while (speed);
  MX28R_GetActualPosition(high);
  /*=================================================================================*/

  /*****************************Get the 0% position***********************************/
  /* Return the position to start position */
  MX28R_SetGoalPosition(MX28R_START_POSITION);
  /* Cycle till the end of the movement */
  do {
    /* Wait 10 ticks = 50 Ms */
    _time_delay_ticks(10);
    MX28R_GetActualSpeed(&speed);
  }
  while (speed);
  MX28R_GetActualPosition(low);
  MX28R_SetGoalPosition(*low);
  /*=================================================================================*/
}

void MX28R_set_task() {
  int8_t thtlBuffer = 0xFF;
  uint8_t thtlSubBuffer = 0x0;
  uint16_t thtlOrder;
  bool thtlNeedChange;
  uint8_t thtlRequestCounter = 0;
  int8_t Error;
  thtlServoInit();
  while (1) {
	/*Set Throttle*/
	if (FinalThtl < 0)
	  FinalThtl = 0;
	if (FinalThtl > 100)
	  FinalThtl = 100;
	if (FinalThtl != thtlBuffer) {
	  // requested throttle is different from actual one
	  if (thtlBuffer > FinalThtl) {
		//request reduction of throttle, execute immediately
		thtlBuffer = FinalThtl;
		//Mark that throttle need to be changed
		thtlNeedChange = TRUE;
	  } else {
		//request augmentation of throttle, execute by several steps
		thtlRequestCounter++;
		if (thtlRequestCounter == THTL_REQUEST_NUMBER_TO_EXECUTE) {
		  //Every (THTL_REQUEST_NUMBER_TO_EXECUTE x 5) Ms will add (THTL_ADD_STEP)% to throttle
		  thtlRequestCounter = 0;
		  //Mark that throttle need to be changed
		  thtlNeedChange = TRUE;
		  //Make add to the throttle, until it's bigger than target throttle
		  thtlBuffer += THTL_ADD_STEP;
		  if (thtlBuffer > FinalThtl)
			thtlBuffer = FinalThtl;
		}
	  }
	  if (thtlNeedChange) {
		thtlNeedChange = FALSE;
		//thtlOrder = thtlBuffer ? 1808 + thtlBuffer * 432 / 100 : 1760;
		MX28R_SetThrottlePercent(thtlBuffer);
	  }
	}
	_time_delay_ticks(1); // 5 ms step
  }
}

RS485FrameStateType MX28R_FrameComposer(uint8_t thtlRcvChar) {
  /* Return packet format */
  /* 0xFF 0xFF ID LEN ERROR PARA... CHECKSUM */
  /* Initialization */
  static uint8_t length = 0;
  static uint8_t i = 0;
  static uint16_t checksum = 0;
  static uint8_t error = 0;
  static MX28R_HeaderState HeaderState = MX28R_COMM_HEADER_NOT_DETECTED;
  switch (HeaderState) {
  case MX28R_COMM_HEADER_NOT_DETECTED:
    if (thtlRcvChar == 0xFF) {
      HeaderState = MX28R_COMM_HEADER_FOUND_1;
    }
    break;
  case MX28R_COMM_HEADER_FOUND_1:
    if (thtlRcvChar == 0xFF) {
      HeaderState = MX28R_COMM_HEADER_DETECTED;
    } else {
      HeaderState = MX28R_COMM_HEADER_NOT_DETECTED;
    }
    break;
  case MX28R_COMM_HEADER_DETECTED:
    i++;                        //number of valid byte received
    if (i != (2 + length) || length == 0) {
      checksum += thtlRcvChar;           //calculate checksum
    }
    MX28R_Response[i] = thtlRcvChar;
    switch (i) {
    case 1:     //ID
      break;
    case 2:     //LEN
      length = thtlRcvChar;              //get length
      break;
    case 3:     //ERR
      error = thtlRcvChar;
      break;
    default:
      if (i == (2 + length)) { //CHECKSUM
        checksum = (~(checksum & 0xFF)) & 0xFF; //calculate checksum
        i = 0;
        HeaderState = MX28R_COMM_HEADER_NOT_DETECTED;

        if(error << 1 || checksum == thtlRcvChar) {
            /* Clear the value prepare to next receive*/
            checksum = 0;
            error = 0;

            return RS485_Frame_DONE;
        } else {
            checksum = 0;
            error = 0;

            return RS485_Frame_ERR;
        }
      }
      break;
    }
    break;
  }

  return RS485_Frame_WORKING;
}

void MX28R_SetAndWaitGoalPos(uint16_t targetPos) {
  uint16_t actualPos = 0;
  uint16_t actualShadowPos = 0;
  MX28R_SetGoalPosition(targetPos);
  // wait
  while (abs(actualPos - targetPos) > MX28R_POS_PRECISION) {
    if (actualShadowPos == 0) {
      MX28R_GetActualPosition(&actualPos);
      actualShadowPos = actualPos;
    } else {
      _time_delay_ticks(10);
      MX28R_GetActualPosition(&actualPos);
      if (actualShadowPos == actualPos) {
        break;
      } else {
        actualShadowPos = actualPos;
      }
    }
  }
}

uint16_t MX28R_SearchMin() {
  uint16_t targetPos;
  uint16_t actualPos;
  MX28R_GetActualPosition(&actualPos);
  targetPos = actualPos;
  // wait
  while (abs(actualPos - targetPos) < MX28R_POS_PRECISION && targetPos != 0) {
    if (targetPos > MX28R_POS_PRECISION) {
      targetPos -= MX28R_POS_PRECISION;
    } else {
      targetPos = 0;
    }
    MX28R_SetGoalPosition(targetPos);
    _time_delay_ticks(10);
    MX28R_GetActualPosition(&actualPos);
  }
  return actualPos;
}

uint16 MX28R_SearchMax() {
  uint16_t targetPos;
  uint16_t actualPos;
  MX28R_GetActualPosition(&actualPos);
  targetPos = actualPos;
  // wait
  while (abs(actualPos - targetPos) < MX28R_POS_PRECISION && targetPos != 0xFFF) {
    if (targetPos < 0xFFF - MX28R_POS_PRECISION) {
      targetPos += MX28R_POS_PRECISION;
    } else {
      targetPos = 0xFFF;
    }
    MX28R_SetGoalPosition(targetPos);
    _time_delay_ticks(10);
    MX28R_GetActualPosition(&actualPos);
  }
  return actualPos;
}
