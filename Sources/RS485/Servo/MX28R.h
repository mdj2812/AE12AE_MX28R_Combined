/*
 * MX28R.h
 *
 *  Created on: 4 nov. 2015
 *      Author: hanch_000
 */

#ifndef SOURCES_MX28R_H_
#define SOURCES_MX28R_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdint.h"
#include "psptypes.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//Every (THTL_REQUEST_NUMBER_TO_EXECUTE x 5) Ms will add (THTL_ADD_STEP)% to throttle
#define THTL_REQUEST_NUMBER_TO_EXECUTE  10
#define THTL_ADD_STEP                   2

/*******************************************************************************
 * Types
 ******************************************************************************/
typedef enum {
  MX28R_COMM_HEADER_NOT_DETECTED,
  MX28R_COMM_HEADER_FOUND_1,
  MX28R_COMM_HEADER_DETECTED,
} MX28R_HeaderState;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void thtlServoInit();

void MX28R_GetCheckSum(uint8_t * array);

uint8_t MX28R_SetGoalPosition(uint16_t pos);

uint8_t MX28R_SetThrottlePercent(uint8_t percent);

uint8_t MX28R_Read(uint8_t StartAddr, uint8_t * Value, uint8_t Length);

uint8_t MX28R_Write(uint8_t StartAddr, uint8_t * Value, uint8_t Length);

uint8_t MX28R_Reset();

uint8_t MX28R_GetActualSpeed(uint16_t* speed);

uint8_t MX28R_GetActualPosition(uint16_t* pos);

void MX28R_Calibration(uint16_t * low, uint16_t * high);

void MX28R_set_task();

boolean MX28R_OnCharReceived(uint8_t thtlRcvChar);

#endif /* SOURCES_MX28R_H_ */
