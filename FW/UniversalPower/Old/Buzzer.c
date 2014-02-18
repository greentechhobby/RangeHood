#include "main.h"
#include "buzzer.h"



//buzzer private global vars
static uint16_t	_gBuzzer_50msTick = 0;
static uint16_t	_gBuzzer_bip_lenght = 3;
static uint8_t 	_gBuzzer_bip_cnt;

static _Bool		_gBuzzer_sound_on;

int Buzzer_bip(void)
{

	if(_gBuzzer_50msTick == 0){
		_gBuzzer_50msTick = 1;
		_gBuzzer_bip_cnt = 1;
		return 0;
	} else {
		return -1;
	}

}
int Buzzer_2bips(void)
{

	if(_gBuzzer_50msTick == 0){
		_gBuzzer_50msTick = 1;
		_gBuzzer_bip_cnt = 2;
		return 0;
	} else {
		return -1;
	}
}

int Buzzer_3bips(void)
{

	if(_gBuzzer_50msTick == 0){
		_gBuzzer_50msTick = 1;
		_gBuzzer_bip_cnt = 3;
		return 0;
	} else {
		return -1;
	}
}

void Buzzer_sound_tickISR(void)
{
	if(!_gBuzzer_sound_on){
		BUZZER_OFF;
	}else{
		BUZZER_TOGGLE;
	}
}

void Buzzer_off_timing_tick50ms(void)
{

	if(_gBuzzer_50msTick){
		if(_gBuzzer_50msTick < _gBuzzer_bip_lenght){
			if(_gBuzzer_50msTick == 1)
				//TIM_Cmd(BUZZER_TIMER, ENABLE);
				_gBuzzer_sound_on = ENABLE;
			_gBuzzer_50msTick++;
		}else{
			//TIM_Cmd(BUZZER_TIMER, DISABLE);
			_gBuzzer_sound_on = DISABLE;
			_gBuzzer_bip_cnt--;
			if(_gBuzzer_bip_cnt == 0)
				_gBuzzer_50msTick = 0;
			else
				_gBuzzer_50msTick = 1;
		}
	}
}
