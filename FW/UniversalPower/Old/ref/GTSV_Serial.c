
#include "main.h"
#include "GTSV_serial.h"

volatile struct Serial_Vars_t gSerial_vars;


struct Serial_Rx_t rxBuff = {
	.nbr_of_cmd_in_buff = 0,
	.head_cmd_idx = 0,
	.tail_cmd_idx = 0,
};

struct Serial_Tx_t txBuff = {
	.pdata = NULL,
	.bytes_left = 0,
	.tx_completed = TRUE,
};

void Serial_time_out_begin(void);
bool Serial_check_timeout(void);
void Serial_rx_reset_to_idle(void);
void Serial_rx_complete_receiving_packet(void);

void Serial_send_my_uid(void)
{
	tx_pk.cmd = SERIAL_CMD_UID;
	tx_pk.data_len = 12;
	tx_pk.pdata = gSystemFlags.system_uid;
	Serial_tx_send_cmd(&tx_pk);
}


void Serial_tx_send_bytes_non_blocking(uint8_t *p_tx, uint8_t nbr_of_bytes)
{
	txBuff.pdata = p_tx;
	txBuff.bytes_left = nbr_of_bytes;
	txBuff.tx_completed = FALSE;
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void Serial_tx_ISR(void)
{
	if(!txBuff.tx_completed){
		USART3->DR = *txBuff.pdata;
		if(txBuff.bytes_left>0){
			txBuff.pdata++;
			txBuff.bytes_left--;
		}else{
			txBuff.tx_completed = TRUE;
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		}
	}
}

void Serial_calculate_chk_sum(struct Serial_Tx_Packet_t *pk)
{
	uint8_t i;
	uint16_t chk_sum;
	chk_sum = SERIAL_PACKET_SOF + pk->cmd;
	if(pk->data_len != 0){
		chk_sum += pk->data_len;
		for(i=0; i< pk->data_len; i++){
			chk_sum += *(pk->pdata+i);
		}
	}
	pk->chk_sum[0] = (uint8_t)(chk_sum & 0x00FF);
	pk->chk_sum[1] = (uint8_t)(chk_sum>>8);
}

uint8_t buf[SERIAL_BUFF_MAX_CMD];
void Serial_tx_send_cmd(struct Serial_Tx_Packet_t *pk)
{
	uint8_t i;
	buf[0] = SERIAL_PACKET_SOF;
	buf[1] = pk->cmd;
	buf[2] = pk->data_len;
	if(pk->data_len>0){
		for(i=0; i< pk->data_len; i++){
			buf[3+i] = *(pk->pdata+i);
		}
	}

	Serial_calculate_chk_sum(pk);
	buf[3+pk->data_len] = pk->chk_sum[0];
	buf[4+pk->data_len] = pk->chk_sum[1];
	buf[5+pk->data_len] = SERIAL_PACKET_EOF1;
	buf[6+pk->data_len] = SERIAL_PACKET_EOF2;
	Serial_tx_send_bytes_non_blocking(buf, (6+pk->data_len));
}




void Serial_rx_ISR(void)
{
	static uint8_t * p_last;
	static uint8_t * p_current;
	p_last = p_current;
	p_current = &(rxBuff.buff[rxBuff.head_cmd_idx][rxBuff.bytes_cnt]);
	*p_current = (uint8_t)(SERIAL_COM_PORT->DR & 0x00FF);

	switch(rxBuff.state){
	case SERIAL_RX_STATE_IDLE:
		//wait until SOF
		if(*p_current == SERIAL_PACKET_SOF){
			rxBuff.state = SERIAL_RX_STATE_RECEIVING;
			rxBuff.bytes_cnt++;
			Serial_time_out_begin();
		}else{
			rxBuff.bytes_cnt = 0;
		}

		break;
	case SERIAL_RX_STATE_RECEIVING:
		//keep receiving until EOF
		if((*p_current==SERIAL_PACKET_EOF2) && (*p_last==SERIAL_PACKET_EOF1)){
			rxBuff.data_len = rxBuff.buff[rxBuff.head_cmd_idx][SERIAL_PACKET_IDX_DATA_LEN];
			if(rxBuff.data_len == (rxBuff.bytes_cnt-6)){
				Serial_rx_complete_receiving_packet();
			}else{
				Serial_rx_reset_to_idle();
			}
		}else{
			rxBuff.bytes_cnt++;
		}
		//& check for  time out
		if(Serial_check_timeout()){
			Serial_rx_reset_to_idle();
		}
		break;
	default:
		Serial_rx_reset_to_idle();
		break;
	}
}

void Serial_rx_complete_receiving_packet(void)
{
	//make first byte = buff len
	//rxBuff.buff[rxBuff.head_cmd_idx][0] = rxBuff.bytes_cnt;
	if(rxBuff.head_cmd_idx < SERIAL_BUFF_MAX_CMD-1)
		rxBuff.head_cmd_idx++;
	else
		rxBuff.head_cmd_idx = 0;

	rxBuff.nbr_of_cmd_in_buff++;

	Serial_rx_reset_to_idle();
}


void Serial_time_out_begin(void)
{
	rxBuff.time_out_cnt 	= 0;
	rxBuff.time_out 		= FALSE;
}
void Serial_time_tick50ms(void)
{
	if(rxBuff.time_out_cnt > (SERIAL_RX_PACKET_TIMEOUT_MS/50))
		rxBuff.time_out = TRUE;
	else
		rxBuff.time_out_cnt++;
}
bool Serial_check_timeout(void)
{
	/*
	static uint16_t last_cnt;
	if(last_cnt == rxBuff){

	}
	last_cnt = rxBuff.time_out_cnt;
	*/
	return rxBuff.time_out;
	//return FALSE;
}

void Serial_rx_reset_to_idle(void)
{
	rxBuff.state = SERIAL_RX_STATE_IDLE;
	rxBuff.bytes_cnt = 0;
}


bool Serial_rx_check_buff_not_empty(void)
{
	if(rxBuff.nbr_of_cmd_in_buff>0)
		return TRUE;
	else
		return FALSE;
}


uint16_t rx_chk_sum;
uint16_t chk_sum=0;
bool Serial_rx_chk_sum_ok(struct Serial_Cmd_Result_t *results)
{
	uint8_t i;

	chk_sum=results->cmd + SERIAL_PACKET_SOF + results->data_len;
	rx_chk_sum = *(results->pdata + results->data_len) + (*(results->pdata+results->data_len+1)<<8);
	for(i=0; i < (results->data_len); i++){
		chk_sum += *(results->pdata + i);
	}
	if(chk_sum == rx_chk_sum)
		return TRUE;
	else
		return FALSE;
}

//only return TRUE if a good cmd: correct: 1. SOF + EOF, data_len, chk sum
bool Serial_rx_get_cmd_from_buff(struct Serial_Cmd_Result_t *results)
{
	bool rturn = FALSE;
	if(rxBuff.nbr_of_cmd_in_buff > 0){
		results->cmd = rxBuff.buff[rxBuff.tail_cmd_idx][1];
		results->data_len = rxBuff.buff[rxBuff.tail_cmd_idx][2];
		results->pdata = &(rxBuff.buff[rxBuff.tail_cmd_idx][3]);
		if(Serial_rx_chk_sum_ok(results)){
			rturn = TRUE;
		}
		//next one
		rxBuff.nbr_of_cmd_in_buff--;
		if(rxBuff.tail_cmd_idx< SERIAL_BUFF_MAX_CMD -1){
			rxBuff.tail_cmd_idx++;
		}else{
			rxBuff.tail_cmd_idx = 0;
		}
	}

	return rturn;
}

void Serial_tx_send_ack_cmd(void)
{
	tx_pk.cmd = SERIAL_CMD_ACK;
	tx_pk.data_len = 0;
	Serial_tx_send_cmd(&tx_pk);
}


void Serial_tx_send_sys_state_upd_cmd(void)
{
	uint8_t tmp_bytes[8];
	
	
	tx_pk.cmd = SERIAL_CMD_SYS_STATE_UPD;
	tx_pk.pdata = tmp_bytes;
	tmp_bytes[0] = gSystemFlags.sys_state;
	tmp_bytes[1] = gSystemFlags.light_state;
	switch(gSystemFlags.sys_state){
	case SYS_STATE_OFF:
		tx_pk.data_len = 2;
		Serial_tx_send_cmd(&tx_pk);
		break;
	case SYS_STATE_CLK_ADJ:
		//tmp_bytes[0] = SYS_STATE_CLK_ADJ;
		
		tmp_bytes[2] = gSystemFlags.time_adj_stage;
		tmp_bytes[3] = gSystemFlags.tmp_hour;
		tmp_bytes[4] = gSystemFlags.tmp_min;
		tmp_bytes[5] = TRUE; //reset time_adj_delay
		tmp_bytes[6] = TRUE; //save current tmp_time to RTC
		tx_pk.data_len = 7;
		Serial_tx_send_cmd(&tx_pk);
		break;
	case SYS_STATE_BLOWING:
		//tmp_bytes[0] = SYS_STATE_BLOWING;
		tmp_bytes[2] = gSystemFlags.fan_spd_default;
		tmp_bytes[3] = gSystemFlags.ctime_hrs;
		tmp_bytes[4] = gSystemFlags.ctime_mins;
		tx_pk.data_len = 5;
		Serial_tx_send_cmd(&tx_pk);
		break;
	case SYS_STATE_APO_DTIME_ADJ:
		//tmp_bytes[0] = SYS_STATE_BLOWING_APO_ADJ;
		tmp_bytes[2] = gSystemFlags.blower_apo_mins;
		tmp_bytes[3] = TRUE; //reset time_adj_delay
		//tmp_bytes[3] = TRUE; //update tmp min into APO remaining sec
		tx_pk.data_len = 4;
		Serial_tx_send_cmd(&tx_pk);
		break;
	case SYS_STATE_APO_BLOWING:
		//tmp_bytes[0] = SYS_STATE_BLOWING_APO;
		tmp_bytes[2] = gSystemFlags.fan_spd_default;
		tx_pk.data_len = 3;
		Serial_tx_send_cmd(&tx_pk);
		break;
	default:
		break;
	}
	

}

void Serial_cmd_detect(void)
{
	uint8_t i;
	if(Serial_rx_get_cmd_from_buff(&results)){
		switch(results.cmd){
			
		case SERIAL_CMD_SYS_STATE_UPD:
			gSystemFlags.control_master = FALSE;
			gSystemFlags.light_state = *(results.pdata+1);
			switch(*(results.pdata)){
			case SYS_STATE_OFF:
				//if(gSystemFlags.sys_state == SYS_STATE_CLK_ADJ)
					//RTC_change_time(gSystemFlags.tmp_hour, gSystemFlags.tmp_min, 0);	
				gSystemFlags.sys_state = SYS_STATE_OFF;
				//Lcd_clear();
				break;
			case SYS_STATE_CLK_ADJ:
				gSystemFlags.sys_state = SYS_STATE_CLK_ADJ;
				gSystemFlags.time_adj_stage = *(results.pdata+2);
				gSystemFlags.tmp_hour = *(results.pdata+3);
				gSystemFlags.tmp_min = *(results.pdata+4);
				//if((*(results.pdata+4) == TRUE))
				gSystemFlags.time_adj_delay = 0;
				//if((*(results.pdata+5) == TRUE))
				RTC_change_time(gSystemFlags.tmp_hour, gSystemFlags.tmp_min, 0);
				break;
			case SYS_STATE_BLOWING:
				if(gSystemFlags.sys_state == SYS_STATE_OFF){
					gSystemFlags.ctime_hrs = *(results.pdata+3);
					gSystemFlags.ctime_mins = *(results.pdata+4);
				}
				gSystemFlags.sys_state = SYS_STATE_BLOWING;
				gSystemFlags.fan_spd_default = *(results.pdata+2);
				//Blower_set_speed(gSystemFlags.fan_spd_default);
				if(gSystemFlags.fan_spd_default != 0)
					Blower_set_speed(gSystemFlags.fan_spd_default);
				break;
			case SYS_STATE_APO_DTIME_ADJ:
				gSystemFlags.sys_state = SYS_STATE_APO_DTIME_ADJ;
				gSystemFlags.blower_apo_mins = *(results.pdata+2);
				//if((*(results.pdata+2) == TRUE))
				gSystemFlags.time_adj_delay = 0;
				break;
			case SYS_STATE_APO_BLOWING:
				if(gSystemFlags.sys_state == SYS_STATE_BLOWING){
					gSystemFlags.blower_apo_remaining_sec = gSystemFlags.blower_apo_mins*60+1;
					gSystemFlags.blower_apo_time_out = 0;
				}					
				gSystemFlags.sys_state = SYS_STATE_APO_BLOWING;
				gSystemFlags.fan_spd_default = *(results.pdata+2);
				if(gSystemFlags.fan_spd_default != 0)
					Blower_set_speed(gSystemFlags.fan_spd_default);
				break;
			default:
				break;
			}
			break;
			
		case SERIAL_CMD_TIMER:
			//gSerial_vars.cmd_valid=TRUE;
			//gSerial_vars.cmd = SERIAL_CMD_TIMER;
			//Serial_tx_send_ack_cmd();
			break;
		case SERIAL_CMD_PLUS:
			break;
		case SERIAL_CMD_UID:
			for(i=0; i<12; i++){
				gSystemFlags.partner_uid[i] = *(results.pdata + i);
			}
			gSystemFlags.partner_uid_valid = TRUE;

			//find the first diff value
			i=0;
			while(gSystemFlags.system_uid[i] == gSystemFlags.partner_uid[i]){
				i++;
			}
			if(gSystemFlags.system_uid[i] > gSystemFlags.partner_uid[i]){
				gSystemFlags.working_mode = WORKING_OUTPUT_MASTER;
			}else{
				gSystemFlags.working_mode = WORKING_INPUT_SLAVE;
			}
			//Buzzer_bip();
			break;
		default:
			gSerial_vars.cmd_valid = FALSE;
			break;
		}
	}
}

bool Serial_check_cmd(enum Serial_Cmd_t cmd)
{
	if( (gSerial_vars.cmd_valid) && (gSerial_vars.cmd == cmd))
		return TRUE;
	else
		return FALSE;
}

void Usart_to_default_config(void)
{
	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_USART1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
}





void Serial_tx_send_bytes_blocking(uint8_t *p_tx, uint8_t nbr_of_bytes)
{
	uint8_t i;
	uint8_t * tmp = p_tx;
	//tmp = p_tx;

	for(i=0; i<nbr_of_bytes; i++){
		while((USART1->SR & USART_FLAG_TXE) == (uint16_t)RESET){}
		USART1->DR = *tmp;
		tmp++;
	}
}


/*

uint8_t* Serial_rx_get_cmd_from_buff1(void)
{
	uint8_t *pdata;
	if(rxBuff.nbr_of_cmd_in_buff>0){
		pdata = rxBuff.buff[rxBuff.tail_cmd_idx];
		rxBuff.nbr_of_cmd_in_buff--;
		if(rxBuff.tail_cmd_idx<SERIAL_BUFF_MAX_CMD-1)
			rxBuff.tail_cmd_idx++;
		else
			rxBuff.tail_cmd_idx=0;
		return pdata;
	}else{
		return NULL;
	}
}




*/

