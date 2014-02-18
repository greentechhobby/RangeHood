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
#ifndef __INPUT_H_
#define __INPUT_H_


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
//! \defgroup Input Input
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


#define INPUT_KEY_DEBOUNCE_TIME_X50MS		(0)
#define INPUT_KEY_HOLD_ACTIVATE_TIME_X50MS	(12)

#define INPUT_KEY_ACTIVE					(1)
#define INPUT_KEY_PASSIVE				(0)




// **************************************************************************
// the typedefs

//! \brief Enumeration for the Control States
//!
typedef enum
{
	INPUT_Key_Power=0,
	INPUT_Key_Minus,
	INPUT_Key_Plus,
	INPUT_Key_Light,
	INPUT_Key_Any,
	INPUT_numKeys
} INPUT_Key_e;

//! \brief Defines the input key detec
//!
typedef struct _INPUT_Key_States_
{
	unsigned detected:1;
	unsigned rising_edge:1;
	//unsigned falling_edge_2:1;
	unsigned falling_edge:1;
	unsigned high_level:1;
	unsigned high_level_late:1;
	unsigned hold:1;
	unsigned hold_late:1;
	unsigned any:1;
	uint8_t  hold_cnt;
	uint8_t  debounce_cnt;
} INPUT_Key_State;


//! \brief Defines the INPUT controller object
//!
typedef struct _INPUT_Key_Obj_
{
	INPUT_Key_State keys[INPUT_numKeys];
	uint8_t 		 debounce_time_x50ms;
	uint8_t		 hold_activate_time_x50ms;
} INPUT_Key_Obj;

//! \brief Defines the INPUT key handle
//!
typedef INPUT_Key_Obj *INPUT_Key_Handle;


//! \brief     Initializes the INPUT Key
//! \param[in] pMemory   A pointer to the memory for the INPUT key object
//! \param[in] numBytes  The number of bytes allocated for the INPUT key object, bytes
//! \return The INPUT key object handle
INPUT_Key_Handle INPUT_key_init(void *pMemory, const size_t numBytes);

void INPUT_key_ISR50ms(INPUT_Key_Handle inputKeyHandle);
void INPUT_key_run(INPUT_Key_Handle inputKeyHandle);

_Bool INPUT_key_check(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key);
_Bool INPUT_key_check_down(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key);
_Bool INPUT_key_check_up(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key);
_Bool INPUT_key_check_touching(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key);
_Bool INPUT_key_check_holding_late(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key);
_Bool INPUT_key_check_holding(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key);



#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of __INPUT_H_ definition


