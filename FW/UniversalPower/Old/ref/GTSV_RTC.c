#include "main.h"
#include "GTSV_RTC.h"



RTC_InitTypeDef   RTC_InitStructure;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef   RTC_DateStructure;




/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);


	/* Allow access to RTC */
	//PWR_RTCAccessCmd(ENABLE);

	//Enable the LSE OSC
	RCC_LSEConfig(RCC_LSE_ON);
	//wait until LSE is ready
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}

	//Select RTC clk source
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	//SynchPrediv = 0xFF;
	//AsynchPrediv = 0x7F;

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();


	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv	=  0xFF; /* (32KHz / 128) - 1 = 0xFF*/
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);


	RTC_WakeUpCmd(DISABLE);
	/* EXTI configuration *******************************************************/
	EXTI_ClearITPendingBit(EXTI_Line20);
	EXTI_InitStructure.EXTI_Line = EXTI_Line20;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable the RTC Wakeup Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(RTC_WKUP_IRQn, INT_PRIORITY_WKUP);
		//RTCCLK=32768Hz ; div=16  =>2048Hz
	RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
		
  	//div 256 =>8Hz  ~ 125ms
	RTC_SetWakeUpCounter(0xFF);	
	
	RTC_ClearITPendingBit(RTC_IT_WUT);
	  EXTI_ClearITPendingBit(EXTI_Line20);
	/* Enable the RTC Wakeup Interrupt */
	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	/* Enable Wakeup Counter */
	RTC_WakeUpCmd(ENABLE);

	//PWR_RTCAccessCmd(DISABLE);

	//disable PWR clock
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, DISABLE);
}



void RTC_to_default_config(void)
{
	//enable clk for PWR & allow access to RTC
	/* Enable the PWR clock */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/* Allow access to RTC */
	PWR_RTCAccessCmd(ENABLE);
	
	/* RTC Configuration -------------------------------------------------------*/
	RTC_Config();
	RTC_DateStructure.RTC_Month = RTC_Month_May;
	RTC_DateStructure.RTC_Date = 1;
	RTC_DateStructure.RTC_Year = 13;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Wednesday;


	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = 12;
	RTC_TimeStructure.RTC_Minutes = 00;
	RTC_TimeStructure.RTC_Seconds = 00;
	
	
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
	//RTC_SetDate(RTC_Format_BIN,&RTC_DateStructure);

	//disable access to RTC & disable clk to PWR
	PWR_RTCAccessCmd(DISABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, DISABLE);
}

void RTC_change_time(uint8_t hr, uint8_t min, uint8_t s)
{
	PWR_RTCAccessCmd(ENABLE);
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = hr;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_TimeStructure.RTC_Seconds = s;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
	PWR_RTCAccessCmd(DISABLE);
}
