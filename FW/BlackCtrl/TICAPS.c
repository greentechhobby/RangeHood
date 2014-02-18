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

//! \file   TICAPS.c
//! Created on: Jul 14, 2013
//!     Author: Long Pham
//!             jacksmtv@gmail.com
//! \brief
//!
//! (C) Copyright 2013, GTSV, Inc.


// **************************************************************************
// the includes

#include <msp430.h>
#include <stdint.h>
#include <stddef.h>
#include "TICAPS.h"

#ifdef TICAPS_RO_PINOSC_TA0_WDTp

#endif

#ifdef TICAPS_RO_PINOSC_TA0
void TICAPS_sckey_read_raw(TICAPS_Sckey_Obj *obj)
{
	uint16_t i;

	//configure TIMERA0 Capture 0  (also clear capture Interupt Flag: CCIFG)
	TA0CCTL0 = CM_1 + CCIS_1 + CAP;     // Capture on rising Edges (CM_1) of ACLK (CCIS_1), Capture mode (CAP)

	//__disable_interrupt();
	//configure TIMERA0 counter and begin counting PinOsc
	TA0CTL = TASSEL_3 + MC_2 + TACLR;   	// TASSEL_3: Clk source from PinOsc ()
						// MC_2: Continuous mode (),
						// TACLR: Clear TimerA counter
	*(obj->inputPxsel2Register) |= obj->inputBits;	//enable PinOsc
	for (i=0;i<TICAPS_ACCUMULATION_CYCLES;i++) {
		while(!(TA0CCTL0 & CCIFG));             // wait for ACLK rising edge -> capture
		TA0CCTL0 &= ~CCIFG;			// clear flag
	}
	*(obj->inputPxsel2Register) &= ~ (obj->inputBits);  //turn pin into normal IO -> disable PinOsc  <- MUST STOP PinOsc or touch will not work.
	TA0CTL = 0;                     // Halt Timer
	//__enable_interrupt();

	obj->meas_count = TACCR0; // Save Measured
} // end of TICAPS_sckey_read_raw() function
#endif

void TICAPS_sckey_init(TICAPS_Sckey_Obj **scKeys)
{
	TICAPS_Sckey_Obj **obj;
	uint8_t i;

	for(obj = scKeys; *obj != NULL; obj++){

		TICAPS_Sckey_Obj *pKey = *obj;

		// make sure PXSEL bit is clear
		*(pKey->inputPxselRegister) &= ~(pKey->inputBits);

		TICAPS_sckey_read_raw(pKey);
		pKey->baseline = pKey->meas_count;

		for(i=0; i< TICAPS_BASELINE_INIT_AVERAGE_NUM; i++){
			TICAPS_sckey_read_raw(pKey);
			pKey->baseline = ((pKey->baseline)>>1) + ((pKey->meas_count)>>1);
		}
	}
} // end of TICAPS_sckey_init() function

void TICAPS_sckey_run(TICAPS_Sckey_Obj **scKeys)
{
	TICAPS_Sckey_Obj **obj;

	for(obj = scKeys; *obj != NULL; obj++){
		TICAPS_Sckey_Obj *pKey = *obj;

		TICAPS_sckey_read_raw(pKey);

		if(pKey->meas_count < pKey->baseline){
			pKey->delta = pKey->baseline - pKey->meas_count;
		}else{
			//if meas_count > baseline  => fast calibrate new baseline
			//pKey->baseline = (pKey->baseline>>1) + (pKey->meas_count>>1);
			pKey->baseline += 5;
			pKey->delta = 0;
		}

		if(pKey->delta > pKey->threshold){
			pKey->detected = 1;
		}else{

			pKey->baseline--;
			pKey->detected = 0;
		}

#ifdef DEBUGGING
		if(pKey->max_delta < pKey->delta)
			pKey->max_delta = pKey->delta;
#endif
	}
} // end of  TICAPS_sckey_run() function
