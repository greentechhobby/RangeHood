#include "main.h"
#include "keyInput.h"


INPUT_Key_Obj gKeys;

void INPUT_key_init(void)
{

}

void INPUT_key_ISR50ms(void)
{
	uint8_t i;
	
	for(i=0; i<INPUT_numKeys; i++){
		switch(i){
		case 0:
			gKeys.keys[i].hw_bit = !TP1_BIT;
			break;
		case 1:
			gKeys.keys[i].hw_bit = !TP2_BIT;
			break;
		case 2:
			gKeys.keys[i].hw_bit =  !TP3_BIT;
			break;
		case 3:
			gKeys.keys[i].hw_bit =  !TP4_BIT;
			break;
		default:
			gKeys.keys[i].hw_bit =  INPUT_KEY_PASSIVE;
			break;
		}
		if(!gKeys.keys[i].hw_bit){
			if(gKeys.keys[i].debounce_cnt > gKeys.debounce_time_x50ms){
				gKeys.keys[i].detected = INPUT_KEY_ACTIVE;
			}else{
				gKeys.keys[i].debounce_cnt++;
			}
		}else{
			gKeys.keys[i].debounce_cnt = 0;
			gKeys.keys[i].detected = INPUT_KEY_PASSIVE;
		}
		
		//detect key hold
		if(gKeys.keys[i].high_level == INPUT_KEY_ACTIVE){
			if(gKeys.keys[i].hold_cnt > gKeys.hold_activate_time_x50ms){
				gKeys.keys[i].hold = INPUT_KEY_ACTIVE;
			}else{
				gKeys.keys[i].hold_cnt++;
			}
		}else{
			gKeys.keys[i].hold_cnt = 0;
			gKeys.keys[i].hold = INPUT_KEY_PASSIVE;
		}
	}
}