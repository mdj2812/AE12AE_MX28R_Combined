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
#include "PE_Types.h"
#include "Events.h"
#include "mqx_tasks.h"
#include "Init_Config.h"
#include "PDD_Includes.h"
#include "Cpu.h"
#include "RS485_Driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RESPONSE_BUFFER_SEIZE	(uint8_t)(20)

/*******************************************************************************
 * Types
 ******************************************************************************/
typedef enum {
	CRRNTMTR_COMM_HEADER_NOT_DETECTED,
	CRRNTMTR_COMM_HEADER_DETECTED
}CrrntMtr_HeaderState;

/*******************************************************************************
 * Variables
 ******************************************************************************/
LDD_TDeviceData* AE12AE_RS485_Ptr;
LDD_TDeviceData* RS485_DE_Ptr;
LDD_TDeviceData* RS485_RE_Ptr;
LWEVENT_STRUCT lwevent_dmndMsr;
uint8_t crrntMtr_Response[RESPONSE_BUFFER_SEIZE];

uint8_t TxData[20];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void crrntMtr_GetCheckSum(uint8_t * array);

uint8_t crrntMtr_Read(uint8_t id, uint8_t * Value);

uint8_t crrntMtr_Write(uint8_t id, uint8_t StartAddr, uint8_t Value);

RS485FrameStateType crrntMtr_FrameComposer(uint8_t crrntMtrRcvChar);

void CurrentMeter_task();
#endif /* SOURCES_AE12AE_H_ */
