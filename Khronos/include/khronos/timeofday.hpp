#pragma once

/*
khronos\timeofday.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'time-of-day' declarations.
*/


#include <khronos/def.hpp>


namespace khronos {

    // LITERALS

    /* UDL - converts an 'AM' hour to 'military' hour. */
    constexpr int operator"" _am(unsigned long long hour) {
        return (hour == 12) ? 0 : hour; // 12 AM is 0 in military time
    }

    /* UDL - converts a 'PM' hour to 'military' hour. */
    constexpr int operator"" _pm(unsigned long long hour) {
        return (hour == 12) ? 12 : hour + 12; // 12 PM is 12 in military time
    }
        
    /* Seconds per day. */
    constexpr second_t SECONDS_PER_DAY = 86400.0;



    /* H:M:S.S to time-of-day [0.0..1.0) */
    inline tod_t tod(hour_t hours, minute_t minutes, second_t seconds) {
        return (hours * 3600 + minutes * 60 + seconds) / SECONDS_PER_DAY;
    }

    /** Convert Time-of-day (TOD) to hours, minutes, seconds.
        Civil days run from midnight to midnight.
        @param tod [in] a time-of-day [0..1)
        @param hours [out] 0-23
        @param minutes [out] 0-59
        @param seconds [out] [0.0..60.0)
    */
    inline void tod_to_hms(tod_t tod, hour_t& hours, minute_t& minutes, second_t& seconds) {

        double tod_seconds = tod * SECONDS_PER_DAY;

        hours = static_cast<int>(tod_seconds / 3600);

        tod_seconds = fmod(tod_seconds, 3600);

        minutes = static_cast<int>(tod_seconds / 60);

        seconds = std::round(fmod(tod_seconds, 60) * 1000.0) / 1000.0; 
    }
    
    inline tod_t tod_jd(jd_t jd) {
        return (jd + 0.5) - floor(jd + 0.5);
    }

    inline void jd_to_hms(jd_t jd, hour_t& hour, minute_t& minute, second_t& second) {

        tod_t tod = tod_jd(jd);

        tod_to_hms(tod, hour, minute, second);

    }
} // end-of-namespace khronos
