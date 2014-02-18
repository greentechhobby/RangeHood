/*
 * IRremote.c
 *
 *  Created on: Jul 17, 2013
 *      Author: Jack
 */

#include "IRremote.h"



//TA1CCR0 is config to capture timer A1 at every falling edge.
//Put this func into the ISR to receive IR command.
void IRR_capture_ISR(IRR_Obj * obj)
{

	uint16_t capture_data = TA1CCR0; //get capture data
	TA1CTL |= TACLR; //clear timer1 counter
	//TA1CCTL0 &= ~(CCIFG);

	switch(obj->state){
	case IRR_Receive_State_Idle:
		if(IRR_CHECK_TIMING_TICK_OK(capture_data, IRR_NEC_HEADER_LEN_us)){
			obj->state = IRR_Receive_State_Receiving;
			obj->raw_data = 0;
			obj->bit_cnt = 0;
		}
		break;
	case IRR_Receive_State_Receiving:
		if(IRR_CHECK_TIMING_TICK_OK(capture_data, IRR_NEC_BIT_ONE_LEN_us)){
			obj->raw_data |= (((uint32_t)1) << (obj->bit_cnt));
			obj->bit_cnt++;
		}else if(IRR_CHECK_TIMING_TICK_OK(capture_data, IRR_NEC_BIT_ZERO_LEN_us)){
			//obj->raw_data &= ~(((uint32_t)1) << (obj->bit_cnt));
			obj->bit_cnt++;
		}else if(capture_data > 3){
			obj->state = IRR_Receive_State_Idle;
			obj->bit_cnt = 0;
		}


		if(obj->bit_cnt == 32){
			obj->state = IRR_Receive_State_Idle;
			obj->bit_cnt = 0;
			obj->recv_data = obj->raw_data;
			obj->data_ready = 1;
		}
		//obj->tmpDat[obj->bit_cnt] = capture_data;
		break;
	default:
		obj->state = IRR_Receive_State_Idle;
		obj->bit_cnt = 0;
		break;
	}

	//TA1CCTL0 &= ~(COV);
}

IRR_NecCmd_e IRR_get_command(IRR_Obj *obj)
{
	if(obj->data_ready){
		obj->data_ready = 0;
		return ((IRR_NecCmd_e)obj->recv_data);
	}else{
		return IRR_NecCmd_None;
	}
}

