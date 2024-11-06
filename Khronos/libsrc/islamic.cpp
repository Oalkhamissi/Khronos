/**	@file Islamic.cpp
	@author Garth Santor
	@date 2013-09-20

	Islamic calendar class implementation.
	*/

#include <khronos/islamic_calendar.hpp>
#include <khronos/utility.hpp>
#include <khronos/calendar.hpp>
#include <iostream>
#include <math.h>
namespace khronos {

	Islamic::Islamic() {
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

	Islamic::Islamic(now_t NOTIMEOFDAY) {
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

	Islamic::Islamic(Jd j) {
		from_jd(j.jd());
	}



	
	inline year_t year(const Islamic& g) { return g.year(); }
	inline month_t month(const Islamic& g) { return g.month(); }
	inline day_t day(const Islamic& g) { return g.day(); }
	inline hour_t hour(const Islamic& g) { return g.hour(); }
	inline minute_t minute(const Islamic& g) { return g.minute(); }
	inline second_t second(const Islamic& g) { return g.second(); }

	/*Islamic::Islamic& operator = (Jd const& jd) {
		from_jd(jd.jd());
		return *this;
	}*/


	/** Islamic + (integer year) operator. */
	Islamic operator + (Islamic const& dt, packaged_year_integer const& year) {
		year_t y = dt.year() + year.nYears_;
		month_t m = dt.month();
		day_t d = dt.day();

		if (m == February && d == 29 && !is_islamic_leapyear(y))
			d = 28;

		return Islamic(y, m, d);
	}



	///** Islamic + (integer month) operator. */
	//Islamic operator + (Islamic const& dt, packaged_month_integer const& month) {
	//	year_t yearsToAdd = month.nMonths_ / 12;
	//	month_t monthsToAdd = month.nMonths_ % 12;
	//	year_t y = dt.year() + yearsToAdd;
	//	month_t m = dt.month() + monthsToAdd;

	//	int adjustment = (m - 1) / 12 + (m - 12) / 12;
	//	y += adjustment;
	//	m -= month_t(adjustment * 12);

	//	day_t d = std::min(dt.day(), Islamic_days_in_month(m, is_Islamic_leapyear(y)));

	//	return Islamic(y, m, d);
	//}

}
