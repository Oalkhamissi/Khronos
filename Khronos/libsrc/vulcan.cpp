/**	@file vulcan.cpp
	@author Garth Santor
	@date 2014-09-29

	Vulcan calendar class implementation.
	*/

#include <khronos/vulcan_calendar.hpp>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>
#include <array>


namespace khronos {
	inline year_t year(const Vulcan& g) { return g.year(); }
	inline month_t month(const Vulcan& g) { return g.month(); }
	inline day_t day(const Vulcan& g) { return g.day(); }
	inline hour_t hour(const Vulcan& g) { return g.hour(); }
	inline minute_t minute(const Vulcan& g) { return g.minute(); }
	inline second_t second(const Vulcan& g) { return g.second(); }
}
