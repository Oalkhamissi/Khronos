#pragma once

/**	@file khronos/Islamic_calendar.hpp
	@author Garth Santor
	@date 2013-09-19

	Islamic calendar class and functions.
	*/

#include <cmath>
#include "def.hpp"
#include "julian_day.hpp"
#include <sstream>
#include <iomanip>
#include <string>

namespace khronos {
	constexpr jd_t ISLAMIC_EPOCH = 1948439.5;
	//constexpr year_t operator"" _C(unsigned long long year) {
	//	return static_cast<year_t>(year);
	//}

	///* UDL - converts a Gregorian year BCE to an astronomical Gregorian year. */
	//constexpr year_t operator"" _BC(unsigned long long gregorianYearBCE) {
	//	return static_cast<year_t>(1 - gregorianYearBCE);  // Astronomical year (1 BCE = 0, 2 BCE = -1, etc.)
	//}
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day);

	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds);

	jd_t islamic_to_jd(year_t year, month_t month, day_t day);


	jd_t islamic_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second);


	inline jd_t islamic_to_jd_integer(year_t year, month_t month, day_t day) {
		/*while (year < 1) {
			year += 1;
		}
		jd_t jd= day + ceil(29.5 * (month - 1)) + (year - 1) * 354 + floor((3 + 11 * year) / 30) + ISLAMIC_EPOCH - 1;
		return jd;*/
		jd_t jd = day + std::ceil(29.5 * (month - 1)) + (year - 1) * 354
			+ std::floor((3 + 11 * year) / 30.0) + ISLAMIC_EPOCH - 1;

		//std::cout << "Calculated JD: " << jd << std::endl;
		return jd;
	}

	inline jd_t islamic_to_jd_real(year_t year, month_t month, day_t day) {
		while (year < 1) {
			year += 1;
		}
		jd_t jd = day + ceil(29.5 * (month - 1)) + (year - 1) * 354 + floor((3 + 11 * year) / 30) + ISLAMIC_EPOCH - 1;
		return jd;
	}



	inline bool is_islamic_leapyear(year_t year) {
		if (((11 * year + 14) % 30) < 11) {
			return true;
		}
		else {
			return false;
		}
	}

	inline std::string islamic_month_name(int i) {
		switch (i) {
		case Muharram: return "Muharram";
		case Safar: return "Safar";
		case RabialAwwal: return "Rabi'al-Awwal";
		case RabiathThani: return "Rabi'ath-Thani";
		case JumadaAlAwwal: return "Jumada I-Ula";
		case JumadatTania: return "Jumada t-Tania";
		case Rajab: return "Rajab";
		case Shaban: return "Sha'ban";
		case Ramadan: return "Ramadan";
		case Shawwal: return "Shawwal";
		case DhulQadah: return "Dhu I-Qa'da";
		case DhulHijja: return "Dhu I-Hijja";
		default: return "Invalid Input";
		}
	}
	class Islamic {
		year_t		year_;
		month_t		month_;
		day_t		day_;
		hour_t		hour_;
		minute_t	minute_;
		second_t	second_;

	public:
		Islamic();
		Islamic(now_t NOTIMEOFDAY);
		Islamic(Jd j);

		void from_jd(jd_t jd) {
			jd_to_islamic(jd, year_, month_, day_, hour_, minute_, second_); 
			//void jd_to_islamic(jd_t jd, year_t & year, month_t & month, day_t & day, hour_t & hour, minute_t & minute, second_t & seconds)
		}
		jd_t to_jd() const { return islamic_to_jd(year_, month_, day_, hour_, minute_, second_); }
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
		Islamic(year_t year, month_t month, day_t day) : year_(year), month_(month), day_(day), hour_(0), minute_(0), second_(0) {}

		Islamic(year_t year, month_t month, day_t day, hour_t hour, minute_t min, second_t sec) : year_(year), month_(month), day_(day), hour_(hour), minute_(min), second_(sec) {}

		bool operator==(const Islamic& other) const
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
		Islamic& operator = (Jd const& jd) {
			from_jd(jd.jd());
			return *this;
		}

		int islamic_days_in_month(int month, bool is_leap_year) {
			// Islamic months typically have 30 or 29 days, with a specific pattern
			if (month < 1 || month > 12) {
				throw std::invalid_argument("Month must be between 1 and 12.");
			}

			// Months 1, 3, 5, 7, 9, and 11 have 30 days
			if (month % 2 != 0) {
				return 30;
			}
			// Month 12 has 30 days in a leap year
			else if (month == 12 && is_leap_year) {
				return 30;
			}
			// All other months have 29 days
			return 29;
		}



		Islamic operator + (khronos::packaged_day const& input)
		{
			Islamic res = *this;


			res.day_ += input.nDays_;
			while ((res.day_ / islamic_days_in_month(res.month(), is_islamic_leapyear(res.year_))) >= 1) {
				res.day_ -= islamic_days_in_month(res.month(), is_islamic_leapyear(res.year_));
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
		
		Islamic operator - (khronos::packaged_day const& input)
		{
			Islamic res = *this;
			res.day_ -= input.nDays_;
			while (res.day_ < 1) {
				int days_in_prev_month = islamic_days_in_month(res.month_, is_islamic_leapyear(res.year_));
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

		Islamic operator + (khronos::packaged_week const& input) {
			Islamic res = *this;
			day_t days = input.nWeeks_ * 7;
			res = res + packaged_day(input.nWeeks_ * 7);
			return res;
		}

		Islamic operator - (khronos::packaged_week const& input) {
			Islamic res = *this;
			day_t days = input.nWeeks_ * 7;
			std::cout << "No :" << days << std::endl;
			res = res - packaged_day(days);
			return res;
		}
		Islamic operator + (khronos::packaged_month_integer const& input) {
			Islamic res = *this;
			res.month_ += input.nMonths_;
			while (res.month_ > 12) {

				res.year_ = res.year_ + 1;
				res.month_ -= 12;

			}
			if (res.day_ > islamic_days_in_month(res.month_, is_islamic_leapyear(res.year_))) {
				res.day_ = islamic_days_in_month(res.month_, is_islamic_leapyear(res.year_));
			}
			return res;
		}
		Islamic operator - (khronos::packaged_month_integer const& input) {
			Islamic res = *this;
			res.month_ -= input.nMonths_;
			while (res.month_ < 1) {
				res.year_ = res.year_ - 1;
				res.month_ += 12;
			}
			return res;
		}
		Islamic operator + (khronos::packaged_year_integer const& year) {
			Islamic res = *this;
			res.year_ += year.nYears_;
			if (res.month_ == February && (res.day_ == 29)) {
				if (!is_islamic_leapyear(res.year_)) {
					res.day_ = 28;
				}
			}
			return res;
		}
		Islamic operator - (khronos::packaged_year_integer const& year) {
			Islamic res = *this;
			res.year_ -= year.nYears_;
			if (res.month_ == February && (res.day_ == 29)) {
				if (!is_islamic_leapyear(this->year_)) {
					res.day_ = 28;
				}
			}
			return res;
		}

		bool operator < (const Islamic& other) const
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

		bool operator <= (const Islamic& other) const
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

		bool operator >= (const Islamic& other) const
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

		bool operator > (const Islamic& other) const
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


		inline jd_t operator-(const Islamic& other) {
			return this->to_jd() - other.to_jd();
		}
		inline jd_t operator-(const Jd& other) {
			return this->to_jd() - other.jd();
		}


		std::string to_string() const {
			std::ostringstream oss;

			day_t day_of_week = khronos::civil::day_of_week(this->to_jd());
			std::string day_name_str = khronos::civil::day_name(day_of_week);
			std::string month_name = islamic_month_name(month_);

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

		friend std::ostream& operator<<(std::ostream& os, const Islamic& g) {
			os << g.to_string();
			return os;
		}

	};


	/*inline year_t year(const Islamic& g) { return g.year(); }
	inline month_t month(const Islamic& g) { return g.month(); }
	inline day_t day(const Islamic& g) { return g.day(); }
	inline hour_t hour(const Islamic& g) { return g.hour(); }
	inline minute_t minute(const Islamic& g) { return g.minute(); }
	inline second_t second(const Islamic& g) { return g.second(); }*/


} // namespace khronos
