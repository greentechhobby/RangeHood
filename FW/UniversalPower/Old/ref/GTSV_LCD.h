 /**
  ******************************************************************************
  * @file    stm32l_discovery_lcd.h
  * @author  Microcontroller Division
  * @version V1.0.0
  * @date    Apri-2011
  * @brief   This file contains all the functions prototypes for the glass LCD
  *          firmware driver.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GTSV_LCD_H_INCLUDED
#define __GTSV_LCD_H_INCLUDED

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"   

/*  =========================================================================
                                 LCD MAPPING
    =========================================================================
     LCD COM0: 

*/
enum Lcd_Icons {
	LCD_ICON_CLOCK,
	LCD_ICON_LIGHT,
	LCD_ICON_COLON1,
	LCD_ICON_COLON2,
	LCD_ICON_FAN1,
	LCD_ICON_FAN2,
	LCD_ICON_FAN3,
	LCD_ICON_ALL
};


void Lcd_configure_GPIO(void);
void Lcd_to_default_config(void);

void Lcd_clear(void);


void Lcd_fill_pos_with_num(uint8_t num, uint8_t pos);
void Lcd_fill_pos_with_double_digit_num(uint8_t num, uint8_t pos);

void Lcd_fill_hours(uint8_t num);
void Lcd_fill_mins(uint8_t num);
void Lcd_icon_on(enum Lcd_Icons icon);
void Lcd_icon_off(enum Lcd_Icons icon);
void Lcd_icon_toggle(enum Lcd_Icons icon);
void Lcd_icon_fan(uint8_t num);
void Lcd_blink_systicISR_ms(void);
void Lcd_icon_buff_flush(void);
uint8_t Lcd_get_blink_cursor(void);
uint8_t Lcd_get_fan_cursor_slow(void);
uint8_t Lcd_get_fan_cursor_fast(void);


void Lcd_fill_pos_with_blank(uint8_t pos);
void Lcd_fill_pos_with_num(uint8_t pos, uint8_t num);

void Lcd_com_seg_set(uint8_t com, uint8_t seg);
void Lcd_com_seg_clear(uint8_t com, uint8_t seg);

#endif /* stm32l_discovery_lcd*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
