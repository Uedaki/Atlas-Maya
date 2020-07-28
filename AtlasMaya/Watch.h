#pragma once

#include <sstream>
#include <atltime.h>

class Watch
{
public:
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
};