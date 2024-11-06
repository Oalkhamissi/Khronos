/**	@file Hebrew.cpp
	@author Garth Santor
	@date 2013-10-20

	Hebrew calendar class implementation.
	*/

#include <khronos/hebrew_calendar.hpp>


#include <khronos/islamic_calendar.hpp>
#include <khronos/utility.hpp>
#include <khronos/calendar.hpp>
#include <iostream>
#include <math.h>
namespace khronos {

	Hebrew::Hebrew() {
		time_t nowTime = time(NULL);
		struct tm tmNow;
		localtime_s(&tmNow, &nowTime);
		year_ = year_t(tmNow.tm_year) + 1900;
		month_ = month_t(tmNow.tm_mon) + 1;
		day_ = day_t(tmNow.tm_mday);
		hour_ = hour_t(tmNow.tm_hour),
			minute_ = minute_t(tmNow.tm_min),
			second_ = second_t(tmNow.tm_sec);
	}

	Hebrew::Hebrew(now_t NOTIMEOFDAY) {
		if (NOTIMEOFDAY == 1) {
			std::cout << "First" << std::endl;
			time_t nowTime = time(NULL);
			struct tm tmNow;
			localtime_s(&tmNow, &nowTime);
			year_ = year_t(tmNow.tm_year) + 1900;
			month_ = month_t(tmNow.tm_mon) + 1;
			day_ = day_t(tmNow.tm_mday);
			hour_ = hour_t(tmNow.tm_hour),
				minute_ = minute_t(tmNow.tm_min),
				second_ = second_t(tmNow.tm_sec);
		}
		else if (NOTIMEOFDAY == 0) {
			std::cout << "Second" << std::endl;
			time_t nowTime = time(NULL);
			struct tm tmNow;
			localtime_s(&tmNow, &nowTime);
			year_ = year_t(tmNow.tm_year) + 1900,
				month_ = month_t(tmNow.tm_mon) + 1,
				day_ = day_t(tmNow.tm_mday);
			hour_ = 0;
			minute_ = 0;
			second_ = 0;
		}
	}

	Hebrew::Hebrew(Jd j) {
		from_jd(j.jd());
	}

	inline year_t year(const Hebrew& g) { return g.year(); }
	inline month_t month(const Hebrew& g) { return g.month(); }
	inline day_t day(const Hebrew& g) { return g.day(); }
	inline hour_t hour(const Hebrew& g) { return g.hour(); }
	inline minute_t minute(const Hebrew& g) { return g.minute(); }
	inline second_t second(const Hebrew& g) { return g.second(); }
}