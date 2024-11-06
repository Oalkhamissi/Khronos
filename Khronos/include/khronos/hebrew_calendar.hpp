#pragma once

/**	@file khronos/hebrew_calendar.hpp
	@author Garth Santor
	@date 2013-09-26

	Hebrew calendar class.
	*/

#include <cmath>
#include "def.hpp"
#include "julian_day.hpp"
#include <sstream>
#include <iomanip>
#include <string>

namespace khronos {
	constexpr jd_t HEBREW_EPOCH = 347995.5;
	//constexpr year_t operator"" _C(unsigned long long year) {
	//	return static_cast<year_t>(year);
	//}

	///* UDL - converts a Gregorian year BCE to an astronomical Gregorian year. */
	//constexpr year_t operator"" _BC(unsigned long long gregorianYearBCE) {
	//	return static_cast<year_t>(1 - gregorianYearBCE);  // Astronomical year (1 BCE = 0, 2 BCE = -1, etc.)
	//}
	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day);

	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds);

	jd_t hebrew_to_jd(year_t year, month_t month, day_t day);


	jd_t hebrew_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second);

	inline jd_t jdn(int year, int month, int day);
	inline bool is_hebrew_leapyear(year_t year) {
		if ((7 * year + 1) % 19 < 7) {
			return true;
		}
		else {
			return false;
		}
	}


	inline day_t delayOfWeek(year_t year) {
		month_t months = floor((235 * year - 234) / 19);
		day_t parts = 12084 + 13753 * months;
		day_t days = months * 29 + floor(parts / 25920);
		if ((3 * (days + 1)) % 7 < 3) {
			days = days + 1;
		}
		return days;
	}



	inline day_t daysInYear(year_t year) {
		return jdn(year, 7, 1) - jdn(year + 1, 7, 1);
	}

	inline day_t delayAdjacentYear(year_t year) {
		day_t last = delayOfWeek(year - 1);
		day_t present = delayOfWeek(year);
		day_t next = delayOfWeek(year + 1);
		day_t days = 0;
		if (next - present == 356) {
			days = 2;
		}
		else if ((next - present != 356) && (present - last == 382)) {
			days = 1;
		}
		return days;
	}

	inline int monthInYear(year_t year) {
		int months = 12;
		if (is_hebrew_leapyear(year)) {
			months++;
		}
		return months;
	}

	inline day_t daysInMonth(year_t year, month_t month) {
		day_t days;
		if (month == 2 || month == 4 || month == 6 || month == 10 || month == 13) {
			days = 29;
		}
		else if (month == 12 && is_hebrew_leapyear(year) != true) {
			days = 29;
		}
		else if (month == 8 && daysInYear(year) % 10 != 5) {
			days = 29;
		}
		else if (month == 9 && daysInYear(year) % 10 == 3) {
			days = 29;
		}
		else {
			days = 30;
		}
		return days;
	}

	inline jd_t jdn(int year, int month, int day) {
		// This function calculates the Julian Day Number for the given Hebrew date
		jd_t jdn = HEBREW_EPOCH + (year - 1) * 354 + (std::floor((3 + 11 * (year - 1)) / 30.0));
		for (int m = 1; m < month; ++m) {
			jdn += daysInMonth(year, m);
		}
		jdn += day; // Add the day
		return jdn;
	}

	inline jd_t hebrew_to_jd_integer(year_t year, month_t month, day_t day) {

		jd_t jdn = HEBREW_EPOCH + delayOfWeek(year) + delayAdjacentYear(year) + day + 1;
		if (month < 7) {
			for (int i = 7; i < monthInYear(year); i++) {
				jdn = jdn + daysInMonth(year, month);
				for (int j = 1; j < month - 1; j++) {
					jdn = jdn + daysInMonth(year, month);
				}
			}
		}
		else {
			for (int i = 7; i < month - 1; i++) {
				jdn = jdn + daysInMonth(year, month);
			}
		}
		return jdn;
	}




	inline std::string hebrew_month_name(int i) {
		switch (i) {
		case Nisan: return "Nisan";
		case Iyyar: return "Iyyar";
		case Sivan: return "Sivan";
		case Tammuz: return "Tammuz";
		case Av: return "Av";
		case Elul: return "Elul";
		case Tishri: return "Tishri";
		case Heshvan: return "Heshvan";
		case Kislev: return "Kislev";
		case Teveth: return "Teveth";
		case Shevat: return "Shevat";
		case Adar: return "Adar";
		case Veadar: return "Veadar";
		default: return "Invalid Input";
		}
	}
	class Gregorian;
	class Hebrew {
		year_t		year_;
		month_t		month_;
		day_t		day_;
		hour_t		hour_;
		minute_t	minute_;
		second_t	second_;

	public:
		Hebrew();
		Hebrew(now_t NOTIMEOFDAY);
		Hebrew(Jd j);

		void from_jd(jd_t jd) { jd_to_hebrew(jd, year_, month_, day_, hour_, minute_, second_); }
		jd_t to_jd() const { return hebrew_to_jd(year_, month_, day_, hour_, minute_, second_); }
		/*! Get the year.
			\return Astronomical year. */
		constexpr year_t  year() const { return year_; }


		/*! Get the month.
			\return Month number [1..12] */
		constexpr month_t month() const { return month_; }


		/*! Get the day of the month.
			\return Day of month number [1..31]. */
		constexpr day_t day() const { return day_; }

		constexpr hour_t hour() const { return hour_; }

		constexpr minute_t minute() const { return minute_; }

		constexpr second_t second() const { return second_; }

		/*! Construct a Gregorian calendar date from year,month,day,[hour,minute,second]
			\param year [in] Astronomical year.
			\param month [in] Month number [1..12]
			\param day [in] Day of month [1..31]
			\param hour [in] 24-hour of the day [0..23]
			\param minute [in] minute of the hour [0..59]
			\param second [in] second of the minute [0..59]
			*/
		Hebrew(year_t year, month_t month, day_t day) : year_(year), month_(month), day_(day), hour_(0), minute_(0), second_(0) {}

		Hebrew(year_t year, month_t month, day_t day, hour_t hour, minute_t min, second_t sec) : year_(year), month_(month), day_(day), hour_(hour), minute_(min), second_(sec) {}

		bool operator==(const Hebrew& other) const
		{
			if (year_ != other.year_)
				return false;

			if (month_ != other.month_)
				return false;

			if (day_ != other.day_)
				return false;

			if (hour_ != other.hour_)
				return false;

			if (minute_ != other.minute_)
				return false;

			if (second_ != other.second_)
				return false;

			return true;
		}
		Hebrew& operator = (Jd const& jd) {
			from_jd(jd.jd());
			return *this;
		}

		bool isHebrewLeapYear(int year) {
			// Leap year calculation: mod(7 * year + 1, 19) < 7
			return (7 * year + 1) % 19 < 7;
		}

		int daysInYear(int year) {
			// Calculate the number of days in the Hebrew year
			return 354 + isHebrewLeapYear(year);  // 354 days in a common year + 1 day for leap year
		}

		int  hebrew_days_in_month(int year, int month) {
			// Determine the number of days in a month of the Hebrew calendar
			if (month == 2 || month == 4 || month == 6 || month == 10 || month == 13) {  // Months with 29 days
				return 29;
			}
			if (month == 12 && !isHebrewLeapYear(year)) {  // Month 12 has 29 days if not a leap year
				return 29;
			}
			if (month == 8 && (daysInYear(year) % 10) != 5) {  // Month 8 has 29 days if days in year mod 10 != 5
				return 29;
			}
			if (month == 9 && (daysInYear(year) % 10) == 3) {  // Month 9 has 29 days if days in year mod 10 == 3
				return 29;
			}
			return 30;  // All other months have 30 days
		}

		


		int hebrew_months_in_year(int year) {
			// Returns the number of months in a Hebrew year
			int months = 12;  // Default to 12 months
			if (isHebrewLeapYear(year)) {
				months += 1;  // Add an extra month for leap years
			}
			return months;
		}


		Hebrew operator + (khronos::packaged_day const& input)
		{
			Hebrew res = *this;


			res.day_ += input.nDays_;
			while ((res.day_ / hebrew_days_in_month(res.month(), is_hebrew_leapyear(res.year_))) >= 1) {
				res.day_ -= hebrew_days_in_month(res.month(), is_hebrew_leapyear(res.year_));
				if (res.month_ == December) {
					res.month_ = January;
					res.year_ = res.year_ + 1;
				}
				else {
					res.month_ = res.month_ + 1;
				}
				std::cout << "Mon" << res.month_ << std::endl;
			}

			return res;
		}

		Hebrew operator - (khronos::packaged_day const& input)
		{
			Hebrew res = *this;
			res.day_ -= input.nDays_;
			while (res.day_ < 1) {
				int days_in_prev_month = hebrew_days_in_month(res.month_, is_hebrew_leapyear(res.year_));
				if (res.month_ == January) {
					res.month_ = December;
					res.year_ -= 1;
				}
				else {
					res.month_ = static_cast<month_t>(res.month_ - 1);
				}
				res.day_ += days_in_prev_month;
			}
			return res;
		}

		Hebrew operator + (khronos::packaged_week const& input) {
			Hebrew res = *this;
			day_t days = input.nWeeks_ * 7;
			res = res + packaged_day(input.nWeeks_ * 7);
			return res;
		}

		Hebrew operator - (khronos::packaged_week const& input) {
			Hebrew res = *this;
			day_t days = input.nWeeks_ * 7;
			std::cout << "No :" << days << std::endl;
			res = res - packaged_day(days);
			return res;
		}
		Hebrew operator + (khronos::packaged_month_integer const& input) {
			Hebrew res = *this;
			res.month_ += input.nMonths_;
			while (res.month_ > 12) {

				res.year_ = res.year_ + 1;
				res.month_ -= 12;

			}
			if (res.day_ > hebrew_days_in_month(res.month_, is_hebrew_leapyear(res.year_))) {
				res.day_ = hebrew_days_in_month(res.month_, is_hebrew_leapyear(res.year_));
			}
			return res;
		}
		Hebrew operator - (khronos::packaged_month_integer const& input) {
			Hebrew res = *this;
			res.month_ -= input.nMonths_;
			while (res.month_ < 1) {
				res.year_ = res.year_ - 1;
				res.month_ += 12;
			}
			return res;
		}
		Hebrew operator + (khronos::packaged_year_integer const& year) {
			Hebrew res = *this;
			res.year_ += year.nYears_;
			if (res.month_ == February && (res.day_ == 29)) {
				if (!is_hebrew_leapyear(res.year_)) {
					res.day_ = 28;
				}
			}
			return res;
		}
		Hebrew operator - (khronos::packaged_year_integer const& year) {
			Hebrew res = *this;
			res.year_ -= year.nYears_;
			if (res.month_ == February && (res.day_ == 29)) {
				if (!is_hebrew_leapyear(this->year_)) {
					res.day_ = 28;
				}
			}
			return res;
		}

		bool operator < (const Hebrew& other) const
		{
			if (year_ < other.year_)
				return true;

			if (month_ < other.month_)
				return true;

			if (day_ < other.day_)
				return true;

			if (hour_ < other.hour_)
				return true;

			if (minute_ < other.minute_)
				return true;

			if (second_ < other.second_)
				return true;

			return false;
		}

		bool operator <= (const Hebrew& other) const
		{
			if (year_ <= other.year_)
				return true;

			if (month_ <= other.month_)
				return true;

			if (day_ <= other.day_)
				return true;

			if (hour_ <= other.hour_)
				return true;

			if (minute_ <= other.minute_)
				return true;

			if (second_ <= other.second_)
				return true;

			return false;
		}

		bool operator >= (const Hebrew& other) const
		{
			if (year_ >= other.year_)
				return true;

			if (month_ >= other.month_)
				return true;

			if (day_ >= other.day_)
				return true;

			if (hour_ >= other.hour_)
				return true;

			if (minute_ >= other.minute_)
				return true;

			if (second_ >= other.second_)
				return true;

			return false;
		}

		bool operator > (const Hebrew& other) const
		{
			if (year_ > other.year_)
				return true;

			if (month_ > other.month_)
				return true;

			if (day_ > other.day_)
				return true;

			if (hour_ > other.hour_)
				return true;

			if (minute_ > other.minute_)
				return true;

			if (second_ > other.second_)
				return true;

			return false;
		}


		inline jd_t operator-(const Hebrew& other) {
			return this->to_jd() - other.to_jd();
		}
		inline jd_t operator-(const Jd& other) {
			return this->to_jd() - other.jd();
		}


		std::string to_string() const {
			std::ostringstream oss;

			day_t day_of_week = khronos::civil::day_of_week(this->to_jd());
			std::string day_name_str = khronos::civil::day_name(day_of_week);
			std::string month_name = hebrew_month_name(month_);

			oss << day_name_str << ", " << month_name << " " << day_ << " ";

			if (year_ > 0) {
				oss << year_ << " AD";
			}
			else {
				oss << (-year_ + 1) << " BC"; // Correctly convert to BCE
			}

			oss << ", " << std::setw(2) << std::setfill('0') << (hour_ % 12 == 0 ? 12 : hour_ % 12)
				<< ":" << std::setw(2) << std::setfill('0') << minute_
				<< ":" << std::setw(2) << std::setfill('0') << second_
				<< (hour_ >= 12 ? " pm" : " am");

			return oss.str();
		}

		friend std::ostream& operator<<(std::ostream& os, const Hebrew& g) {
			os << g.to_string();
			return os;
		}

		/*bool operator==(const Gregorian& other)
		{
			if (year_ != other.year())
				return false;

			if (month_ != other.month())
				return false;

			if (day_ != other.day())
				return false;

			if (hour_ != other.hour())
				return false;

			if (minute_ != other.minute())
				return false;

			if (second_ != other.second())
				return false;

			return true;
		}*/

	};


	/*inline year_t year(const Hebrew& g) { return g.year(); }
	inline month_t month(const Hebrew& g) { return g.month(); }
	inline day_t day(const Hebrew& g) { return g.day(); }
	inline hour_t hour(const Hebrew& g) { return g.hour(); }
	inline minute_t minute(const Hebrew& g) { return g.minute(); }
	inline second_t second(const Hebrew& g) { return g.second(); }*/


} // namespace khronos
