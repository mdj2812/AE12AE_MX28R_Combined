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
#include "RS485_Driver.h"

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
void MX28R_set_task();

RS485FrameStateType MX28R_FrameComposer(uint8_t thtlRcvChar);

#endif /* SOURCES_MX28R_H_ */
