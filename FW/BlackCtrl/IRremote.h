/*
 * IRremote.h
 *
 *  Created on: Jul 17, 2013
 *      Author: Jack
 */

#ifndef __IRREMOTE_H_
#define __IRREMOTE_H_
#include "main.h"



//define timing related to NEC protocol
#define IRR_MIN_GAP_us				(5000)
#define IRR_NEC_HEADER_MARK_us			(9000)
#define IRR_NEC_HEADER_SPACE_us			(4500)
#define IRR_NEC_BIT_MARK_us			(560)
#define IRR_NEC_BIT_ONE_SPACE_us		(1600)
#define IRR_NEC_BIT_ZERO_SPACE_us		(560)
#define IRR_NEC_REPEAT_SPACE_us			(2250)

#define IRR_NEC_HEADER_LEN_us			(IRR_NEC_HEADER_MARK_us + IRR_NEC_HEADER_SPACE_us)
#define IRR_NEC_BIT_ONE_LEN_us			(IRR_NEC_BIT_MARK_us + IRR_NEC_BIT_ONE_SPACE_us)
#define IRR_NEC_BIT_ZERO_LEN_us			(IRR_NEC_BIT_MARK_us + IRR_NEC_BIT_ZERO_SPACE_us)


#define IRR_TOLERANCE				(0.22)

#define IRR_TOLERANCE_MAX_LEN_us(us)		(uint16_t) ( us*(1+IRR_TOLERANCE) )
#define IRR_TOLERANCE_MIN_LEN_us(us)		(uint16_t) ( us*(1-IRR_TOLERANCE) )


// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define IRR_EXCESS_us				(100)

#define IRR_TIMER_TICK_LEN_us			(31)

#define IRR_TOLERANCE_MAX_LEN_ticks(us)		(IRR_TOLERANCE_MAX_LEN_us(us)/IRR_TIMER_TICK_LEN_us)
#define IRR_TOLERANCE_MIN_LEN_ticks(us)		(IRR_TOLERANCE_MIN_LEN_us(us)/IRR_TIMER_TICK_LEN_us)

#define IRR_CHECK_TIMING_TICK_OK(ticks, target_us) \
		((ticks > IRR_TOLERANCE_MIN_LEN_ticks(target_us))\
		 && (ticks < IRR_TOLERANCE_MAX_LEN_ticks(target_us)))

typedef enum
{
	IRR_NecCmd_None = 0,
	IRR_NecCmd_Plus = 0xEE11DF20,
	IRR_NecCmd_Minus = 0xF50ADF20,
	IRR_NecCmd_Light = 0xE51ADF20,
	IRR_NecCmd_Timer = 0xE619DF20,
	IRR_NecCmd_Auto  = 0xF609DF20,
	IRR_NecCmd_OnOff = 0xFD02DF20,
} IRR_NecCmd_e;

typedef enum
{
	IRR_Input_Mark = 0,
	IRR_Input_Space = 1
} IRR_Input_e;

typedef enum
{
	IRR_Return_Success = 0,
	IRR_Return_Error = 1
} IRR_Return_e;

typedef enum 
{
	IRR_Receive_State_Idle,
	IRR_Receive_State_Receiving,
	IRR_Receive_numState
} IRR_Receive_State_e;

//! \brief
//! \details    
//!         
typedef struct _IRR_Obj_
{
	//uint8_t 		*input_register;   	//!< Port Input regs

	//uint8_t             	input_bit;		//!<input bit ex: if(*input_register & input_bit)

	//uint16_t 		tmpDat[33];
	IRR_Receive_State_e 	state;

	uint8_t			bit_cnt;
	
	uint32_t		raw_data;
	
	uint32_t		recv_data;

	unsigned		data_ready:1;

} IRR_Obj;


void IRR_capture_ISR(IRR_Obj * obj);
IRR_NecCmd_e IRR_get_command(IRR_Obj *obj);

#endif /* __IRREMOTE_H_ */
