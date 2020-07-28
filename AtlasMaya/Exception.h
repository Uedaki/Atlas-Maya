#pragma once

#include <string>
#include <stdexcept>

#ifdef FULL_EXCEPTION
# define EXCEPTION(msg) Exception(msg)
#else
# define EXCEPTION(msg) Exception(msg, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#endif

class Exception : public std::exception
{
public:
	Exception(const std::string &msg)
		: msg(msg)
	{}

	Exception(const std::string &msg, const char *file, const char *function, int line)
		: msg(msg), file(file), line(line)
	{}

	virtual const char *what() const noexcept
	{
		return (msg.c_str());
	}

	inline const std::string &getMsg() const { return (msg); }
	inline const std::string &getFile() const { return (file); }
	inline const std::string &getFunction() const { return (function); }
	inline const int getLine() const { return (line); }

private:
	std::string msg;
	std::string file;
	std::string function;
	int line;
};