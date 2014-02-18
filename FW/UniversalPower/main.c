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


/* Includes ------------------------------------------------------------------*/

#include "main.h"
//#include "input.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void Cpu_to_default_config(void);
static void Ports_to_default_config(void);
static void Timers_to_default_config(void);


/* Private functions ---------------------------------------------------------*/
//__IO union _STM8_GPIO_IDR_REG *test1PortIDR = (union _STM8_GPIO_IDR_REG*)(GPIOA_BaseAddress+1);


INPUT_Key_Obj gKeys;
INPUT_Key_Handle keyHandle;

CTRL_Obj		gCtrl;
CTRL_Handle	ctrlHandle;

MAIN_WorkingMode_e gMainWorkingMode;

MAIN_Obj gMain;

void main(void)
{
	__disable_interrupt();
	Cpu_to_default_config();
	Ports_to_default_config();


	Check_working_mode();
	LCD_to_default_config();
	LCD_clear();
	LCD_flush_buf_to_lcd();

	Timers_to_default_config();
	__enable_interrupt();
	//

	keyHandle = INPUT_key_init(&gKeys,sizeof(gKeys));
	ctrlHandle = CTLR_init(&gCtrl, sizeof(gCtrl));
	gCtrl.inputHandle = keyHandle;
	CTRL_set_main_working_mode(ctrlHandle,gMainWorkingMode);

	while (1)
	{
		Main_background_loop();
	}

}

void Main_background_loop(void)
{

	CTRL_run(ctrlHandle);

	//Testing_LCD();
	LCD_flush_buf_to_lcd();

}

void MainISR_50ms(void)
{
	//Buzzer_off_timing_tick50ms();
	if(gMain.buzzer_50msTick_cnt){
		if(gMain.buzzer_50msTick_cnt < BUZZER_BIPLENGHT_X50MS){
			gMain.buzzer_50msTick_cnt++;
		}else{
			if(gMain.buzzer_doubleBip){
				gMain.buzzer_50msTick_cnt = 1;
				gMain.buzzer_doubleBip= 0;
			}else{
				gMain.buzzer_50msTick_cnt = 0;
			}
		}
	}

	INPUT_key_ISR50ms(keyHandle);
	CTRL_ISR50ms(ctrlHandle);
}

void MainISR_100ms(void)
{
	LCD_blink_ISR100ms();

}

void MainISR_1s(void)
{
	CTRL_ISR1s(ctrlHandle);
}

//50ms tick
void Timer2_ISR(void)
{
	static _Bool 	flag_50ms;
	static uint8_t cnt_100ms = 30;

	MainISR_50ms();

	if(flag_50ms == 0){
		flag_50ms = 1;
	}else{
		flag_50ms = 0;

		MainISR_100ms();
		cnt_100ms--;
		if(cnt_100ms==0){
			cnt_100ms = 30;
		}
		if(cnt_100ms%10 == 0)
			MainISR_1s();
	}
}

//125us tick
void Timer4_ISR(void)
{
	if(gMain.buzzer_50msTick_cnt == 0){
		BUZZER_OFF;
	}else{
		BUZZER_TOGGLE;
	}
}


void Buzzer_bip(void)
{
	if(gMain.buzzer_50msTick_cnt == 0){
		gMain.buzzer_50msTick_cnt = 1;
	}
}
void Buzzer_doubleBip(void)
{
	if(gMain.buzzer_50msTick_cnt == 0){
		gMain.buzzer_50msTick_cnt = 1;
		gMain.buzzer_doubleBip = 1;
	}
}



static void Cpu_to_default_config(void)
{
	//ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_3);
	//ITC_SetSoftwarePriority(ITC_IRQ_TIM2_OVF, ITC_PRIORITYLEVEL_2);

	//TIM2 OVF
	ITC->ISPR4 &= (uint8_t)(~(uint8_t)(0x03U << (((uint8_t)ITC_IRQ_TIM2_OVF % 4U) * 2U)));;
     ITC->ISPR4 |= (uint8_t)((uint8_t)(ITC_PRIORITYLEVEL_2) << (((uint8_t)ITC_IRQ_TIM2_OVF % 4U) * 2U));

     //TIM4
     ITC->ISPR6 &= (uint8_t)(~(uint8_t)(0x03U << (((uint8_t)ITC_IRQ_TIM4_OVF % 4U) * 2U)));;
     ITC->ISPR6 |= (uint8_t)((uint8_t)(ITC_PRIORITYLEVEL_3) << (((uint8_t)ITC_IRQ_TIM4_OVF % 4U) * 2U));

	CLK->CKDIVR = 0;                     //  Ensure the clocks are running at full speed.
}



static void Ports_to_default_config(void)
{

	GPIO_Init(BLOWER_FAN1_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BLOWER_FAN2_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN2_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BLOWER_FAN3_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN3_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BLOWER_FAN4_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN4_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BLOWER_FAN1_GPIO,(GPIO_Pin_TypeDef)BLOWER_FAN1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(LAMP_GPIO,(GPIO_Pin_TypeDef)LAMP_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(BUZZER_GPIO,(GPIO_Pin_TypeDef)BUZZER_PIN, GPIO_MODE_OUT_PP_LOW_FAST);


	GPIO_Init(SPI_DATA_GPIO, (GPIO_Pin_TypeDef)SPI_DATA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(SPI_CLK_GPIO, (GPIO_Pin_TypeDef)SPI_CLK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(SPI_STB_GPIO, (GPIO_Pin_TypeDef)SPI_STB_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	SPI_STB_HIGH;

	GPIO_Init(TP1_GPIO, (GPIO_Pin_TypeDef)TP1_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(TP2_GPIO, (GPIO_Pin_TypeDef)TP2_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(TP3_GPIO, (GPIO_Pin_TypeDef)TP3_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(TP4_GPIO, (GPIO_Pin_TypeDef)TP4_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(LED_GPIO, (GPIO_Pin_TypeDef)LED_PIN, GPIO_MODE_IN_PU_NO_IT);

}
uint16_t led_reset_cnt=0, tp_set_cnt[4]={0,0,0,0};

void Check_working_mode(void){


	uint32_t i;

		//detect if black control, white control or no control is attatched to port
	//if LED ==1 for 1xDelaytime => no control
	//if LED == 0 && TP1..TP4 == 1110 => black control.
	//if LED == 0 && TP1.. TP4 == 1111 => white control.


	//for black control:
	//TP1..TP3 == 0x01 .. 0x05  for at least 10 .. 20ms
	//TP1..TP3 == 0x07

	Delay_blocking(88888); //wait for white board to start up

	for(i=0; i<58888; i++){  //delay for white control to start up
		if(LED_BIT == RESET)
			led_reset_cnt++;
		if(TP1_BIT)
			tp_set_cnt[0]++;
		if(TP2_BIT)
			tp_set_cnt[1]++;
		if(TP3_BIT)
			tp_set_cnt[2]++;
		if(TP4_BIT)
			tp_set_cnt[3]++;
	}

	GPIO_Init(LED_GPIO, (GPIO_Pin_TypeDef)LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);


	if(led_reset_cnt < 28888)
	{
		if(tp_set_cnt[1]>28888){
			gMainWorkingMode = MAIN_WorkingMode_BlackCtrl;
			//while(1);//TODO: sleep LP mode
		}else{
			gMainWorkingMode = MAIN_WorkingMode_Standby;
			while(1);//TODO: sleep LP mode
		}

	}else{

		gMainWorkingMode = MAIN_WorkingMode_WhiteCtrl;
	}
}

static void Timers_to_default_config(void)
{
	//Timer 4 used for 125us Tick

	//TIM4CLK = 16MHz/16  = 1000kHz
	//Period = 1000k/freq_hz -1
	//TIM4_TimeBaseInit(TIM4_PRESCALER_16,(uint8_t)(1000000/8000-1));
	TIM4->PSCR = (uint8_t)(TIM4_PRESCALER_16);
     TIM4->ARR = (uint8_t)((1000/8-1));

	//TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	TIM4->SR1 = (uint8_t)(~TIM4_FLAG_UPDATE);
	//TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	TIM4->IER |= (uint8_t)TIM4_IT_UPDATE;




	//Timer2 used for 50ms Tick
	//TIM2_DeInit();
	//TIM2_TimeBaseInit(TIM2_PRESCALER_128,(uint16_t)(125000/20-1));

    TIM2->PSCR = (uint8_t)(TIM2_PRESCALER_128);
    TIM2->ARRH = (uint8_t)((uint16_t)(125000/20-1) >> 8);
    TIM2->ARRL = (uint8_t)((uint16_t)(125000/20-1));

	//TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	TIM2->SR1 = (uint8_t)(~((uint8_t)(TIM2_FLAG_UPDATE)));
     TIM2->SR2 = (uint8_t)(~((uint8_t)((uint8_t)TIM2_FLAG_UPDATE >> 8)));

	//TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);
	TIM2->IER |= (uint8_t)TIM2_IT_UPDATE;

	//TIM2_Cmd(ENABLE);
	//TIM4_Cmd(ENABLE);
	TIM2->CR1 |= (uint8_t)TIM2_CR1_CEN;
	TIM4->CR1 |= TIM4_CR1_CEN;

}






void Delay_blocking(__IO uint32_t nCount)
{
	while(nCount !=0)
	{
		nCount--;
	}
}

#ifdef DEBUGGING

uint8_t test_vars[8];
void Testing_LCD(void)
{
	if(test_vars[3]==1){
		LCD_fill_hour1(test_vars[0]);
		LCD_fill_min1(test_vars[1]);
		LCD_icon_rotate_fan(test_vars[2]);

		if(LCD_get_blink_cursor()){
			LCD_icon_on(LCD_Icon_Clock);
			LCD_icon_on(LCD_Icon_Colon1);
			LCD_icon_on(LCD_Icon_Light);
		}else{
			LCD_icon_off(LCD_Icon_Clock);
			LCD_icon_off(LCD_Icon_Colon1);
			LCD_icon_off(LCD_Icon_Light);
		}
	}
	if(test_vars[3]==2){
		LCD_fill_pos_with_num(0,test_vars[0]);
		LCD_fill_pos_with_num(1,test_vars[1]);
		LCD_fill_pos_with_num(2,test_vars[2]);
		LCD_fill_pos_with_num(3,test_vars[4]);
	}
}

#endif

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2013 GTSV incorporated *****END OF FILE****/
