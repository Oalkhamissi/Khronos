#pragma once
/*  julian_day.hpp

    Khronos library Julian Day class declaration.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.09.25
    Replaced relational operators with <=>

Version 2016.09.30
    Added: Phase List

Version 2015.09.21
    Initial Release


=============================================================

Copyright Garth Santor/Trinh Han

The copyright to the computer program(s) herein
is the property of Garth Santor/Trinh Han, Canada.
The program(s) may be used and/or copied only with
the written permission of Garth Santor/Trinh Han
or in accordance with the terms and conditions
stipulated in the agreement/contract under which
the program(s) have been supplied.
=============================================================*/

#include <cmath>
#include <string>
#include <ostream>
#include <khronos/def.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/utility.hpp>
#include <khronos/calendar.hpp>
#include <iostream>

namespace khronos {
    const jd_t JULIAN_EPOCH = 1721423.5;
    class Gregorian;

    class Jd {

        jd_t jd_;
    public:
        Jd();
        Jd(Gregorian g);
        Jd(now_t timeOption);
        /**	Constructor from real number. */
        constexpr Jd(jd_t jd) : jd_(jd) {  }

        /**	Get the number of days. */
        constexpr jd_t jd() const { return jd_; }

        std::string to_string() const;
    };

    /** stream insertion operator. */
    // Overload the output operator to allow printing the Julian Day as a string.
    inline std::ostream& operator << (std::ostream& os, Jd const& jd) {
        return os << jd.to_string();
    }

    // Check if a given year is a leap year in the Julian calendar.
    inline bool is_julian_leapyear(year_t year) {
        return year % 4 == 0;
    }

    // Return the number of days in a given month, accounting for leap years.
    inline int julian_days_in_month(month_t month, bool leapyear) {
        return khronos::civil::days_in_month(month, leapyear);
    }

    // Return the full name of a month given its numeric representation.
    inline std::string julian_month_name(int i) {
        switch (i) {
        case January: return "January";
        case February: return "February";
        case March: return "March";
        case April: return "April";
        case May: return "May";
        case June: return "June";
        case July: return "July";
        case August: return "August";
        case September: return "September";
        case October: return "October";
        case November: return "November";
        case December: return "December";
        default: return "Invalid Input";
        }
    }

    // Return the abbreviated name of a month given its numeric representation.
    inline std::string julian_short_month_name(int i) {
        switch (i) {
        case JAN: return "JAN";
        case FEB: return "FEB";
        case MAR: return "MAR";
        case APR: return "APR";
        case MAY: return "MAY";
        case JUN: return "JUN";
        case JUL: return "JUL";
        case AUG: return "AUG";
        case SEP: return "SEP";
        case OCT: return "OCT";
        case NOV: return "NOV";
        case DEC: return "DEC";
        default: return "Invalid Input";
        }
    }

    // Convert a Julian calendar date to Julian Day number (JD).
    inline jd_t julian_to_jd(year_t year, month_t month, day_t day, hour_t hour = 0, minute_t minute = 0, second_t second = 0) {
        // Adjust for month and year
        int a = (14 - month) / 12;
        int y = year + 4800 - a;
        int m = month + (12 * a) - 3;

        // Calculate the Julian Day Number
        jd_t jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083.5;
        tod_t tod1 = tod(hour, minute, second);

        // Add 0.5 for the time of day (to get the JD)
        return jdn + tod1;
    }

    // Convert a Julian Day number (JD) to a Julian calendar date (year, month, day).
    inline void jd_to_julian(jd_t jdn, year_t& year, month_t& month, day_t& day) {
        int a = floor(jdn + 0.5);
        int b = a + 1524;
        int c = floor((b - 122.1) / 365.25);
        int d = floor(365.25 * c);
        int e = floor((b - d) / 30.6001);
        if (e < 14) {
            month = floor(e - 1);
        }
        else {
            month = floor(e - 13);
        }
        if (month > 2) {
            year = c - 4716;
        }
        else {
            year = c - 4715;
        }
        day = b - d - floor(30.6001 * e);

    }

    // Convert a Julian Day number (JD) to a Julian calendar date (year, month, day) and time (hour, minute, second).
    inline void jd_to_julian(jd_t jdn, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& second) {
        jd_to_julian(jdn, year, month, day);
        jd_to_hms(jdn, hour, minute, second);
    }

    // Round a Julian Day number (JD) to the nearest Julian Day Number (JDN).
    inline jd_t jd_to_jdn(jd_t jd) {
        return floor(jd + 0.5);
    }

    // RELATIONAL OPERATORS
    // --------------------------------------------------------------------------------------

    /** Jd relational operators. */
    // Compare two Julian Day objects using the three-way comparison operator (<=>).
    inline auto operator <=> (Jd const& lhs, Jd const& rhs) { return lhs.jd() <=> rhs.jd(); }
    inline bool operator == (Jd const& lhs, Jd const& rhs) { return lhs.jd() == rhs.jd(); }

    /**Jd difference operator. */
    // Subtract the Julian Day of one Jd object from another.
    inline jd_t operator - (Jd const& lhs, Jd const& rhs) { return lhs.jd() - rhs.jd(); }

    // Day of the week calculation based on Julian Day.
    inline day_t day_of_week(Jd const& jd) { return civil::day_of_week(jd.jd()); }

    // ADDITION/SUBTRACTION OPERATORS
    // --------------------------------------------------------------------------------------

    /** Day addition/subtraction. */
    // Structure for handling the addition/subtraction of days.
    struct packaged_day {
        day_t nDays_;
        packaged_day(day_t d) : nDays_(d) { };
    };

    // Function to wrap a number of days for addition/subtraction.
    inline packaged_day days(day_t d) { return packaged_day(d); }

    // Overload the addition operator to add days to a Julian Day.
    inline Jd operator + (Jd const& jdn, packaged_day days) { return Jd(jdn.jd() + days.nDays_); }

    // Overload the subtraction operator to subtract days from a Julian Day.
    inline Jd operator - (Jd const& jdn, packaged_day days) { return jdn.jd() - days.nDays_; }

    /** Week addition/subtraction. */
    // Structure for handling the addition/subtraction of weeks.
    struct packaged_week {
        day_t nWeeks_;
        inline packaged_week(day_t w) : nWeeks_(w) { };
    };

    // Function to wrap a number of weeks for addition/subtraction.
    inline packaged_week weeks(day_t w) { return packaged_week(w); }

    // Overload the addition operator to add weeks to a Julian Day.
    inline Jd operator + (Jd const& jdn, packaged_week weeks) { return jdn.jd() + weeks.nWeeks_ * 7.0; }

    // Overload the subtraction operator to subtract weeks from a Julian Day.
    inline Jd operator - (Jd const& jdn, packaged_week weeks) { return jdn.jd() - weeks.nWeeks_ * 7.0; }

    /** Month addition/subtraction. */
    // Structure for handling the addition/subtraction of months.
    struct packaged_month_integer {
        month_t nMonths_;
        inline packaged_month_integer(month_t m) : nMonths_(m) { }
    };
    struct packaged_month_real {
        double nMonths_;
        inline packaged_month_real(double m) : nMonths_(m) { }
    };

    // Functions to wrap a number of months for addition/subtraction.
    inline packaged_month_real months(double m) { return packaged_month_real(m); }
    inline packaged_month_real months(float m) { return packaged_month_real(m); }
    template <typename T> inline packaged_month_integer months(T m) { return packaged_month_integer(month_t(m)); }

    /** Year addition/subtraction. */
    // Structure for handling the addition/subtraction of years (integer).
    struct packaged_year_integer {
        year_t nYears_;
        inline packaged_year_integer(year_t y) : nYears_(y) { }
    };

    // Structure for handling the addition/subtraction of years (real).
    struct packaged_year_real {
        double nYears_;
        inline packaged_year_real(double y) : nYears_(y) { }
    };

    // Functions to wrap a number of years for addition/subtraction.
    inline packaged_year_real years(double y) { return packaged_year_real(y); }
    inline packaged_year_real years(float y) { return packaged_year_real(y); }
    template<typename T> inline packaged_year_integer years(T y) { return packaged_year_integer(static_cast<year_t>(y)); }

    // Overload the addition operator to add years (real) to a Julian Day.
    inline Jd operator + (Jd const& dt, packaged_year_real const& year) {
        return Jd(dt.jd() + year.nYears_ * EARTH_ORBITAL_PERIOD_DAYS);
    }

    // Overload the subtraction operator to subtract years (real) from a Julian Day.
    inline Jd operator - (Jd const& dt, packaged_year_real const& year) { return dt + packaged_year_real(-year.nYears_); }

    // Overload the addition operator to add years (integer) to a Julian Day.
    inline Jd operator + (Jd const& dt, packaged_year_integer const& year) { return dt + packaged_year_real((double)year.nYears_); }

    // Overload the subtraction operator to subtract years (integer) from a Julian Day.
    inline Jd operator - (Jd const& dt, packaged_year_integer const& year) { return dt + packaged_year_integer(-year.nYears_); }

} // end-of-namespace khronos
