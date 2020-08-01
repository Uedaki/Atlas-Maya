#pragma once

#include <sstream>
#include <atltime.h>

#include <chrono>

class Stopwatch
{
public:
	Stopwatch()
	{
		start = std::chrono::steady_clock::now();
	}

	const std::string elapsed()
	{
		std::ostringstream os;

		if (isTicking)
			end = std::chrono::steady_clock::now();
		uint32_t total = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

		uint32_t hours = total / 60 / 60 / 1000 / 1000;
		uint32_t minutes = (total / 60 / 1000 / 1000) % 60;
		uint32_t seconds = (total / 1000 / 1000) % 60;
		uint32_t milliseconds = (total / 1000) % 1000;
		uint32_t nanoseconds = total % 1000;

		if (hours != 0)
			os << hours << " hours ";
		if (hours != 0 || minutes != 0)
			os << minutes << " minutes ";
		if (hours != 0 || minutes != 0 || seconds != 0)
			os << seconds << " seconds ";
		if (hours == 0 && minutes == 0)
			os << milliseconds << " milliseconds";
		if (hours == 0 && minutes == 0 && seconds == 0)
			os << " " << nanoseconds << " nanoseconds";

		return (os.str());
	}

	void stop()
	{
		isTicking = false;
		end = std::chrono::steady_clock::now();
	}

	static const std::string getTime()
	{
		std::ostringstream os;

		CTime time = CTime::GetCurrentTime();
		os << time.GetHour() << ":" << time.GetMinute() << ":" << time.GetSecond();
		return (os.str());
	}

    static const std::string getDate()
    {
        std::ostringstream oss;

        CTime time = CTime::GetCurrentTime();
        oss << time.GetDay() << "/" << time.GetMonth() << "/" << time.GetYear();
        return oss.str();
    }

private:
	bool isTicking = true;
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
};