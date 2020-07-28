#pragma once

#include <fstream>

#include <Maya/MIOStream.h>

#include "Exception.h"
#include "Watch.h"

#define LOG_INFO(msg) Logger::get().write(Logger::Type::INFO, msg)
#define LOG_WARNING(msg) Logger::get().write(Logger::Type::WARNING, msg)
#define LOG_CRITICAL(msg) Logger::get().write(Logger::Type::CRITICAL, msg)
#define LOG_EXCEPTION(error) Logger::get().writeException(error)

#ifdef DEV
# define LOG_DEBUG(msg) Logger::get().write(Logger::Type::DEBUG, msg)
#else
# define LOG_WARNING(msg)
#endif

class Logger
{
public:
    enum class Type : uint8_t
    {
        INFO,
        DEBUG,
        WARNING,
        CRITICAL
    };

    static Logger &get()
    {
        static Logger log;
        return (log);
    }

    ~Logger()
    {
        if (file)
        {
            file << "\n  ===============================================\n"
                << "    End   Output log ( "
                << Watch::getDate()
                << " at "
                << Watch::getTime()
                << " ): "
                << "\n  ===============================================\n\n";
            file.flush();
            file.close();
        }
    }

    void defineOuputFile(const std::string &filename)
    {
        file.open(filename, std::ios::app);
        file.seekp(std::ios::beg);

        if (!file.good()) return;
        file << "  ===============================================\n"
            << "    Begin Output log ( "
            << Watch::getDate()
            << " at "
            << Watch::getTime()
            << " ): "
            << "\n  ===============================================\n\n";
        file.flush();
    }

    void write(Type type, const std::string &msg)
    {
        std::ostringstream os;
        os << "[" << Watch::getTime() << "][";
        switch (type)
        {
        default:
        case Logger::Type::INFO:
            os << "INFO";
            break;
        case Logger::Type::DEBUG:
            os << "DEBUG";
            break;
        case Logger::Type::WARNING:
            os << "WARNING";
            break;
        case Logger::Type::CRITICAL:
            os << "CRITICAL";
            break;
        }
        os << "]: " << msg << "\n";

        std::cout << os.str();
        if (file)
        {
            file << os.str();
            file.flush();
        }
    }

    void writeException(const Exception &exception)
    {
        std::ostringstream os;
        os << "  ==========  EXCEPTION  ==========  " << "\n"
            << "Exception found in function " << exception.getFunction() << "\n"
            << "File: " << exception.getFile() << "\n"
            << "Line: " << exception.getLine() << "\n"
            << "  -------------  MSG  -------------  " << "\n"
            << exception.getMsg() << "\n";
        std::cout << os.str();
        if (file)
        {
            file << os.str();
            file.flush();
        }
    }

private:
    std::ofstream file;
};