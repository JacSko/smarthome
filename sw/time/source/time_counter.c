/* =============================
 *   Includes of common headers
 * =============================*/
#include <stdlib.h>
/* =============================
 *  Includes of project headers
 * =============================*/
#include "stm32f4xx.h"
#include "time_counter.h"
#include "core_cmFunc.h"
#include "Logger.h"
/* =============================
 *          Defines
 * =============================*/
#define TIME_CNT_CALLBACK_MAX_SIZE 10
#define TIME_BASETIME_MS 10
/* =============================
 *   Internal module functions
 * =============================*/
RET_CODE time_is_time_ok(TimeItem* item);
RET_CODE time_is_leap_year();
void time_increment_time();
void time_call_low_prio_callbacks();
void time_call_high_prio_callbacks();
/* =============================
 *       Internal types
 * =============================*/
typedef struct TimeCallbackItem
{
   TimeCallbackPriority priority;
   void(*fnc)(TimeItem*);
} TimeCallbackItem;
/* =============================
 *      Module variables
 * =============================*/
TimeItem timestamp;
volatile uint8_t time_time_changed;
TimeCallbackItem TIME_CALLBACKS[TIME_CNT_CALLBACK_MAX_SIZE];
uint8_t winter_time_active = 1;
uint8_t month_day_cnt[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};



void time_init()
{
	timestamp.day = 1;
	timestamp.month = 1;
	timestamp.year = 2000;
	timestamp.hour = 0;
	timestamp.minute = 0;
	timestamp.second = 0;
	timestamp.msecond = 0;
	/* systick_value = F_CPU/(100/PERIOD[ms]) */
	SysTick_Config(100000000/(1000/TIME_BASETIME_MS));

}

void time_deinit()
{
	for (uint8_t i = 0; i < TIME_CNT_CALLBACK_MAX_SIZE; i ++)
	{
	   TIME_CALLBACKS[i].fnc = NULL;
	}
}

RET_CODE time_is_time_ok(TimeItem* item)
{
	uint8_t result = 1;
	result &= item->month <= 12;
	if (result == RETURN_OK)
	{
		result &= item->day <= month_day_cnt[item->month];
	}
	result &= item->hour <= 23;
	result &= item->minute <= 59;
	result &= item->second <= 59;
	result &= item->msecond <= 990 && (item->msecond%10) == 0;

	return result? RETURN_OK : RETURN_NOK;

}

void time_set_winter_time(uint8_t state)
{
	winter_time_active = state;
}

RET_CODE time_set_utc(TimeItem* item)
{
	RET_CODE result = RETURN_NOK;
	if (item)
	{
		if (time_is_time_ok(item))
		{
			__disable_irq();
			timestamp = *item;
			if (winter_time_active)
			{
				timestamp.hour += 1;
			}
			else
			{
				timestamp.hour += 2;
			}
			if (timestamp.hour > 23)
			{
				timestamp.hour = timestamp.hour % 24;
				timestamp.day++;
				if (timestamp.day > month_day_cnt[timestamp.month])
				{
					timestamp.month++;
					timestamp.day = 1;
					if (timestamp.month > 12)
					{
						timestamp.month = 1;
						timestamp.year++;
					}
				}
			}
			__enable_irq();
			result = RETURN_OK;
		}
	}
	return result;
}

TimeItem* time_get()
{
	return &timestamp;
}
RET_CODE time_register_callback(void(*callback)(TimeItem*), TimeCallbackPriority prio)
{
	RET_CODE result = RETURN_NOK;

	for (uint8_t i = 0; i < TIME_CNT_CALLBACK_MAX_SIZE; i++)
	{
		if (TIME_CALLBACKS[i].fnc == NULL)
		{
		   TIME_CALLBACKS[i].priority = prio;
		   TIME_CALLBACKS[i].fnc = callback;
			result = RETURN_OK;
			break;
		}
	}
	return result;
}

RET_CODE time_unregister_callback(void(*callback)(TimeItem*))
{
	RET_CODE result = RETURN_NOK;
	for (uint8_t i = 0; i < TIME_CNT_CALLBACK_MAX_SIZE; i++)
	{
		if (TIME_CALLBACKS[i].fnc == callback)
		{
		   TIME_CALLBACKS[i].fnc = NULL;
			result = RETURN_OK;
		}
	}
	return result;
}

RET_CODE time_is_leap_year()
{
	return (timestamp.year % 4) == 0? RETURN_OK : RETURN_NOK;
}

void time_increment_time()
{
	timestamp.msecond += TIME_BASETIME_MS;
	if (timestamp.msecond >= 1000)
	{
		timestamp.second++;
		timestamp.msecond %= 1000;
	}
	if (timestamp.second == 60)
	{
		timestamp.minute++;
		timestamp.second = 0;
	}
	if (timestamp.minute == 60)
	{
		timestamp.hour++;
		timestamp.minute = 0;
	}
	if (timestamp.hour == 24)
	{
		timestamp.day++;
		timestamp.hour = 0;
		uint8_t exp_days = (time_is_leap_year() == RETURN_OK) && (timestamp.month == 2)? month_day_cnt[timestamp.month] +1 : month_day_cnt[timestamp.month];
		if (timestamp.day >= exp_days)
		{
			timestamp.month++;
			timestamp.day = 1;
			if (timestamp.month > 12)
			{
				timestamp.year++;
				timestamp.month = 1;
			}
		}
	}


}

void time_call_low_prio_callbacks()
{
	for (uint8_t i = 0; i < TIME_CNT_CALLBACK_MAX_SIZE; i++)
	{
		if (TIME_CALLBACKS[i].fnc != NULL && TIME_CALLBACKS[i].priority == TIME_PRIORITY_LOW)
		{
		   TIME_CALLBACKS[i].fnc(&timestamp);
		}
	}
}

void time_call_high_prio_callbacks()
{
   for (uint8_t i = 0; i < TIME_CNT_CALLBACK_MAX_SIZE; i++)
   {
      if (TIME_CALLBACKS[i].fnc != NULL && TIME_CALLBACKS[i].priority == TIME_PRIORITY_HIGH)
      {
         TIME_CALLBACKS[i].fnc(&timestamp);
      }
   }
}

void time_watcher()
{
	if (time_time_changed)
	{
		time_time_changed = 0;
		time_call_low_prio_callbacks();
	}
}
uint16_t time_get_basetime()
{
	return TIME_BASETIME_MS;
}

void SysTick_Handler(void)
{
	time_increment_time();
	time_call_high_prio_callbacks();
   time_time_changed++;
}

