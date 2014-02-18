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
 * *  Neither the name of Texas Instruments Incorporated nor the names of
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
#ifndef __CTRL_H_
#define __CTRL_H_


//! \file   main.h
//! \brief  Contains global definitions & vars
//!         
//!
//! (C) Copyright 2013, GTSV, Inc.


// **************************************************************************
// the includes

#include "main.h"

//!
//!
//! \defgroup Main Main
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


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
//! \details    The controller object implements all of the FOC algorithms and calls the FAST
//!             observer functions.
//!
typedef struct _CTRL_Obj_ 
{
	CTRL_Version		version;             //!< the controller version

	CTRL_State_e		state;			//!< the current state of the controller
	
	CTRL_State_e		prevState;		//!< the previous state of the controller

	CTRL_ErrorCode_e	errorCode;          //!< the error code for the controller
	
} CTRL_Obj;

//! \brief Defines the CTRL handle
//!
typedef struct CTRL_Obj   *CTRL_Handle;



#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of __CTRL_H_ definition


