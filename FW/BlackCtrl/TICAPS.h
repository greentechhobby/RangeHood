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

#ifndef __TICAPS_H_
#define __TICAPS_H_

//! \file   TICAPS.h
//!     Created on: Jul 14, 2013
//!     Author:     Long Pham
//!                 jacksmtv@gmail.com
//! \details	. Only Single channel key is supported
//!		. Only TICAPS_RO_PINOSC_TA0 method is support
//!		. The method use blocking style coding.
//!		   Global interrupt is disable while reading cap so that
//!		   the reading is more exact. please look in the source code
//!		   of the func: TICAPS_sckey_read_raw() for more information
//!
//! (C) Copyright 2013, GTSV, Inc.

// **************************************************************************
// the includes

#include "main.h"

//!
//!
//! \defgroup TICAPS TICAPS
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the define

//! \brief define for debugging purpose. Comment out this line when release.
//!
#define DEBUGGING

//! \brief Defines the method used for CAP SENSE
//!        Right now only support TICAPS_RO_PINOSC_TA0 method.
//#define TICAPS_RO_PINOSC_TA0_WDTp
#define TICAPS_RO_PINOSC_TA0

#ifdef TICAPS_RO_PINOSC_TA0_WDTp
#define TICAPS_WDTp_GATE_32768    0x0000  // watchdog source/32768
#define TICAPS_WDTp_GATE_8192     0x0001  // watchdog source/8192
#define TICAPS_WDTp_GATE_512      0x0002  // watchdog source/512
#define TICAPS_WDTp_GATE_64       0x0003  // watchdog source/64
#define TICAPS_GATE_WDT_ACLK      0x0004
#define TICAPS_GATE_WDT_SMCLK     0x0000

#define TICAPS_ACCUMULATION_CYCLES		(WDTp_GATE_8192)
#endif

#ifdef TICAPS_RO_PINOSC_TA0
#define TICAPS_ACCUMULATION_CYCLES		(158)
#define TICAPS_BASELINE_INIT_AVERAGE_NUM	(8)
#endif

// **************************************************************************
// the typedefs

//! \brief      Defines the CAPSENSE Obj for Single Channel key.
//! \details	Every single channel key is a CAPSENSE_Sckey_Obj
//!		All functions will require:
//!			a pointer or
//!			an array of pointers to CAPSENSE_Sckey_Obj
//!
//!
typedef struct _TICAPS_Sckey_Obj_ {

	uint8_t *inputPxselRegister;	//!< PXSEL register

	uint8_t *inputPxsel2Register;	//!< PXSEL2 register

	uint16_t inputBits;		//!< input bit number


	uint16_t meas_count;		//!< measurement count

	uint16_t baseline;		//!< baseline for touch detect

	uint16_t delta;			//!< = baseline - meas_count

	uint16_t threshold;		//!< threshold for detected

	unsigned detected:1;		//!< detected bit

#ifdef DEBUGGING
	uint16_t max_delta;		//!< Max delta used for calibrating of threshold
#endif

} TICAPS_Sckey_Obj;


// **************************************************************************
// the globals


// **************************************************************************
// the function prototypes

#ifdef DEBUGGING
//! \brief      Read raw for single channel key obj.
//! \param[in]  a pointer to TICAPS_Sckey_Obj
//! \return     none (Measurement count is updated)
void TICAPS_sckey_read_raw(TICAPS_Sckey_Obj *obj);
#endif

//! \brief      Init all single channel keys. This function will make sure
//!		PXSEL bit is cleared & update baseline.
//! 		call this function once at the beginning of main program
//! \param[in]  an array of pointers to TICAPS_Sckey_Obj;
//!		the array must be ended with a pointer to NULL (terminator)
//! \return     none (baseline is updated)
void TICAPS_sckey_init(TICAPS_Sckey_Obj **scKeys);

//! \brief      Read all single channel keys, update delta, detected bit
//!		& baseline tracking.
//!		call this function periodically in a loop or in an ISR.
//! \param[in]  an array of pointers to TICAPS_Sckey_Obj;
//!		the array must be ended with a pointer to NULL (terminator)
//! \return     none
void TICAPS_sckey_run(TICAPS_Sckey_Obj **scKeys);

//TODO: make read raw in a ISR.

#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of __TICAPS_H_ definition
