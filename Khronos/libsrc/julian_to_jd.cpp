/**	@file Julian_to_jd.cpp
	@author Garth Santor
	@date 2012-09-14

	Date and time library Julian conversion implementations.
	*/

#include <khronos/julian_calendar.hpp>
//#include <timeofday.hpp>
namespace khronos {
    //jd_t julian_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second) {
    //    // Adjust for month and year
    //    int a = (14 - month) / 12;
    //    int y = year + 4800 - a;
    //    int m = month + (12 * a) - 3;

    //    // Calculate the Julian Day Number
    //    jd_t jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083.5;
    //    tod_t tod1 = tod(hour, minute, second);

    //    // Add 0.5 for the time of day (to get the JD)
    //    return jdn + tod1;
    //}

    //inline void jd_to_julian(jd_t jdn, year_t& year, month_t& month, day_t& day) {
    //    int a = floor(jdn + 0.5);
    //    int b = a + 1524;
    //    int c = floor((b - 122.1) / 365.25);
    //    int d = floor(365.25 * c);
    //    int e = floor((b - d) / 30.6001);
    //    if (e < 14) {
    //        month = floor(e - 1);
    //    }
    //    else {
    //        month = floor(e - 13);
    //    }
    //    if (month > 2) {
    //        year = c - 4716;
    //    }
    //    else {
    //        year = c - 4715;
    //    }
    //    day = b - d - floor(30.6001 * e);

    //}

    //inline void jd_to_julian(jd_t jdn, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& second) {
    //    jd_to_julian(jdn, year, month, day);
    //    jd_to_hms(jdn, hour, minute, second);
    //}

}
