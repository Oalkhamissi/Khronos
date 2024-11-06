/*
khronos\jdn_to_gregorian.cpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'gregorian::from_jdn' implementation.
*/

#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>

namespace khronos {
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day) {
		assert(jd >= -31738.5 && "Minimum convertible date not provided.");
		using namespace utility;

		double wjd, depoch, quadricent, dqc, cent, dcent, quad, dquad, yindex, yearday, leapadj;
		wjd = floor(jd - 0.5) + 0.5;
		depoch = wjd - GREGORIAN_EPOCH;
		quadricent = floor(depoch / 146097.0);
		dqc = mod(depoch, 146097.0);
		cent = floor(dqc / 36524.0);
		dcent = mod(dqc, 36524);
		quad = floor(dcent / 1461.0);
		dquad = mod(dcent, 1461.0);
		yindex = floor(dquad / 365.0);
		year = (year_t)floor((quadricent * 400.0) + (cent * 100.0) + (quad * 4.0) + yindex);
		if (!((cent == 4.0) || (yindex == 4.0)))
			++year;

		yearday = wjd - gregorian_to_jd(year, 1, 1);
		leapadj = ((wjd < gregorian_to_jd(year, 3, 1)) ? 0 : (is_gregorian_leapyear(year) ? 1 : 2));
		month = static_cast<month_t>(floor((((yearday + leapadj) * 12) + 373) / 367));
		day = static_cast<day_t>((wjd - gregorian_to_jd(year, month, 1)) + 1);
	}

	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds) {
		jd_to_gregorian(jd, year, month, day);
		jd_to_hms(jd, hour, minute, seconds);
	}


}