/*
 * MX28R.h
 *
 *  Created on: 4 nov. 2015
 *      Author: hanch_000
 */

#ifndef SOURCES_AE12AE_H_
#define SOURCES_AE12AE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include "RS485_Driver.h"

/*******************************************************************************
 * Types
 ******************************************************************************/
typedef enum {
	AE12AE_COMM_HEADER_NOT_DETECTED,
	AE12AE_COMM_HEADER_DETECTED
}AE12AE_HeaderState;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
boolean AE12AE_Init();

void AE12AE_task();

RS485FrameStateType AE12AE_FrameComposer(uint8_t AE12AERcvChar);

#endif /* SOURCES_AE12AE_H_ */
