/*
khronos\gregorian_to_jdn.cpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'gregorian::to_jdn' implementation.
*/

#include <khronos/gregorian_calendar.hpp>
#include <khronos/julian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/calendar.hpp>


namespace khronos {


	jd_t gregorian_to_jd(year_t year, month_t month, day_t day) {
		assert(year > -4800 && "Minimum year is 4800 BCE");
		assert(month >= 1 && "Minimum month is January = 1");
		assert(month <= 12 && "Maximum month is December = 12");
		assert(day >= 1 && "Minimum day of the month is 1");
		assert(day <= 31 && "Maximum day of the month is 31");
		assert(greg2jd_integer(year, month, day) == greg2jd_real(year, month, day));
		return greg2jd_integer(year, month, day) + tod(0, 0, 0);
	}

	jd_t gregorian_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds) {
		assert(year > -4800 && "Minimum year is 4800 BCE");
		assert(month >= 1 && "Minimum month is January = 1");
		assert(month <= 12 && "Maximum month is December = 12");
		assert(day >= 1 && "Minimum day of the month is 1");
		assert(day <= 31 && "Maximum day of the month is 31");
		assert(greg2jd_integer(year, month, day) == greg2jd_real(year, month, day));
		return greg2jd_integer(year, month, day) + tod(hour, minute, seconds);
	}

	/*

		}
	}*/


	/** Print the Gregorian Date as a string. */




	///** Gregorian + (integer year) operator. */
	/*Gregorian operator + (Gregorian const& dt, packaged_year_integer const& year) {
		year_t y = dt.year() + year.nYears_;
		month_t m = dt.month();
		day_t d = dt.day();

		if (m == February && d == 29 && !is_gregorian_leapyear(y))
			d = 28;

		return Gregorian(y, m, d);
	}



	///** Gregorian + (integer month) operator. */
	/*Gregorian operator + (Gregorian const& dt, packaged_month_integer const& month) {
		year_t yearsToAdd = month.nMonths_ / 12;
		month_t monthsToAdd = month.nMonths_ % 12;
		year_t y = dt.year() + yearsToAdd;
		month_t m = dt.month() + monthsToAdd;

		int adjustment = (m - 1) / 12 + (m - 12) / 12;
		y += adjustment;
		m -= month_t(adjustment * 12);

		day_t d = std::min(dt.day(), gregorian_days_in_month(m, is_gregorian_leapyear(y)));

		return Gregorian(y, m, d);
	}*/
}

