/**	@file Jdn.cpp
	@author Garth Santor
	@date 2012-09-16

	Date and time library to_string method implementations.
	*/

#include <khronos/julian_day.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include <ctime>
#include <khronos/gregorian_calendar.hpp>
using namespace std;

namespace khronos {

	/**
	 * Jd default constructor.
	 * Initialize the Julian Day (JD) to the current local time.
	 */
	Jd::Jd()
		: jd_(0)
	{
		// Get the current time
		time_t nowTime = time(NULL);
		struct tm tmNow;
		localtime_s(&tmNow, &nowTime);

		// Convert the current Gregorian date and time to Julian Day
		jd_ = gregorian_to_jd(
			year_t(tmNow.tm_year) + 1900,  // Adjust for the tm_year offset
			month_t(tmNow.tm_mon) + 1,     // Adjust for zero-indexed months
			day_t(tmNow.tm_mday),          // Day of the month
			hour_t(tmNow.tm_hour),         // Hour of the day
			minute_t(tmNow.tm_min),        // Minute of the hour
			second_t(tmNow.tm_sec)         // Second of the minute
		);
	}

	/**
	 * Jd constructor from a Gregorian calendar object.
	 * Converts a given Gregorian date to Julian Day (JD).
	 */
	Jd::Jd(Gregorian g)
	{
		// Convert the given Gregorian date to Julian Day
		jd_ = gregorian_to_jd(
			year_t(g.year()) + 1900,      // Adjust for Gregorian year
			month_t(g.month()) + 1,       // Adjust for zero-indexed months
			day_t(g.day()),               // Day of the month
			hour_t(g.hour()),             // Hour of the day
			minute_t(g.minute()),         // Minute of the hour
			second_t(g.second())          // Second of the minute
		);
	}

	/**
	 * Jd constructor with a now_t option.
	 * Depending on the timeOption, initializes JD to the current date or date and time.
	 */
	Jd::Jd(now_t timeOption)
		: jd_(0)
	{
		if (timeOption == 1) {
			// If timeOption is 1, include the current time
			time_t nowTime = time(NULL);
			struct tm tmNow;
			localtime_s(&tmNow, &nowTime);

			// Convert the current Gregorian date and time to Julian Day
			jd_ = gregorian_to_jd(
				year_t(tmNow.tm_year) + 1900,  // Adjust for the tm_year offset
				month_t(tmNow.tm_mon) + 1,     // Adjust for zero-indexed months
				day_t(tmNow.tm_mday),          // Day of the month
				hour_t(tmNow.tm_hour),         // Hour of the day
				minute_t(tmNow.tm_min),        // Minute of the hour
				second_t(tmNow.tm_sec)         // Second of the minute
			);
		}
		else if (timeOption == 0) {
			// If timeOption is 0, only include the current date (no time)
			time_t nowTime = time(NULL);
			struct tm tmNow;
			localtime_s(&tmNow, &nowTime);

			// Convert the current Gregorian date to Julian Day without time
			jd_ = gregorian_to_jd(
				year_t(tmNow.tm_year) + 1900,  // Adjust for the tm_year offset
				month_t(tmNow.tm_mon) + 1,     // Adjust for zero-indexed months
				day_t(tmNow.tm_mday)           // Day of the month
			);
		}
	}

	/**
	 * Convert the Julian Day (JD) to a string representation.
	 * Returns the JD as a formatted string.
	 */
	string Jd::to_string() const {
		ostringstream oss;
		oss << "JD " << jd_;  // Output the Julian Day with the prefix "JD"
		return oss.str();
	}

}  // namespace khronos
