/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GTSV_SPILCD_TM172x_H_INCLUDED
#define __GTSV_SPILCD_TM172x_H_INCLUDED
#include "main.h"


enum Lcd_Icons_Enum_t {
	LCD_ICON_CLOCK,
	LCD_ICON_LIGHT,
	LCD_ICON_COLON1,
	LCD_ICON_COLON2,
	LCD_ICON_FAN1,
	LCD_ICON_FAN2,
	LCD_ICON_FAN3,
	LCD_ICON_ALL
};

#define LCD_POS_FAN_SPEED	(8)

extern uint8_t _lcd_buf[16];
extern const uint8_t _num_to_7seg2[12];
extern const uint8_t _num_to_7seg1[12];
extern const uint8_t _number_to_7segments[12];
extern const uint8_t _num_to_7seg0[12];
extern const uint8_t _num_pos[9];

void Spilcd_configure_GPIO(void);
void Spilcd_to_default_config(void);

void Spilcd_flush_buf_to_lcd(void);
void Lcd_clear(void);
void Lcd_set(void);


void Lcd_icon_on(enum Lcd_Icons_Enum_t icon);
void Lcd_icon_off(enum Lcd_Icons_Enum_t icon);
void Lcd_icon_toggle(enum Lcd_Icons_Enum_t icon);
void Lcd_icon_fan(uint8_t num);


uint8_t Lcd_get_blink_cursor(void);
uint8_t Lcd_get_fan_cursor_slow(void);
uint8_t Lcd_get_fan_cursor_fast(void);

void Lcd_blink_tick125ms(void);

void Lcd_fill_pos_with_num(uint8_t pos, uint8_t num);
void Lcd_fill_2pos_with_num(uint8_t pos, uint8_t num);

void Lcd_fill_hour1(uint8_t num);
void Lcd_fill_hour2(uint8_t num);
void Lcd_fill_min1(uint8_t num);
void Lcd_fill_min2(uint8_t num);

void Lcd_clear_hour2_min2(void);
void Lcd_clear_hour1_min1(void);

void Lcd_clear_min1(void);
void Lcd_clear_hour1(void);


#endif
