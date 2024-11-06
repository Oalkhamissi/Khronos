#include <khronos/hebrew_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/calendar.hpp>
#include <cmath>
namespace khronos {
	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day) {
		jd = floor(jd) + 0.5;
		year_t count = floor(((jd - HEBREW_EPOCH) * 98496) / 35975351);
		year = count - 1;
		year_t i = count;
		while (jd >= jdn(i, 7, 1)) {
			year = year + 1;
			i = i + 1;
		}
		month_t first = 1;
		if (jd < jdn(year, 1, 1)) {
			first = 7;
		}
		month = first;
		month_t j = first;
		while (jd > jdn(year, j, daysInMonth(year, j))) {
			month = month + 1;
			j = j + 1;
		}
		day = floor(jd - jdn(year, month, 1) + 1);
	}

	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds) {
		jd_to_hebrew(jd, year, month, day);
		jd_to_hms(jd, hour, minute, seconds);
	}
}