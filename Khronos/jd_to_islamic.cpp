/**	@file Islamic_to_jd.cpp
    @author Garth Santor
    @date 2013-09-19

    Islamic calendar conversion implementations.
    */

#include <khronos/islamic_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/calendar.hpp>
#include <cmath>
#include <algorithm>
namespace khronos {
    void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day) {
        assert(jd >= -31738.5 && "Minimum convertible date not provided.");
        using namespace utility;

        jd = std::ceil(jd) + 0.5;  // Normalize JD to the nearest noon.

        // Calculate the Islamic year based on JD and epoch.
        year = static_cast<year_t>(std::floor((30 * (jd - ISLAMIC_EPOCH) + 10646) / 10631));

        // Determine the JD at the start of the calculated year.
        jd_t jd1m1 = ISLAMIC_EPOCH + (year - 1) * 354 + std::ceil((3 + 11 * year) / 30.0);

        // Month calculation - use floor to ensure correct rounding boundaries.
        //month = static_cast<month_t>(std::min(12.0, std::floor((jd - jd1m1) / 29.5) + 1));
        if ((std::floor((jd - jd1m1) / 29.5) + 1) < 12){
            month = (std::floor ((jd - jd1m1) / 29.5) + 1);
        }
        else {
            month = 12;
        }
        // Calculate the start of the month JD based on month and year.
        jd_t jdm = jd1m1 + std::ceil(29.5 * (month - 1));

        // Determine the day in the month.
        day = static_cast<day_t>(jd - jdm + 1);
    }
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds) {
		jd_to_islamic(jd, year, month, day);
		jd_to_hms(jd, hour, minute, seconds);
	}

    //void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds) {
    //    // Adjust JD to the nearest noon
    //    jd = std::floor(jd) + 0.5;

    //    // Calculate Islamic year
    //    year = static_cast<year_t>(std::floor((30 * (jd - ISLAMIC_EPOCH) + 10646) / 10631));

    //    // Calculate JD of the beginning of the year
    //    jd_t jd1m1 = ISLAMIC_EPOCH + (year - 1) * 354 + std::floor((3 + 11 * year) / 30.0);

    //    // Calculate the month
    //    month = static_cast<month_t>(std::min(12.0, std::floor((jd - jd1m1) / 29.5) + 1));

    //    // Calculate JD of the start of the month
    //    jd_t jdm = jd1m1 + std::ceil(29.5 * (month - 1));

    //    // Calculate the day in the month
    //    day = static_cast<day_t>(jd - jdm + 1);

    //    // Extract hours, minutes, and seconds from JD fraction
    //    jd_to_hms(jd, hour, minute, seconds);
    //}
}