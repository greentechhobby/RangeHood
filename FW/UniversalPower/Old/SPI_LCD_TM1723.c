#include "main.h"

#define LCD_CLEAR_NUM	10

const uint8_t _num_to_7seg0[12] =
					{0xAF, 0x06, 0x6D, 0x4F, 0xC6, 0xCB, 0xEB, 0x0E, 0xEF, 0xCF, 0x00, 0x00};

const uint8_t _num_to_7seg1[12] =
					{0xF5, 0x60, 0xB6, 0xF2, 0x63, 0xD3, 0xD7, 0x70, 0xF7, 0xF3, 0x00, 0x00};

const uint8_t _num_to_7seg2[12] =
					{ 0x5F, 0x06, 0x6B, 0x2F, 0x36, 0x3D, 0x7D, 0x07, 0x7F, 0x3F, 0x00, 0x00 };

const uint8_t _num_pos[9] = {10, 9, 5, 4, 12, 11, 2, 3, 14};


uint8_t _lcd_buf[16];
volatile _Bool _lcd_blink_cursor;
volatile uint8_t _lcd_fan_cursor_slow=0;
volatile uint8_t _lcd_fan_cursor_med=0;
volatile uint8_t _lcd_fan_cursor_fast=0;

static void SPI_to_default_config(void)
{
	SPI_DeInit();
	SPI_Init(SPI_FIRSTBIT_LSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_HIGH,
	      SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT,(uint8_t)0x07);
	//Delay_blocking(0xFFF);
	SPI_Cmd(ENABLE);
}

void LCD_to_default_config(void)
{
	SPI_to_default_config();
	while(SPI_GetFlagStatus(SPI_FLAG_BSY));
	SPI_STB_HIGH;
	SPI->DR = 0x00;

	while(SPI_GetFlagStatus(SPI_FLAG_BSY));
	SPI_STB_HIGH;
	SPI_STB_HIGH;
	SPI_STB_HIGH;

	SPI_STB_LOW;
	SPI->DR = 0x97;
	while(SPI_GetFlagStatus(SPI_FLAG_BSY));
	SPI_STB_HIGH;
	SPI_STB_HIGH;
	SPI_STB_HIGH;
}




void LCD_flush_buf_to_lcd(void)
{
	uint8_t i;

	SPI_STB_LOW;
	SPI->DR = 0x40;  //write data to LCD ram/ auto increment
	while(SPI_GetFlagStatus(SPI_FLAG_BSY));
	SPI_STB_HIGH;
	SPI_STB_LOW;
	SPI->DR = 0xC2; //begin at address 0x02
	while(SPI_GetFlagStatus(SPI_FLAG_BSY));

	for(i=2; i<15; i++){
		SPI->DR = _lcd_buf[i];
		while(SPI_GetFlagStatus(SPI_FLAG_BSY));
	}
	SPI_STB_HIGH;

}

void LCD_clear(void)
{
	uint8_t i;

	for(i=2; i<15; i++){
		_lcd_buf[i] = 0x00;
	}

}

void LCD_set(void)
{
	uint8_t i;

	for(i=2; i<15; i++){
		_lcd_buf[i] = 0xFF;
	}


}

//fill single digit num from 0..9  ; if num > 10 => clear the position
void LCD_fill_pos_with_num(uint8_t pos, uint8_t num)
{
	uint8_t tmp;

	//fill num from 0..9
	if(num<10){
		tmp = num;
	}else{ //other number => clear
		tmp = LCD_CLEAR_NUM;
	}
	switch(pos){
	case 0:
	case 1:
	case 2:
	case 3:
		_lcd_buf[_num_pos[pos]] = _num_to_7seg0[tmp] | ( _lcd_buf[_num_pos[pos]] & (1<<4)) ;
		break;
	case 4:
	case 5:
	case 8:
		_lcd_buf[_num_pos[pos]] = _num_to_7seg2[tmp] | ( _lcd_buf[_num_pos[pos]] & (1<<7));
		break;
	case 6:
	case 7:
		_lcd_buf[_num_pos[pos]] = _num_to_7seg1[tmp] | ( _lcd_buf[_num_pos[pos]] & (1<<3));
		break;
	}
}

void LCD_fill_hour1(uint8_t num)
{
	LCD_fill_pos_with_num(0, num/10);
	LCD_fill_pos_with_num(1, num%10);
}

void LCD_fill_min1(uint8_t num)
{
	LCD_fill_pos_with_num(2, num/10);
	LCD_fill_pos_with_num(3, num%10);
}

void LCD_fill_hour2(uint8_t num)
{
	LCD_fill_pos_with_num(4, num/10);
	LCD_fill_pos_with_num(5, num%10);
}

void LCD_fill_min2(uint8_t num)
{
	LCD_fill_pos_with_num(6, num/10);
	LCD_fill_pos_with_num(7, num%10);
}


void LCD_icon_on(LCD_Icon_e i)
{
	switch(i){
	case LCD_Icon_Clock:  //seg 21 20
		_lcd_buf[10] |= (1<<4);
		break;
	case LCD_Icon_Fan1:   //seg 19 18
		_lcd_buf[9] |= (1<<4);
		break;
	case LCD_Icon_Colon1:  //seg 11 10
		_lcd_buf[5] |= (1<<4);
		break;
	case LCD_Icon_Fan2: //seg 9 8
		_lcd_buf[4] |= (1<<4);
		break;
	case LCD_Icon_Colon2: //seg 5 4
		_lcd_buf[2] |= (1<<3);
		break;
	case LCD_Icon_Light:
		_lcd_buf[12] |= (1<<7);
		break;
	case LCD_Icon_Fan3:
		_lcd_buf[14] |= (1<<7);
		break;
	case LCD_Icon_All:
		_lcd_buf[10] |= (1<<4);
		_lcd_buf[9] |= (1<<4);
		_lcd_buf[5] |= (1<<4);
		_lcd_buf[4] |= (1<<4);
		_lcd_buf[2] |= (1<<3);
		_lcd_buf[12] |= (1<<7);
		_lcd_buf[14] |= (1<<7);
		break;

	}
}

void LCD_icon_off(LCD_Icon_e i)
{
	switch(i){
	case LCD_Icon_Clock:  //seg 21 20
		_lcd_buf[10] &= ~(1<<4);
		break;
	case LCD_Icon_Fan1:   //seg 19 18
		_lcd_buf[9] &= ~(1<<4);
		break;
	case LCD_Icon_Colon1:  //seg 11 10
		_lcd_buf[5] &= ~(1<<4);
		break;
	case LCD_Icon_Fan2: //seg 9 8
		_lcd_buf[4] &= ~(1<<4);
		break;
	case LCD_Icon_Colon2: //seg 5 4
		_lcd_buf[2] &= ~(1<<3);
		break;
	case LCD_Icon_Light:
		_lcd_buf[12] &= ~(1<<7);
		break;
	case LCD_Icon_Fan3:
		_lcd_buf[14] &= ~(1<<7);
		break;
	case LCD_Icon_All:
		_lcd_buf[10] &= ~(1<<4);
		_lcd_buf[9] &= ~(1<<4);
		_lcd_buf[5] &= ~(1<<4);
		_lcd_buf[4] &= ~(1<<4);
		_lcd_buf[2] &= ~(1<<3);
		_lcd_buf[12] &= ~(1<<7);
		_lcd_buf[14] &= ~(1<<7);
		break;
	}

}

void LCD_icon_toggle(LCD_Icon_e i)
{
	switch(i){
	case LCD_Icon_Clock:  //seg 21 20
		_lcd_buf[10] ^= (1<<4);
		break;
	case LCD_Icon_Fan1:   //seg 19 18
		_lcd_buf[9] ^= (1<<4);
		break;
	case LCD_Icon_Colon1:  //seg 11 10
		_lcd_buf[5] ^= (1<<4);
		break;
	case LCD_Icon_Fan2: //seg 9 8
		_lcd_buf[4] ^= (1<<4);
		break;
	case LCD_Icon_Colon2: //seg 5 4
		_lcd_buf[2] ^= (1<<3);
		break;
	case LCD_Icon_Light:
		_lcd_buf[12] ^= (1<<7);
		break;
	case LCD_Icon_Fan3:
		_lcd_buf[14] ^= (1<<7);
		break;
	case LCD_Icon_All:
		_lcd_buf[10] ^= (1<<4);
		_lcd_buf[9] ^= (1<<4);
		_lcd_buf[5] ^= (1<<4);
		_lcd_buf[4] ^= (1<<4);
		_lcd_buf[2] ^= (1<<3);
		_lcd_buf[12] ^= (1<<7);
		_lcd_buf[14] ^= (1<<7);
		break;
	}

}

void LCD_icon_fan(uint8_t fanNum)
{
	switch(fanNum){
	case 0:
		LCD_icon_on(LCD_Icon_Fan1);
		LCD_icon_off(LCD_Icon_Fan2);
		LCD_icon_off(LCD_Icon_Fan3);
		break;
	case 1:
		LCD_icon_off(LCD_Icon_Fan1);
		LCD_icon_on(LCD_Icon_Fan2);
		LCD_icon_off(LCD_Icon_Fan3);
		break;
	case 2:
		LCD_icon_off(LCD_Icon_Fan1);
		LCD_icon_off(LCD_Icon_Fan2);
		LCD_icon_on(LCD_Icon_Fan3);
		break;
	default:
		LCD_icon_off(LCD_Icon_Fan1);
		LCD_icon_off(LCD_Icon_Fan2);
		LCD_icon_off(LCD_Icon_Fan3);
		break;
	}

}

void LCD_blink_ISR100ms(void)
{
	static uint8_t ms125Tick;
	static uint8_t fan_slow_cnt=0, fan_med_cnt=0;
	static _Bool fan_fast_cnt=0;

	 if(++ms125Tick==10)
		 ms125Tick = 0;

	 if(ms125Tick<3){
		 //_lcd_icons |= _lcd_blink_mask;
		 _lcd_blink_cursor = 1;

	 }else{
		 //_lcd_icons &= ~(_lcd_blink_mask);
		 _lcd_blink_cursor = 0;

	 }

 	if(fan_fast_cnt==0){
 		fan_fast_cnt=1;
 	}else{
 		fan_fast_cnt=0;
	 	if(++_lcd_fan_cursor_fast==3)
				 _lcd_fan_cursor_fast=0;
	}
	 if(++fan_slow_cnt==5){
		 fan_slow_cnt= 0;
		 if(++_lcd_fan_cursor_slow==3)
			 _lcd_fan_cursor_slow=0;
	 }
	 if(++fan_med_cnt==3){
		 fan_med_cnt= 0;
		 if(++_lcd_fan_cursor_med==3)
			 _lcd_fan_cursor_med=0;
	 }
}

_Bool LCD_get_blink_cursor(void)
{
	return _lcd_blink_cursor;
}


uint8_t LCD_get_fan_cursor_slow(void)
{
	return _lcd_fan_cursor_slow;
}

uint8_t LCD_get_fan_cursor_fast(void)
{
	return _lcd_fan_cursor_fast;
}

uint8_t LCD_get_fan_cursor_med(void)
{
	return _lcd_fan_cursor_med;
}

