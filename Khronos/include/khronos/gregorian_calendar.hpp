#pragma once
/*
khronos\gregorian_calendar.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'Gregorian calendar' declarations.
*/


#include <khronos/def.hpp>
#include <khronos/utility.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <khronos/julian_calendar.hpp>
#include <string>
#include <type_traits>
#include <khronos.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <khronos/islamic_calendar.hpp>

namespace khronos {
	constexpr jd_t GREGORIAN_EPOCH = 1721425.5;


	// LITERALS
	/* UDL - converts a Gregorian year CE to an astronomical Gregorian year. */
	constexpr year_t operator"" _CE(unsigned long long year) {
		return static_cast<year_t>(year);
	}

	/* UDL - converts a Gregorian year BCE to an astronomical Gregorian year. */
	constexpr year_t operator"" _BCE(unsigned long long gregorianYearBCE) { return static_cast<year_t>(1 - gregorianYearBCE);  // Astronomical year (1 BCE = 0, 2 BCE = -1, etc.)
	}

	struct packaged_day;

	// FUNCTIONS
	// --------------------------------------------------------------------------------------
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day);

	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds);

	// OPERATIONS
	/*!	Leap year test for Proleptic Gregorian Calendar.
	\return 'true' if leap year, 'false' if not.
	\param year [in] Astronomical year (1 CE = 1, 1 BCE = 0, 2 BCE = -1, etc.)
	*/


	constexpr bool is_gregorian_leapyear(year_t year) {
		return (year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0);
	}


	/** Provide the number of days in the month of the Gregorian Calendar. */
	constexpr day_t gregorian_days_in_month(month_t month, bool isLeapYear) {
		return civil::days_in_month(month, isLeapYear);
	}

	/** Provide the name of the given month in the Gregorian calendar. */
	constexpr char const* gregorian_month_name(month_t month) {
		return civil::month_name_long(month);
	}

	/** Provide the short name of the given month in the Gregorian calendar. */
	constexpr char const* gregorian_short_month_name(month_t month) {
		return civil::month_name_short(month);
	}

	
	
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day);

	
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second);
	

	inline jd_t greg2jd_integer(year_t year, month_t month, day_t day) {
		long long a = (14 - month) / 12;
		long long y = year + 4800 - a;
		long long m = month + (12 * a) - 3;
		return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045.5;
	}

	inline jd_t greg2jd_real(year_t year, month_t month, day_t dayOfMonth) {
		long long y = year - 1;
		return
			GREGORIAN_EPOCH - 1
			+ 365 * y
			+ floor(y / 4.0)
			- floor(y / 100.0)
			+ floor(y / 400.0)
			+ floor(((367.0 * month - 362.0) / 12.0)
				+ ((month <= 2)
					? 0
					: (is_gregorian_leapyear(year) ? -1 : -2)))
			+ dayOfMonth;
	}

	

	// CLASSES
	// --------------------------------------------------------------------------------------

	class Jd;
	class Julian;

	/**	Proleptic Gregorian Calendar Date class. */
	class Gregorian {
		year_t		year_;
		month_t		month_;
		day_t		day_;
		hour_t		hour_;
		minute_t	minute_;
		second_t	second_;
		void from_jd(jd_t jd) { jd_to_gregorian(jd, year_, month_, day_, hour_, minute_, second_); }
	public:
		
		Gregorian();
		Gregorian(now_t NOTIMEOFDAY);
		Gregorian(Jd j);
		Gregorian(Julian J) {
			year_ = J.year();
			month_ = J.month();
			day_ = J.day();
			hour_ = J.hour();
			minute_ = J.minute();
			second_ = J.second();
		}
		jd_t to_jd() const { return gregorian_to_jd(year_, month_, day_, hour_, minute_, second_); }
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
		Gregorian(year_t year, month_t month, day_t day) : year_(year), month_(month), day_(day), hour_(0), minute_(0), second_(0) {}

		Gregorian(year_t year, month_t month, day_t day, hour_t hour, minute_t min,second_t sec) : year_(year), month_(month), day_(day) ,hour_(hour), minute_(min),second_(sec){}
		
		/*operator Jd () const { return Jd(to_jd()); }*/


		///**	Assign and convert from Jd type to Gregorian type. */
		Gregorian& operator = (Jd const& jd) {
			from_jd(jd.jd());
			return *this;
		}

		bool operator==(const Gregorian& other) const
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

		bool operator==(const Julian& other) const
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
		}

		bool operator==(const Islamic& other) const
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
		}

		/*bool operator==(const Hebrew& other) const
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

		bool operator < (const Gregorian& other) const
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

		bool operator <= (const Gregorian& other) const
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

		bool operator >= (const Gregorian& other) const
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

		bool operator > (const Gregorian& other) const
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

		jd_t operator-(const Gregorian& other) const {
			return this->to_jd() - other.to_jd();
		}


		inline jd_t operator- (Jd& other ) {
			return (this->to_jd() - other.jd());
		}

		bool operator==(const Jd& other) const { return this->to_jd() == other.jd(); }
		bool operator!=(const Jd& other) const { return this->to_jd() != other.jd(); }
		bool operator<(const Jd& other) const { return this->to_jd() < other.jd(); }
		bool operator<=(const Jd& other) const { return this->to_jd() <= other.jd(); }
		bool operator>(const Jd& other) const { return this->to_jd() > other.jd(); }
		bool operator>=(const Jd& other) const { return this->to_jd() >= other.jd(); }

		bool operator<=(const Julian& other) const { return this->to_jd() <= other.to_jd(); }

		friend inline bool operator!=(Jd const& j, Gregorian const& g) {
			return j.jd() != g.to_jd();
		}
		/** Construct a Gregorian date from Julian Day Number object.
			@param jd [in] Jd object.
			*/
		/*Gregorian(Jd const& jd) { from_jd(jd.jd()); }*/


		//std::string to_string() const;


		/**	Implicit cast to Jd class. */
		/*operator Jd () const { return Jd(to_jd()); }*/


		/**	Assign and convert from Jd type to Gregorian type. */
		/*Gregorian& operator = (Jd const& jd) {
			from_jd(jd.jd());
			return *this;
		}*/


		// block some operators
	
		/*Gregorian operator + (packaged_year_real const& input)
		{
			this->year_ += input.nYears_;
			return *this;
		}*/

		Gregorian operator + (khronos::packaged_day const& input)
		{
			Gregorian res = *this;


			res.day_ += input.nDays_;
			while ((res.day_ / gregorian_days_in_month(res.month(), is_gregorian_leapyear(res.year_))) >= 1) {
				res.day_ -= gregorian_days_in_month(res.month(), is_gregorian_leapyear(res.year_));
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

		Gregorian operator - (khronos::packaged_day const& input)
		{
			Gregorian res = *this;
			res.day_ -= input.nDays_; 
			while (res.day_ < 1) {
				int days_in_prev_month = gregorian_days_in_month(res.month_, is_gregorian_leapyear(res.year_));
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


		Gregorian operator + (khronos::packaged_week const& input) {
			Gregorian res = *this;
			day_t days = input.nWeeks_ * 7;
			res = res + packaged_day(input.nWeeks_ * 7);
			return res;
		}

		Gregorian operator - (khronos::packaged_week const& input) {
			Gregorian res = *this;
			day_t days = input.nWeeks_ * 7;
			std::cout << "No :" << days << std::endl;
			res = res - packaged_day(days);
			return res;
		}

	//	Gregorian operator - (detail::packaged_year_real const&);
		//Gregorian operator + (khronos::packaged_month_real const& input) {
		//	Gregorian res = *this;
		//	res.month_ = input.nMonths_;
		//	while (res.month_ > 12) {
		//		res.year_ = res.year_ + 1;
		//		res.month_ -= 12;
		//	}
		//	return res;
		//}
		inline Gregorian operator + (khronos::packaged_month_integer const& input) {
			Gregorian res = *this;
			res.month_ += input.nMonths_;
			while (res.month_ > 12) {
				
				res.year_ = res.year_ + 1;
				res.month_ -= 12;
				
			}
			if (res.day_ > gregorian_days_in_month(res.month_, is_gregorian_leapyear(res.year_))) {
				res.day_ = gregorian_days_in_month(res.month_, is_gregorian_leapyear(res.year_));
			}
			return res;
		}
		Gregorian operator - (khronos::packaged_month_integer const& input) {
			Gregorian res = *this;
			res.month_ -= input.nMonths_;
			while (res.month_ < 1) {
				res.year_ = res.year_ - 1;
				res.month_ += 12;
			}
			return res;
		}
		inline Gregorian operator + (khronos::packaged_year_integer const& year) {
			Gregorian res = *this;
			res.year_ += year.nYears_;
			if (res.month_ == February && (res.day_ == 29)) {
				if (!is_gregorian_leapyear(res.year_)) {
					res.day_ = 28;
				}
			}
			return res;
		}
		Gregorian operator - (khronos::packaged_year_integer const& year) {
			Gregorian res = *this;
			res.year_ -= year.nYears_;
			if (res.month_ == February && (res.day_ == 29)) {
				if (!is_gregorian_leapyear(this->year_)) {
					res.day_ = 28;
				}
			}
			return res;
		}

		std::string to_string() const{
			std::ostringstream oss;

			day_t day_of_week = khronos::civil::day_on_date(day_, month_, year_);
			std::string day_name_str = khronos::civil::day_name(day_of_week);
			std::string month_name = gregorian_month_name(month_);

			oss << day_name_str << ", " << month_name << " " << day_ << " ";

			if (year_ > 0) {
				oss << year_ << " CE";
			}
			else {
				oss << (-year_ + 1) << " BCE"; // Correctly convert to BCE
			}

			oss << ", " << std::setw(2) << std::setfill('0') << (hour_ % 12 == 0 ? 12 : hour_ % 12)
				<< ":" << std::setw(2) << std::setfill('0') << minute_
				<< ":" << std::setw(2) << std::setfill('0') << second_
				<< (hour_ >= 12 ? " pm" : " am");

			return oss.str();
		}

		friend std::ostream& operator<<(std::ostream& os, const Gregorian& g) {
			os << g.to_string(); 
			return os;
		}

	//	Gregorian operator - (detail::packaged_month_real const&);
	};

	/*Gregorian operator + (Gregorian const& dt, packaged_day day)
	{

		Gregorian temp(dt);
		temp = temp + day;
		return temp;
	}*/


	//// OPERATORS
	//// ====================

	///** Gregorian + (integer month) */
	//Gregorian operator + (Gregorian const& dt, packaged_month_integer const& month);

	
	///** Gregorian - (integer month) */
	//inline Gregorian operator - (Gregorian const& dt, detail::packaged_month_integer const& month) { return dt + detail::packaged_month_integer(-month.nMonths_); }

	///**	Gregorian + (integer year) */
	//Gregorian operator + (Gregorian const& dt, detail::packaged_year_integer const& year);

	///**	Gregorian - (integer year) */
	//inline Gregorian operator - (Gregorian const& dt, detail::packaged_year_integer const& year) { return dt + detail::packaged_year_integer(-year.nYears_); }

	///** Stream insertion operator. */
	///*inline std::ostream& operator << (std::ostream& os, Gregorian const& g) { return os << g.to_string(); }*/
	inline year_t year(const Gregorian& g) { return g.year();}
	inline month_t month(const Gregorian& g) { return g.month(); }
	inline day_t day(const Gregorian& g) { return g.day(); }
	inline hour_t hour(const Gregorian& g) { return g.hour(); }
	inline minute_t minute(const Gregorian& g) { return g.minute(); }
	inline second_t second(const Gregorian& g) { return g.second(); }


}; // end-of-namespace khronos

