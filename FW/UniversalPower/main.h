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
#ifndef __MAIN_H_
#define __MAIN_H_


//! \file   main.h
//! \brief  Contains global definitions & vars
//!
//!
//! (C) Copyright 2013, GTSV, Inc.


// **************************************************************************
// the includes

#include <intrinsics.h>
#include <stddef.h>
#include "stm8s.h"



//!
//!
//! \defgroup Main Main
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif


#ifndef NULL
#define NULL 0
#endif

#define DEBUGGING

#define BLOWER_FAN1_GPIO	GPIOD
#define BLOWER_FAN1_PIN	GPIO_PIN_3

#define BLOWER_FAN2_GPIO	GPIOD
#define BLOWER_FAN2_PIN	GPIO_PIN_2

#define BLOWER_FAN3_GPIO	GPIOC
#define BLOWER_FAN3_PIN	GPIO_PIN_7

#define BLOWER_FAN4_GPIO	GPIOC
#define BLOWER_FAN4_PIN	GPIO_PIN_3

#define LAMP_GPIO		GPIOB
#define LAMP_PIN		GPIO_PIN_4

#define BUZZER_GPIO		GPIOB
#define BUZZER_PIN		GPIO_PIN_5

#define LED_GPIO		GPIOA
#define LED_PIN		GPIO_PIN_2

#define TP1_GPIO		GPIOA
#define TP1_PIN		GPIO_PIN_1

#define TP2_GPIO		GPIOD
#define TP2_PIN		GPIO_PIN_4

#define TP3_GPIO		GPIOD
#define TP3_PIN		GPIO_PIN_5

#define TP4_GPIO		GPIOA
#define TP4_PIN		GPIO_PIN_3

#define SPI_DATA_GPIO	GPIOC
#define SPI_DATA_PIN	GPIO_PIN_6
#define SPI_CLK_GPIO	GPIOC
#define SPI_CLK_PIN		GPIO_PIN_5
#define SPI_STB_GPIO	GPIOC
#define SPI_STB_PIN		GPIO_PIN_4

//#define OLD_UNIVERSAL_BOARD

#ifdef OLD_UNIVERSAL_BOARD
#define TP1_BIT			((BitStatus)((TP1_GPIO->IDR & (uint8_t)TP1_PIN)))
#define TP2_BIT			((BitStatus)((TP2_GPIO->IDR & (uint8_t)TP2_PIN)))
#define TP3_BIT			((BitStatus)((TP3_GPIO->IDR & (uint8_t)TP3_PIN)))
#define TP4_BIT			((BitStatus)((TP4_GPIO->IDR & (uint8_t)TP4_PIN)))
#endif

#ifndef OLD_UNIVERSAL_BOARD
#define TP1_BIT			(!((BitStatus)((TP1_GPIO->IDR & (uint8_t)TP1_PIN))))
#define TP2_BIT			(!((BitStatus)((TP2_GPIO->IDR & (uint8_t)TP2_PIN))))
#define TP3_BIT			(!((BitStatus)((TP3_GPIO->IDR & (uint8_t)TP3_PIN))))
#define TP4_BIT			(!((BitStatus)((TP4_GPIO->IDR & (uint8_t)TP4_PIN))))
#endif

#define LED_BIT			((BitStatus)((LED_GPIO->IDR & (uint8_t)LED_PIN)))

#define BLOWER_FAN1_ON		BLOWER_FAN1_GPIO->ODR |= (uint8_t)BLOWER_FAN1_PIN
#define BLOWER_FAN1_OFF		BLOWER_FAN1_GPIO->ODR &= (uint8_t)(~BLOWER_FAN1_PIN)
#define BLOWER_FAN2_ON		BLOWER_FAN2_GPIO->ODR |= (uint8_t)BLOWER_FAN2_PIN
#define BLOWER_FAN2_OFF		BLOWER_FAN2_GPIO->ODR &= (uint8_t)(~BLOWER_FAN2_PIN)
#define BLOWER_FAN3_ON		BLOWER_FAN3_GPIO->ODR |= (uint8_t)BLOWER_FAN3_PIN
#define BLOWER_FAN3_OFF		BLOWER_FAN3_GPIO->ODR &= (uint8_t)(~BLOWER_FAN3_PIN)
#define BLOWER_FAN4_ON		BLOWER_FAN4_GPIO->ODR |= (uint8_t)BLOWER_FAN4_PIN
#define BLOWER_FAN4_OFF		BLOWER_FAN4_GPIO->ODR &= (uint8_t)(~BLOWER_FAN4_PIN)


#define LAMP_ON			LAMP_GPIO->ODR |= (uint8_t)LAMP_PIN
#define LAMP_OFF			LAMP_GPIO->ODR &= (uint8_t)(~LAMP_PIN)
#define LED_ON				LED_GPIO->ODR |= (uint8_t)LED_PIN
#define LED_OFF			LED_GPIO->ODR &= (uint8_t)(~LED_PIN)
#define BUZZER_ON			BUZZER_GPIO->ODR |= (uint8_t)BUZZER_PIN
#define BUZZER_OFF			BUZZER_GPIO->ODR &= (uint8_t)(~BUZZER_PIN)
#define BUZZER_TOGGLE		BUZZER_GPIO->ODR ^= (uint8_t)BUZZER_PIN

#define SPI_STB_HIGH		SPI_STB_GPIO->ODR |= (uint8_t)SPI_STB_PIN
#define SPI_STB_LOW			SPI_STB_GPIO->ODR &= (uint8_t)(~SPI_STB_PIN)


#define BUZZER_BIPLENGHT_X50MS	2

// **************************************************************************
// the typedefs

//#ifndef size_t
//typedef uint16_t size_t;
//#endif

typedef enum {
	MAIN_WorkingMode_Standby,
	MAIN_WorkingMode_BlackCtrl,
	MAIN_WorkingMode_WhiteCtrl
} MAIN_WorkingMode_e;

typedef struct _MAIN_Obj_
{
	//unsigned buzzer_doubleBip:1;

	uint8_t buzzer_doubleBip;
	uint8_t buzzer_50msTick_cnt;
} MAIN_Obj;

extern MAIN_WorkingMode_e gMainWorkingMode;

void Buzzer_bip(void);
void Buzzer_doubleBip(void);

void Check_working_mode(void);


void MainISR_50ms(void);
void MainISR_100ms(void);
void MainISR_1s(void);


void Timer4_ISR(void);
void Timer2_ISR(void);

void Main_background_loop(void);
void Delay_blocking(__IO uint32_t nCount);
void Testing_LCD(void);



#include "SPI_LCD_TM1723.h"
#include "input.h"
#include "UI_WhiteCTRL.h"

#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of __MAIN_H_ definition

