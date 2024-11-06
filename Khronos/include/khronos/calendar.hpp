#pragma once

/*
khronos\calendar.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'civil' declarations.
*/


#include <khronos/def.hpp>
#include <khronos/utility.hpp>
#include <array>
#include <cassert>

namespace khronos {

	namespace civil {

		namespace detail {
			constexpr std::array<std::array<day_t, 13>, 2> daysInMonth = {
				0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
				0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
			};
		}


		/** Get the full name of the civil day of week.
			@return pointer to the day name as a c-string.
			@param day [in] day number [0..6], where 0 = Monday
		*/
		constexpr char const* day_name(day_t day) {
			constexpr char const* names[7] = {
				"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
			};
			assert(day >= 0);
			assert(day < 7);
			return names[day];
		}

		constexpr day_t day_on_date(int d, int m, int y) {
			// Implementation of the day calculation logic
			if (m < 3) {
				m += 12;
				y--;
			}
			int K = (y < 0) ? -y % 100 : y % 100; // Year of the century
			int J = (y < 0) ? (-y / 100) - 1 : y / 100; // Zero-based century
			int h = (d + (13 * (m + 1)) / 5 + K + (K / 4) + (J / 4) - 2 * J) % 7;
			return (h + 5) % 7; // Adjust to Monday start
		}


		inline day_t day_of_week(jd_t jd) {
			return static_cast<day_t>(utility::mod(static_cast<int>(jd + 0.5) + 4, 7));
		}
		/**	Returns the number of days in the given month of the civil calendar.
			@return The number of days in the given month.
			@param month [in] month number [1..12]
			@param isLeapYear [in] 'true' for leap year, 'false' otherwise.
			*/
		constexpr day_t days_in_month(month_t month, bool isLeapYear) {
			return detail::daysInMonth[isLeapYear][month];
		}

		// month functions

		/** Get the full name of the civil month.
			@return pointer to the month name as a c-string.
			@param month [in] month number [1..12]
		*/
		constexpr char const* month_name_long(month_t month) {
			constexpr char const* const names[13] = {
				"",
				"January", "February", "March", "April", "May", "June",
				"July", "August", "September", "October", "November", "December"
			};

			assert(month > 0);
			assert(month < 13);
			return names[month];
		}



		/** Get the short name of the civil month.
			@return pointer to the 3-letter month name as a c-string.
			@param month [in] month number [1..12]
		*/
		constexpr char const* month_name_short(month_t month) {
			constexpr char const* const names[13] = {
				"",
				"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
				"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
			};

			assert(month > 0);
			assert(month < 13);
			return names[month];
		}
	}



	// VALUES
	enum civil_month_codes_long { January = 1, February, March, April, May, June, July, August, September, October, November, December };
	enum civil_month_codes_short { JAN = January, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };
	enum civil_weekday_codes_long { Monday = 0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };
	enum civil_weekday_codes_short { MON = Monday, TUE, WED, THU, FRI, SAT, SUN };
	enum civil_IslamicMonth { Muharram = 1, Safar, RabialAwwal, RabiathThani, JumadaAlAwwal, JumadatTania, Rajab, Shaban, Ramadan, Shawwal, DhulQadah, DhulHijja };
	enum civil_HebrewMonth{ Nisan = 1, Iyyar, Sivan, Tammuz, Av, Elul, Tishri, Heshvan, Kislev, Teveth, Shevat, Adar, Veadar};


	enum civil_VulcanMonth {
		Zat = 1, Druh,
		KriBrax,
		reTKhutai,
		TkeKhuti,
		Khuti,
		TaKrat,
		Krilior,
		etkhior,
		Tlakht,
		TkeTas,
		Tasmeen
	};

} // end-of-namespace khronos
