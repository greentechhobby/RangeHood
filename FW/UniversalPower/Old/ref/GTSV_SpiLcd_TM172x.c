#include "main.h"
#include "GTSV_SpiLcd_TM172x.h"


#ifndef LCD_COM_SEG
#define LCD_COM_SEG(lcd_com, lcd_seg)\
			*((volatile unsigned char *)(BITBAND_PERI(LCD_BASE + lcd_com, lcd_seg)))
#endif

#define SPILCD_STB_BIT		BITBAND_POINTER_AT(GPIOA_BASE + ODR_REG_OFFSET, 4)
#define SPILCD_STB_BIT_SET	BITBAND_POINTER_AT(GPIOA_BASE + BSRRL_REG_OFFSET, 4)=1
#define SPILCD_STB_BIT_RESET 	BITBAND_POINTER_AT(GPIOA_BASE + BSRRH_REG_OFFSET, 4)=1

#define LCD_CHAR_CLEAR		11

//this is the offset of the com channel address in memory relative to the LCD_RAM_BASE (or COM0 Offset)
//the table is the offset of MCU com channel that drive the LCD pins: COM0, COM1, COM2, COM3
static const uint8_t _lcd_com[4] =
					{0x14, 0x1C, 0x24, 0x2C};
//this is the segment offset relative to MCU segment 0 ,
//the table is the segment offset of MCU segment channel that drive the LCD SEGMENT: SEG0... SEG13
//to change the MCU segment, just change the offset
//currently MCUSeg0 driving LCDSeg0, MCUSeg1 driving LCDSeg1, ..., MCUSeg7 driving  LCDSeg4
//Ex: if you want MCUSeg0(offset=0) driving LCDSeg8 then _lcd_segment_offset[8]=0
static const uint8_t _lcd_segment[18] =
					{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

//change from number to 7 segments: g f e d c b a
//Ex: number 0 will have: g=0, f=1, e=1, d=1, c=1, b=1, a=1 => the binary: 0111111 or hex:0x3F
//0,1, 2, 3, 4, 5, 6, 7, 8, 9, F, blank
const uint8_t _number_to_7segments[12] =
					{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00};

const uint8_t _num_to_7seg0[12] = 
					{0xAF, 0x06, 0x6D, 0x4F, 0xC6, 0xCB, 0xEB, 0x0E, 0xEF, 0xCF, 0x00, 0x00};

const uint8_t _num_to_7seg1[12] =
					{0xF5, 0x60, 0xB6, 0xF2, 0x63, 0xD3, 0xD7, 0x70, 0xF7, 0xF3, 0x00, 0x00};
					
const uint8_t _num_to_7seg2[12] =
					{ 0x5F, 0x06, 0x6B, 0x2F, 0x36, 0x3D, 0x7D, 0x07, 0x7F, 0x3F, 0x00, 0x00 };
					
const uint8_t _num_pos[9] = {10, 9, 5, 4, 12, 11, 2, 3, 14};

uint8_t _lcd_buf[16];

volatile uint8_t _lcd_blink_cursor;
volatile uint8_t _lcd_fan_cursor_slow=0;
volatile uint8_t _lcd_fan_cursor_fast=0;


volatile int j;


/*
_lcd_buf[_num_pos[0]] = _num_to_7seg0[i];
_lcd_buf[_num_pos[1]] = _num_to_7seg0[i];
_lcd_buf[_num_pos[2]] = _num_to_7seg0[i];
_lcd_buf[_num_pos[3]] = _num_to_7seg0[i];

_lcd_buf[_num_pos[8]] = _num_to_7seg2[i];
_lcd_buf[_num_pos[4]] = _num_to_7seg2[i];
_lcd_buf[_num_pos[5]] = _num_to_7seg2[i];

_lcd_buf[_num_pos[6]] = _num_to_7seg1[i];
_lcd_buf[_num_pos[7]] = _num_to_7seg1[i];
*/


void Lcd_fill_pos_with_num(uint8_t pos, uint8_t num)
{
	uint8_t tmp;
	if(num<10)
		tmp = num;
	else 
		tmp = 11; //10 is clear
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

void Lcd_fill_2pos_with_num(uint8_t pos, uint8_t num)
{
	Lcd_fill_pos_with_num(pos, num/10);
	Lcd_fill_pos_with_num(pos+1, num%10);
}

void Lcd_fill_hour1(uint8_t num)
{
	Lcd_fill_2pos_with_num(0, num);

}

void Lcd_fill_hour2(uint8_t num)
{
	//Lcd_fill_2pos_with_num(4, num);
	Lcd_fill_pos_with_num(4, num/10);
	Lcd_fill_pos_with_num(5, num%10);
}

void Lcd_fill_min1(uint8_t num)
{
	Lcd_fill_2pos_with_num(2, num);
}

void Lcd_fill_min2(uint8_t num)
{
	Lcd_fill_2pos_with_num(6, num);
}

void Lcd_clear_hour1(void)
{
	Lcd_fill_pos_with_num(0, LCD_CHAR_CLEAR);
	Lcd_fill_pos_with_num(1, LCD_CHAR_CLEAR);
}
void Lcd_clear_min1(void)
{
	Lcd_fill_pos_with_num(2, LCD_CHAR_CLEAR);
	Lcd_fill_pos_with_num(3, LCD_CHAR_CLEAR);
}

void Lcd_clear_hour1_min1(void)
{
	Lcd_fill_pos_with_num(0, LCD_CHAR_CLEAR);
	Lcd_fill_pos_with_num(1, LCD_CHAR_CLEAR);
	Lcd_fill_pos_with_num(2, LCD_CHAR_CLEAR);
	Lcd_fill_pos_with_num(3, LCD_CHAR_CLEAR);
}

void Lcd_clear_hour2_min2(void)
{
	Lcd_fill_pos_with_num(4, LCD_CHAR_CLEAR);
	Lcd_fill_pos_with_num(5, LCD_CHAR_CLEAR);
	Lcd_fill_pos_with_num(6, LCD_CHAR_CLEAR);
	Lcd_fill_pos_with_num(7, LCD_CHAR_CLEAR);
}

void Spilcd_flush_buf_to_lcd(void)
{
	uint8_t i;



	SPILCD_STB_BIT = 0;
	SPI1->DR = 0x40;  //write data to LCD ram/ auto increment
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	SPILCD_STB_BIT = 1;
	SPILCD_STB_BIT = 0;
	SPI1->DR = 0xC2; //begin at address 0x02
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}

	for(i=2; i<15; i++){
		SPI1->DR = _lcd_buf[i];
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	}
	SPILCD_STB_BIT = 1;

}

void Lcd_clear(void)
{
	uint8_t i;

	SPILCD_STB_BIT = 0;
	SPI1->DR = 0x40;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	SPILCD_STB_BIT = 1;
	SPILCD_STB_BIT = 0;
	SPI1->DR = 0xC0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}

	for(i=0; i<16; i++){
		SPI1->DR = 0x00;
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	}
	SPILCD_STB_BIT = 1;
}

void Lcd_set(void)
{
	uint8_t i;

	SPILCD_STB_BIT = 0;
	SPI1->DR = 0x40;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	SPILCD_STB_BIT = 1;
	SPILCD_STB_BIT = 0;
	SPI1->DR = 0xC0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}

	for(i=0; i<16; i++){
		SPI1->DR = 0xFF;
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	}
	SPILCD_STB_BIT = 1;
}

void Lcd_icon_on(enum Lcd_Icons_Enum_t icon)
{
	switch(icon){
	case LCD_ICON_CLOCK:  //seg 21 20
		_lcd_buf[10] |= (1<<4);
		break;
	case LCD_ICON_FAN1:   //seg 19 18
		_lcd_buf[9] |= (1<<4);
		break;
	case LCD_ICON_COLON1:  //seg 11 10
		_lcd_buf[5] |= (1<<4);
		break;
	case LCD_ICON_FAN2: //seg 9 8
		_lcd_buf[4] |= (1<<4);
		break;
	case LCD_ICON_COLON2: //seg 5 4
		_lcd_buf[2] |= (1<<3);
		break;
	case LCD_ICON_LIGHT:
		_lcd_buf[12] |= (1<<7);
		break;
	case LCD_ICON_FAN3:
		_lcd_buf[14] |= (1<<7);
		break;
	case LCD_ICON_ALL:
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

void Lcd_icon_off(enum Lcd_Icons_Enum_t icon)
{
	switch(icon){
	case LCD_ICON_CLOCK:  //seg 21 20
		_lcd_buf[10] &= ~(1<<4);
		break;
	case LCD_ICON_FAN1:   //seg 19 18
		_lcd_buf[9] &= ~(1<<4);
		break;
	case LCD_ICON_COLON1:  //seg 11 10
		_lcd_buf[5] &= ~(1<<4);
		break;
	case LCD_ICON_FAN2: //seg 9 8
		_lcd_buf[4] &= ~(1<<4);
		break;
	case LCD_ICON_COLON2: //seg 5 4
		_lcd_buf[2] &= ~(1<<3);
		break;
	case LCD_ICON_LIGHT:
		_lcd_buf[12] &= ~(1<<7);
		break;
	case LCD_ICON_FAN3:
		_lcd_buf[14] &= ~(1<<7);
		break;
	case LCD_ICON_ALL:
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

void Lcd_icon_toogle(enum Lcd_Icons_Enum_t icon)
{
	switch(icon){
	case LCD_ICON_CLOCK:  //seg 21 20
		_lcd_buf[10] ^= (1<<4);
		break;
	case LCD_ICON_FAN1:   //seg 19 18
		_lcd_buf[9] ^= (1<<4);
		break;
	case LCD_ICON_COLON1:  //seg 11 10
		_lcd_buf[5] ^= (1<<4);
		break;
	case LCD_ICON_FAN2: //seg 9 8
		_lcd_buf[4] ^= (1<<4);
		break;
	case LCD_ICON_COLON2: //seg 5 4
		_lcd_buf[2] ^= (1<<3);
		break;
	case LCD_ICON_LIGHT:
		_lcd_buf[12] ^= (1<<7);
		break;
	case LCD_ICON_FAN3:
		_lcd_buf[14] ^= (1<<7);
		break;
	case LCD_ICON_ALL:
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

void Lcd_icon_fan(uint8_t num)
{
	switch(num){
	case 0:
		Lcd_icon_on(LCD_ICON_FAN1);
		Lcd_icon_off(LCD_ICON_FAN2);
		Lcd_icon_off(LCD_ICON_FAN3);
		break;
	case 1:
		Lcd_icon_off(LCD_ICON_FAN1);
		Lcd_icon_on(LCD_ICON_FAN2);
		Lcd_icon_off(LCD_ICON_FAN3);
		break;
	case 2:
		Lcd_icon_off(LCD_ICON_FAN1);
		Lcd_icon_off(LCD_ICON_FAN2);
		Lcd_icon_on(LCD_ICON_FAN3);
		break;
	default:
		Lcd_icon_off(LCD_ICON_FAN1);
		Lcd_icon_off(LCD_ICON_FAN2);
		Lcd_icon_off(LCD_ICON_FAN3);
		break;
	}

}


void Lcd_blink_tick125ms(void)
{
	static uint8_t ms125Tick;
	static uint8_t fan_slow_cnt=0;
	 
	 if(++ms125Tick==8)
		 ms125Tick = 0;
	 
	 if(ms125Tick<4){
		 //_lcd_icons |= _lcd_blink_mask;
		 _lcd_blink_cursor = 1;
 
	 }else{
		 //_lcd_icons &= ~(_lcd_blink_mask);
		 _lcd_blink_cursor = 0;
		 
	 }
 
 	if(++_lcd_fan_cursor_fast==3)
			 _lcd_fan_cursor_fast=0;

	 if(++fan_slow_cnt==3){
		 fan_slow_cnt= 0;
		 if(++_lcd_fan_cursor_slow==3)
			 _lcd_fan_cursor_slow=0;
	 }
}

uint8_t Lcd_get_blink_cursor(void)
{
	return _lcd_blink_cursor;
}

uint8_t Lcd_get_fan_cursor_slow(void)
{
	return _lcd_fan_cursor_slow;
}

uint8_t Lcd_get_fan_cursor_fast(void)
{
	return _lcd_fan_cursor_fast;
}


void Spilcd_to_default_config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;  //clk~1us
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_Init(SPI1, &SPI_InitStructure);
	//SPI_SSOutputCmd(SPI1, ENABLE);
	SPI_Cmd(SPI1, ENABLE);


	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	SPILCD_STB_BIT = 0;
	SPI1->DR = 0x00;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	SPILCD_STB_BIT = 1;
	SPILCD_STB_BIT = 1;
	SPILCD_STB_BIT = 1;

	SPILCD_STB_BIT = 0;
	SPI1->DR = 0x97;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	SPILCD_STB_BIT = 1;
	SPILCD_STB_BIT = 1;
	SPILCD_STB_BIT = 1;

	Lcd_clear();
}

void Spilcd_configure_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//config GPIO for SPI1
	//for STB pins
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7,GPIO_AF_SPI1);
}
