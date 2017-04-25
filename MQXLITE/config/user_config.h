/** ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : user_config.h
**     Project     : AE12AE_MX28R_Combined
**     Processor   : MK64FN1M0VLQ12
**     Version     : Component 01.110, Driver 01.00, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-04-25, 10:25, # CodeGen: 2
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file user_config.h                                                  
** @version 01.00
*/         
/*!
**  @addtogroup user_config_module user_config module documentation
**  @{
*/         
#ifndef __user_config_h__
#define __user_config_h__

/* MQX Lite version number */
#define MQX_LITE_VERSION               "V1.1.1"
#define MQX_LITE_VERSION_NUMBER        0x01010001

/* Selected MQX CPU type */
#define MQX_CPU              PSP_CPU_NUM(PSP_CPU_ARCH_ARM_CORTEX_M4, 0x00, 0x00)

/* Component settings */
#define MQX_USE_SEMAPHORES             1
#define MQX_USE_LWEVENTS               1
#define MQX_USE_MUTEXES                0
#define MQX_USE_LWMSGQ                 0
#define MQX_USE_LWTIMER                0
#define MQX_KERNEL_LOGGING             0

/* Idle task settings */
#define MQX_USE_IDLE_TASK              1
#define MQX_ENABLE_LOW_POWER           0
#define MQX_ENABLE_IDLE_LOOP           0


/* Interrupt settings */
#define MQX_ROM_VECTORS                1
#define MQX_INTERRUPT_STACK_SIZE       512

/* PSP Settings */
#define MQXCFG_ENABLE_FP               0
#define PSP_HAS_FPU                    1
#define PSP_MAXIMUM_INTERRUPT_VECTORS  (sizeof(__vect_table)/sizeof(tIsrFunc))


/* Additional settings can be defined in the property User Definitions > Definitions of the MQXLite RTOS component */

#endif /* __user_config_h__ */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
