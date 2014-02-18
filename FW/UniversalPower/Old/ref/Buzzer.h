/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUZZER_H_INCLUDED
#define __BUZZER_H_INCLUDED

#define BUZZER_DEFAULT_SOUND_FREQ_HZ	2000
#define BUZZER_DEFAULT_BIP_LENGHT_MS	200

#define BUZZER_TIMER_NUM				7
#define BUZZER_TIMER				TIM7


#define BUZZER_PORT_BASE				GPIOA_BASE
#define BUZZER_PIN_NUM				12

//call this to initialize timer ISR to make sound
void Buzzer_timer_to_default_state(void);

//put this in the Timer ISR to generate sound
void Buzzer_timerISR_make_sound(void);

//put this in systick ISR which tick ms to timing bip lenght of buzzer
void Buzzer_systickISR_timing_ms(void);

int Buzzer_bip(void);  //call this anywhere to make bip w/ default lenght
int Buzzer_bip_ms(uint16_t ms);  //making bip with specific lenght in ms



#endif
