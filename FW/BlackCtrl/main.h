/*
 * main.h
 *
 *  Created on: Jul 13, 2013
 *      Author: Jack
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <msp430.h>
#include <stdint.h>
#include <stddef.h>
#include "TICAPS.h"
#include "IRremote.h"

#define LED_BACKLIGHT_ON	(P2OUT |= BIT1)
#define LED_BACKLIGHT_OFF	(P2OUT &= ~BIT1)
#define LED_TIMER_ON		(P3OUT |= BIT2)
#define LED_TIMER_OFF		(P3OUT &= ~BIT2)
#define LED_LAMP_ON		(P2OUT |= BIT2)
#define LED_LAMP_OFF		(P2OUT &= ~BIT2)
#define LED_PLUSMINUS_ON	(P1OUT |= BIT2)
#define LED_PLUSMINUS_OFF	(P1OUT &= ~BIT2)
#define LED_AUTO_ON		(P1OUT |= BIT1)
#define LED_AUTO_OFF		(P1OUT &= ~BIT1)

#define LED_TIMER_TOGGLE	(((P3OUT & BIT2)!= 0) ? LED_TIMER_OFF:LED_TIMER_ON)
#define LED_LAMP_TOGGLE		(((P2OUT & BIT2)!= 0) ? LED_LAMP_OFF:LED_LAMP_ON)
#define LED_PLUSMINUS_TOGGLE	(((P1OUT & BIT2)!= 0) ? LED_PLUSMINUS_OFF:LED_PLUSMINUS_ON)
#define LED_AUTO_TOGGLE		(((P1OUT & BIT1)!= 0) ? LED_AUTO_OFF:LED_AUTO_ON)
#define LED_BACKLIGHT_TOGGLE	(((P2OUT & BIT1)!= 0) ? LED_BACKLIGHT_OFF:LED_BACKLIGHT_ON)

#define LED_ALL_ON	LED_BACKLIGHT_ON; LED_TIMER_ON; LED_LAMP_ON; LED_PLUSMINUS_ON;
#define LED_ALL_OFF	LED_BACKLIGHT_OFF; LED_TIMER_OFF; LED_LAMP_OFF; LED_PLUSMINUS_OFF;

#define LED_INPUT_STATE		((P3IN & BIT6) != 0)




//white control
//LED PW MINUS PLUS LIGHT

//Universal board
//LED T1  T2  T3  T4

//Black control
//LED  T TP0 TP1  TP2
//LED input
//T TP0 TP1 --> Button output
//TP2       -->  Timing for Universal board


#define T_PORT			(P3OUT)
#define T_BIT			(BIT5)
#define TP0_PORT		(P2OUT)
#define TP0_BIT			(BIT5)
#define TP1_PORT		(P2OUT)
#define TP1_BIT			(BIT4)



#define NEW_BLACK_CTRL
#ifdef NEW_BLACK_CTRL
#define TP_PLUS_PORT		(TP1_PORT)
#define TP_PLUS_BIT		(TP1_BIT)
#endif
#ifndef NEW_BLACK_CTRL
#define TP_PLUS_PORT	(P2OUT)
#define TP_PLUS_BIT		(BIT3)
#endif

#define TP_MINUS_PORT		(TP0_PORT)
#define TP_MINUS_BIT		(TP0_BIT)
#define TP_PW_PORT		(T_PORT)
#define TP_PW_BIT		(T_BIT)

#define TP_PLUS_HIGH	(TP_PLUS_PORT |= TP_PLUS_BIT)
#define TP_PLUS_LOW		(TP_PLUS_PORT &= ~TP_PLUS_BIT)
#define TP_MINUS_HIGH	(TP_MINUS_PORT |= TP_MINUS_BIT)
#define TP_MINUS_LOW	(TP_MINUS_PORT &= ~TP_MINUS_BIT)
#define TP_PW_HIGH		(TP_PW_PORT |= TP_PW_BIT)
#define TP_PW_LOW		(TP_PW_PORT &= ~TP_PW_BIT)



#define BUTTON_TIMER_DETECTED	\
		TP_PW_HIGH; \
		TP_MINUS_LOW;\
		TP_PLUS_LOW;

#define BUTTON_MINUS_DETECTED \
		TP_PW_LOW; \
		TP_MINUS_HIGH;\
		TP_PLUS_LOW;

#define BUTTON_PLUS_DETECTED \
		TP_PW_HIGH; \
		TP_MINUS_HIGH;\
		TP_PLUS_LOW;

#define BUTTON_LAMP_DETECTED \
		TP_PW_LOW; \
		TP_MINUS_LOW;\
		TP_PLUS_HIGH;

#define BUTTON_NONE_DETECTED \
		TP_PW_LOW; \
		TP_MINUS_LOW;\
		TP_PLUS_LOW;

#define TP_ALL_HIGH \
	TP_PW_HIGH; \
	TP_MINUS_HIGH;\
	TP_PLUS_HIGH;



		

#define TP_LIGHT_PORT	(P3OUT)
#define TP_LIGHT_BIT	(BIT4)

#define OUTPUT_SEC_TOGGLE	(((TP_LIGHT_PORT & TP_LIGHT_BIT)!= 0) ? (TP_LIGHT_PORT &= ~TP_LIGHT_BIT):(TP_LIGHT_PORT |= TP_LIGHT_BIT))
#define OUTPUT_SEC_HIGH		(TP_LIGHT_PORT |= TP_LIGHT_BIT)



typedef struct _MAIN_Obj_
{
	uint8_t RTC_sec;
	uint8_t RTC_min;
	uint8_t RTC_hour;

	volatile uint8_t initialized;
} MAIN_Obj;

extern MAIN_Obj gMain;

void MainISR(void);
void CTRL_run(void);
void MainISR_timer1_capture(void);
#endif /* MAIN_H_ */
