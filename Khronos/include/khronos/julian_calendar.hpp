#pragma once

/**	@file khronos/julian_calendar.hpp
	@author Garth Santor
	@date 2013-09-19

	Julian calendar functions and class declaration.
	*/


#include "julian_calendar.hpp"
#include <cmath>
#include "def.hpp"
#include "julian_day.hpp"
#include <sstream>
#include <iomanip>
#include <string>

namespace khronos {
	constexpr year_t operator"" _C(unsigned long long year) {
		return static_cast<year_t>(year);
	}

	/* UDL - converts a Gregorian year BCE to an astronomical Gregorian year. */
	constexpr year_t operator"" _BC(unsigned long long gregorianYearBCE) {
		return static_cast<year_t>(1 - gregorianYearBCE);  // Astronomical year (1 BCE = 0, 2 BCE = -1, etc.)
	}


	class Gregorian;

	class Julian {
		year_t		year_;
		month_t		month_;
		day_t		day_;
		hour_t		hour_;
		minute_t	minute_;
		second_t	second_;

	public:
		Julian();
		Julian(now_t NOTIMEOFDAY);
		Julian(Jd j);

		void from_jd(jd_t jd) { jd_to_julian(jd, year_, month_, day_, hour_, minute_, second_); }
		jd_t to_jd() const { return julian_to_jd(year_, month_, day_, hour_, minute_, second_); }
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
		Julian(year_t year, month_t month, day_t day) : year_(year), month_(month), day_(day), hour_(0), minute_(0), second_(0) {}

		Julian(year_t year, month_t month, day_t day, hour_t hour, minute_t min, second_t sec) : year_(year), month_(month), day_(day), hour_(hour), minute_(min), second_(sec) {}

		bool operator==(const Julian& other) const
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
		Julian& operator = (Jd const& jd) {
			from_jd(jd.jd());
			return *this;
		}


		Julian operator + (khronos::packaged_day const& input)
		{
			Julian res = *this;


			res.day_ += input.nDays_;
			while ((res.day_ / julian_days_in_month(res.month(), is_julian_leapyear(res.year_))) >= 1) {
				res.day_ -= julian_days_in_month(res.month(), is_julian_leapyear(res.year_));
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

		Julian operator - (khronos::packaged_day const& input)
		{
			Julian res = *this;
			res.day_ -= input.nDays_;
			while (res.day_ < 1) {
				int days_in_prev_month = julian_days_in_month(res.month_, is_julian_leapyear(res.year_));
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

		Julian operator + (khronos::packaged_week const& input) {
			Julian res = *this;
			day_t days = input.nWeeks_ * 7;
			res = res + packaged_day(input.nWeeks_ * 7);
			return res;
		}

		Julian operator - (khronos::packaged_week const& input) {
			Julian res = *this;
			day_t days = input.nWeeks_ * 7;
			std::cout << "No :" << days << std::endl;
			res = res - packaged_day(days);
			return res;
		}
		Julian operator + (khronos::packaged_month_integer const& input) {
			Julian res = *this;
			res.month_ += input.nMonths_;
			while (res.month_ > 12) {

				res.year_ = res.year_ + 1;
				res.month_ -= 12;

			}
			if (res.day_ > julian_days_in_month(res.month_, is_julian_leapyear(res.year_))) {
				res.day_ = julian_days_in_month(res.month_, is_julian_leapyear(res.year_));
			}
			return res;
		}
		Julian operator - (khronos::packaged_month_integer const& input) {
			Julian res = *this;
			res.month_ -= input.nMonths_;
			while (res.month_ < 1) {
				res.year_ = res.year_ - 1;
				res.month_ += 12;
			}
			return res;
		}
		Julian operator + (khronos::packaged_year_integer const& year) {
			Julian res = *this;
			res.year_ += year.nYears_;
			if (res.month_ == February && (res.day_ == 29)) {
				if (!is_julian_leapyear(res.year_)) {
					res.day_ = 28;
				}
			}
			return res;
		}
		Julian operator - (khronos::packaged_year_integer const& year) {
			Julian res = *this;
			res.year_ -= year.nYears_;
			if (res.month_ == February && (res.day_ == 29)) {
				if (!is_julian_leapyear(this->year_)) {
					res.day_ = 28;
				}
			}
			return res;
		}

		bool operator < (const Julian& other) const
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

		bool operator <= (const Julian& other) const
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

		bool operator >= (const Julian& other) const
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

		bool operator > (const Julian& other) const
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
		
		
		inline jd_t operator-(const Julian& other) {
			return this->to_jd() - other.to_jd();
		}
		inline jd_t operator-(const Jd& other) {
			return this->to_jd() - other.jd();
		}


		std::string to_string() const {
			std::ostringstream oss;

			day_t day_of_week = khronos::civil::day_of_week(this->to_jd());
			std::string day_name_str = khronos::civil::day_name(day_of_week);
			std::string month_name = julian_month_name(month_);

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

		friend std::ostream& operator<<(std::ostream& os, const Julian& g) {
			os << g.to_string();
			return os;
		}

	};


	inline year_t year(const Julian& g) { return g.year(); }
	inline month_t month(const Julian& g) { return g.month(); }
	inline day_t day(const Julian& g) { return g.day(); }
	inline hour_t hour(const Julian& g) { return g.hour(); }
	inline minute_t minute(const Julian& g) { return g.minute(); }
	inline second_t second(const Julian& g) { return g.second(); }


} // namespace khronos
