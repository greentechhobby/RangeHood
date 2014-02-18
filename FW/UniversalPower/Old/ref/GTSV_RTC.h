#ifndef __GTSV_RTC_H_INCLUDED
#define __GTSV_RTC_H_INCLUDED

extern RTC_TimeTypeDef RTC_TimeStructure;
extern RTC_DateTypeDef RTC_DateStructure;

void RTC_to_default_config(void);
void RTC_change_time(uint8_t hr, uint8_t min, uint8_t s);

#endif