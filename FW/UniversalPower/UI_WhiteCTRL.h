/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, GTSV Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of GTSV Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#ifndef __UI_WHITECTRL_H_
#define __UI_WHITECTRL_H_


//! \file   UI_WhiteCTRL.h
//! \brief  Contains global definitions & vars
//!
//!
//! (C) Copyright 2013, GTSV, Inc.


// **************************************************************************
// the includes

#include "main.h"



//!
//!
//! \defgroup UI_WhiteCTRL UI_WhiteCTRL
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the define
#define TIME_ADJ_DELAY_DEFAULT	88

#define CTRL_RUNNING_AUTO_POWER_OFF_SEC	10800

#define CTRL_SHORT_TICK_LENGH_50MS	3
#define CTRL_LONG_TICK_LENGH_50MS	108

#define STATE_OFF			((_Bool)(0))
#define STATE_ON			((_Bool)(!STATE_OFF))

#define CTRL_VERSION_MAJOR	(0)
#define CTRL_VERSION_MINOR	(1)
// **************************************************************************
// the typedefs

//! \brief Enumeration for the error codes
//!
typedef enum
{
  CTRL_ErrorCode_NoError=0,        //!< no error error code
  CTRL_ErrorCode_Unknown,           //!< Id clip error code
  CTRL_numErrorCodes               //!< the number of controller error codes
} CTRL_ErrorCode_e;


//! \brief Enumeration for the Control States
//!
typedef enum
{
	CTRL_State_Error = 0,
	CTRL_State_Idle,					 //!<
	CTRL_State_Blowing_APO_Time_Adj,
	CTRL_State_On,
	CTRL_State_Blowing,
	CTRL_State_Blowing_Auto_Power_Off,
	CTRL_State_Auto_Power_Off_Delay_Time_Adj,
	CTRL_State_Clk_Adj,
	CTRL_numStates
} CTRL_State_e;

//! \brief Defines the controller (CTRL) version number
//!
typedef struct _CTRL_Version_
{
  uint16_t rsvd;            //!< reserved value
  uint16_t targetProc;      //!< the target processor
  uint16_t major;           //!< the major release number
  uint16_t minor;           //!< the minor release number
} CTRL_Version;


//! \brief      Defines the controller (CTRL) object
//! \details    
//!            
typedef struct _CTRL_Obj_
{
	CTRL_Version		version;             //!< the controller version

	CTRL_State_e		state;			//!< the current state of the controller

	CTRL_State_e		prevState;		//!< the previous state of the controller

	CTRL_ErrorCode_e	errorCode;          //!< the error code for the controller

	INPUT_Key_Handle	inputHandle;

	MAIN_WorkingMode_e	working_mode;


	uint8_t			blower_speed;		//!< blower speed

	uint8_t			blower_last_speed;	//!< blower last running speed

	uint32_t			cu_runing_time;	//!< cummulative running time

	uint16_t			auto_power_off_delay_time; //!<

	uint16_t			auto_power_off_remain_time; //!<

	uint16_t			running_time_cnt;		//!< for turn off after 3hours running

	uint8_t			short_tick_cnt;	//!<short tick for increase while holding

	uint8_t			long_tick_cnt;		//!<long tick for change state

	uint8_t			rtc_sec;

	uint8_t			rtc_min;

	uint8_t			rtc_hour;

	uint8_t			rtc_tmp_min;
	
	uint8_t			rtc_tmp_hour;

	unsigned			lamp_state:1;		//!< main lamp on = 1 or off = 0
	unsigned 			led_state:1;
	unsigned			state_change:1;

	unsigned 			rtc_last_input:1;
	unsigned			rtc_curr_input:1;
	unsigned			rtc_time_adj_stage:1;

} CTRL_Obj;

//! \brief Defines the CTRL handle
//!
typedef struct CTRL_Obj   *CTRL_Handle;


CTRL_Handle CTLR_init(void *pMemory,const size_t numBytes);
//void CTRL_set_blowerSpeed(CTRL_Handle handle, const uint8_t spd);
//void CTRL_set_lamp(CTRL_Handle handle, const bool state);
//void CTRL_set_led(CTRL_Handle handle, const _Bool state);

void CTRL_run(CTRL_Handle ctrlHandle);
void CTRL_ISR1s(CTRL_Handle ctrlHandle);
void CTRL_ISR50ms(CTRL_Handle ctrlHandle);
void CTRL_set_main_working_mode(CTRL_Handle ctrlHandle, MAIN_WorkingMode_e mode);

#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of __UI_WHITECTRL_H_ definition


