/**	@file Hebrew_to_jd.cpp
	@author Garth Santor
	@date 2012-09-14

	Hebrew calendar conversion implementations.
	*/

#include <khronos/hebrew_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/calendar.hpp>
namespace khronos {
	jd_t hebrew_to_jd(year_t year, month_t month, day_t day) {
		return hebrew_to_jd_integer(year, month, day);
	}

	jd_t hebrew_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds) {
		tod_t t = tod(hour, minute, seconds);
		if (t >= 0.5) {
			t -= 1;
		}
		return hebrew_to_jd_integer(year, month, day) + t;
	}
}