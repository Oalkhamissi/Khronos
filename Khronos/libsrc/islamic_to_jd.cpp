/**	@file Islamic_to_jd.cpp
	@author Garth Santor
	@date 2013-09-19

	Islamic calendar conversion implementations.
	*/

#include <khronos/islamic_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/calendar.hpp>
namespace khronos {
	jd_t islamic_to_jd(year_t year, month_t month, day_t day) {
		/*assert(month >= 1 && "Minimum month is Muharram = 1");
		assert(month <= 12 && "Maximum month is DhulHajjah = 12");
		assert(day >= 1 && "Minimum day of the month is 1");
		assert(day <= 30 && "Maximum day of the month is 30");*/
		//assert(islamic_to_jd_integer(year, month, day) == islamic_to_jd_real(year, month, day));
		return islamic_to_jd_integer(year, month, day);
	}

	jd_t islamic_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds) {
		/*assert(month >= 1 && "Minimum month is Muharram = 1");
		assert(month <= 12 && "Maximum month is DhulHajjah = 12");
		assert(day >= 1 && "Minimum day of the month is 1");
		assert(day <= 30 && "Maximum day of the month is 30");*/
		//assert(islamic_to_jd_integer(year, month, day) == islamic_to_jd_real(year, month, day));
		tod_t t = tod(hour, minute, seconds);
		if (t >= 0.5) {
			t -= 1;
		}
		return islamic_to_jd_integer(year, month, day) + t;
	}
}