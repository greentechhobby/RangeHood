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
#ifndef __SPI_LCD_TM1723_H_
#define __SPI_LCD_TM1723_H_


//! \file   SPI_LCD_TM1723.h
//! \brief  Contains global definitions & vars
//!         
//!
//! (C) Copyright 2013, GTSV, Inc.


// **************************************************************************
// the includes

#include "main.h"



//!
//!
//! \defgroup SPI_LCD_TM1723 SPI_LCD_TM1723
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the define


// **************************************************************************
// the typedefs

typedef enum _LCD_Icon_e_ {
	LCD_Icon_Clock = 0,
	LCD_Icon_Light,
	LCD_Icon_Colon1,
	LCD_Icon_Colon2,
	LCD_Icon_Fan1,
	LCD_Icon_Fan2,
	LCD_Icon_Fan3,
	LCD_Icon_All
} LCD_Icon_e;

/*
typedef enum _LCD_Icon_e_ {
	LCD_Icon_Clock  = (1<<1),
	LCD_Icon_Light  = (1<<2),
	LCD_Icon_Colon1 = (1<<3),
	LCD_Icon_Colon2 = (1<<4),
	LCD_Icon_Fan1   = (1<<5),
	LCD_Icon_Fan2   = (1<<6),
	LCD_Icon_Fan3   = (1<<7),
	LCD_Icon_All
} LCD_Icon_e;
*/

/*
typedef struct _LCD_Obj_
{
	uint8_t _lcd_buf[16];
	volatile uint8_t fan_cursor_slow=0;
	volatile uint8_t fan_cursor_med=0;
	volatile uint8_t fan_cursor_fast=0;
	unsigned blink_cursor:1;
	
} LCD_Obj;

typedef struct LCD_Obj *LCD_Handle;
*/

// **************************************************************************
// the functions

//void SPI_to_default_config(void);
void LCD_to_default_config(void);

void LCD_flush_buf_to_lcd(void);
void LCD_clear(void);
void LCD_set(void);

void LCD_fill_hour1(uint8_t num);
void LCD_clear_hour1(void);
void LCD_fill_min1(uint8_t num);
void LCD_clear_min1(void);
void LCD_fill_hour2(uint8_t num);
void LCD_clear_hour2(void);
void LCD_fill_min2(uint8_t num);
void LCD_clear_min2(void);
void LCD_fill_speed(uint8_t num);
void LCD_clear_speed(void);
void LCD_clear_fan(void);


void LCD_icon_on(LCD_Icon_e i);
void LCD_icon_off(LCD_Icon_e i);
void LCD_icon_toggle(LCD_Icon_e i);
void LCD_icon_fan(uint8_t fanNum);
void LCD_icon_rotate_fan(uint8_t spd);


_Bool LCD_get_blink_cursor(void);
uint8_t LCD_get_fan_cursor_slow(void);
uint8_t LCD_get_fan_cursor_fast(void);
uint8_t LCD_get_fan_cursor_med(void);

void LCD_blink_ISR100ms(void);

#ifdef DEBUGGING

void LCD_fill_pos_with_num(uint8_t pos, uint8_t num);

#endif

#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of __SPI_LCD_TM1723_H_ definition


