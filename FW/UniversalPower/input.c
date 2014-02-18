#include "main.h"


INPUT_Key_Handle INPUT_key_init(void *pMemory,const size_t numBytes)
{
	//INPUT_Key_Handle inputHandle;
	INPUT_Key_Obj *input;


	//if(numBytes < sizeof(INPUT_Key_Obj))
	//return((INPUT_Key_Handle)NULL);

	// assign the handle
	//inputHandle = (INPUT_Key_Handle)pMemory;

	// assign the object
	input = (INPUT_Key_Obj *)pMemory;

	//set default debounce time
	input->debounce_time_x50ms = INPUT_KEY_DEBOUNCE_TIME_X50MS;
	input->hold_activate_time_x50ms = INPUT_KEY_HOLD_ACTIVATE_TIME_X50MS;

	return((INPUT_Key_Handle)pMemory);
}



#define INPUT_HWKEY_ON(key, obj) \
		if(key->debounce_cnt > obj->debounce_time_x50ms){\
			key->detected = 1;\
		}else{							\
			key->debounce_cnt++;	\
		}


#define INPUT_HWKEY_OFF(key)\
		key->debounce_cnt = 0;	\
		key->detected = 0;




void INPUT_key_ISR50ms(INPUT_Key_Handle inputKeyHandle)
{
	INPUT_Key_Obj *obj = (INPUT_Key_Obj *)inputKeyHandle;
	INPUT_Key_State *key;

	uint8_t i;

	if(gMainWorkingMode == MAIN_WorkingMode_WhiteCtrl){
		key = &obj->keys[INPUT_Key_Power];
		if(TP1_BIT){
			INPUT_HWKEY_ON(key, obj);
		}else{
			INPUT_HWKEY_OFF(key);
		}

		key = &obj->keys[INPUT_Key_Minus];
		if(TP2_BIT){
			INPUT_HWKEY_ON(key, obj);
		}else{
			INPUT_HWKEY_OFF(key);
		}

		key = &obj->keys[INPUT_Key_Plus];
		if(TP3_BIT){
			INPUT_HWKEY_ON(key, obj);
		}else{
			INPUT_HWKEY_OFF(key);
		}

		key = &obj->keys[INPUT_Key_Light];
		if(TP4_BIT){
			INPUT_HWKEY_ON(key, obj);
		}else{
			INPUT_HWKEY_OFF(key);
		}

		key = &obj->keys[INPUT_Key_Any];
		if(TP4_BIT || TP1_BIT || TP2_BIT || TP3_BIT){
			INPUT_HWKEY_ON(key, obj);
		}else{
			INPUT_HWKEY_OFF(key);
		}
	}else if(gMainWorkingMode == MAIN_WorkingMode_BlackCtrl){
		if((TP1_BIT) && (!TP2_BIT) &&(!TP3_BIT)){
			key = &obj->keys[INPUT_Key_Power];
		}else if((!TP1_BIT) && (TP2_BIT) &&(!TP3_BIT)){
			key = &obj->keys[INPUT_Key_Minus];
		}else if((TP1_BIT) && (TP2_BIT) &&(!TP3_BIT)){
			key = &obj->keys[INPUT_Key_Plus];
		}else if((!TP1_BIT) && (!TP2_BIT) &&(TP3_BIT)){
			key = &obj->keys[INPUT_Key_Light];
		}else{
			for(i=0; i<INPUT_numKeys; i++){
				key = &obj->keys[i];
				INPUT_HWKEY_OFF(key);
			}

			key = NULL;
		}
		
		if(key != NULL){
			INPUT_HWKEY_ON(key, obj);
			key = &obj->keys[INPUT_Key_Any];
			INPUT_HWKEY_ON(key, obj);
		}
	}
	
	for(i=0; i<INPUT_numKeys; i++){
		key = &obj->keys[i];

		//detect key hold
		if(key->high_level){
			if(key->hold_cnt > obj->hold_activate_time_x50ms){
				key->hold = 1;
				key->hold_late = 1;
			}else{
				key->hold_cnt++;
			}
		}else{
			key->hold_cnt = 0;
			key->hold = 0;
			key->hold_late = 0;
		}
	}

}



void INPUT_key_run(INPUT_Key_Handle inputKeyHandle)
{
	INPUT_Key_Obj *obj = (INPUT_Key_Obj *)inputKeyHandle;

	uint8_t i;

	//obj->keys[INPUT_Key_Any].rising_edge = 0;
	for(i=0; i<(INPUT_numKeys); i++){
		INPUT_Key_State *key = &obj->keys[i];

		//after risng edge => high_level =1
		if(key->rising_edge){
			key->rising_edge = 0;
			key->high_level = 1;
		}

		//after falling edge -> high_level =0
		if(key->falling_edge){
			key->falling_edge = 0;
			key->hold_late = 0;
		}

		//rising edge is key detected while not high level
		if((key->detected)
			&& (!key->high_level))
		{
			key->rising_edge = 1;
		}

		//falling edge is when key not detected & high_level =1
		if((!key->detected)
			&& (key->high_level))
		{
			key->falling_edge = 1;
			key->high_level = 0;
			key->hold = 0;
		}

		//obj->keys[INPUT_Key_Any].rising_edge |= key->rising_edge;
	}

}

_Bool INPUT_key_check(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key)
{
	INPUT_Key_Obj *obj = (INPUT_Key_Obj *)inputKeyHandle;
	return (obj->keys[key].rising_edge);

}

_Bool INPUT_key_check_down(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key)
{
	INPUT_Key_Obj *obj = (INPUT_Key_Obj *)inputKeyHandle;
	return (obj->keys[key].rising_edge);
}

_Bool INPUT_key_check_up(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key)
{
	INPUT_Key_Obj *obj = (INPUT_Key_Obj *)inputKeyHandle;
	return (obj->keys[key].falling_edge);
}

_Bool INPUT_key_check_touching(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key)
{
	INPUT_Key_Obj *obj = (INPUT_Key_Obj *)inputKeyHandle;
	return (obj->keys[key].high_level);
}

_Bool INPUT_key_check_holding_late(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key)
{
	INPUT_Key_Obj *obj = (INPUT_Key_Obj *)inputKeyHandle;
	return (obj->keys[key].hold_late);
}
_Bool INPUT_key_check_holding(INPUT_Key_Handle inputKeyHandle, INPUT_Key_e key)
{
	INPUT_Key_Obj *obj = (INPUT_Key_Obj *)inputKeyHandle;
	return (obj->keys[key].hold);
}

